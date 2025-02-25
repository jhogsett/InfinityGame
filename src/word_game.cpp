#include "bank.h"
#include "buttons.h"
#include "buffers.h"
#include "displays.h"
#include "leds.h"
#include "prompts.h"
#include "play_data.h"
#include "play_views.h"
#include "seeding.h"
#include "streak.h"
#include "timeouts.h"
#include "utils.h"
#include "word_lists.h"
#include "word_game.h"

char chosen_word[WORD_BUFFER_SIZE];
char scramble_word[SCRAMBLE_BUFFER_SIZE];
bool new_game = true;

// rotation size of buffer not including null terminator
void rotate_left(char *buffer, int size){
	char carry = buffer[0];
	for(int i = 0; i < size-1; i++){
		buffer[i] = buffer[i+1];
	}
	buffer[size-1] = carry;
}

void rotate_right(char *buffer, int size){
	char carry = buffer[size-1];
	for(int i = size-1; i > 0; i--){
		buffer[i] = buffer[i-1];
	}
	buffer[0] = carry;
}

// size must be a multiple of two
void flip(char *buffer, int size){
	int half_size = size / 2;
	for(int i = 0; i < half_size; i++){
		char carry = buffer[i];
		buffer[i] = buffer[(size-1) -i];
		buffer[(size-1) -i] = carry;
	}
}

#define MOVE_ROTATE 0
#define MOVE_FLIP 1

int shuffle_word(char *buffer, int size, int min_times, int max_times){
	int times = random(min_times, max_times+1);

	// choose a direction to rotate ahead of time
	bool go_right = random(2) ? true : false;

	for(int i = 0; i < times; i++){
		switch(random(2) ? MOVE_FLIP : MOVE_ROTATE){
			case MOVE_ROTATE:
				go_right ? rotate_right(buffer, size) : rotate_left(buffer, size);
				break;
			case MOVE_FLIP:
				flip(buffer, size);
				break;
		}
	}
	return times;
}

void format_scamble_word(char *buffer){
	for(int i = 0; i < WORD_WIDTH; i++){
		buffer[i] = scramble_word[i];
	}
	buffer[WORD_BUFFER_SIZE- 1] = '\0';
}

void format_scamble_word_display(char *buffer){
	char show_word[WORD_BUFFER_SIZE];
	format_scamble_word(show_word);
	sprintf_P(buffer, PSTR("rol %s ror"), show_word);
}

#define LETTERS_LEN 12
#define LETTERS_BUFFER_LEN 13

int last_word_choice = -1;

// returns the number of random shifts done
int choose_word(bool rude){
	const char **words;
	if(rude)
		words = rude_words;
	else
		words = nice_words;

	randomizer.randomize();

	int word_choice;
	while((word_choice = random(NUM_WORDS)) == last_word_choice)
		word_choice = random(NUM_WORDS);
	last_word_choice = word_choice;

	strcpy(chosen_word, words[word_choice]);

	// https://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
	char letters[LETTERS_BUFFER_LEN];
	load_f_string(F("ETAOINSRHDLU"), letters);

	char add_chars[ADD_CHARS_BUFFER_SIZE];
	for(int i = 0; i < ADD_CHARS; i++)
	{
		int letter = random(LETTERS_LEN);
		add_chars[i] = letters[letter];
	}
	add_chars[ADD_CHARS_BUFFER_SIZE-1] = '\0';

	sprintf_P(scramble_word, PSTR("%s%s"), chosen_word, add_chars);

	int scramble_moves = 0;

	char show_word[WORD_BUFFER_SIZE];
	format_scamble_word(show_word);

	while(strcmp(show_word, chosen_word) == 0){
		scramble_moves = shuffle_word(scramble_word, SCRAMBLE_SIZE, SHUFFLE_TIMES_MIN, SHUFFLE_TIMES_MAX);
		format_scamble_word(show_word);
	}

	return scramble_moves;
}

// returns -1 on time out or long press, 0 of not beaten, otherwise winning factor 0 or more
// returns -2 if player exceeds maximum moves
int word_game_round(bool rude){
	int instruction_times = new_game ? CONTROLS_SHOW_TIMES : 1;
    int instruction_delay = new_game ? ROUND_DELAY : INSTRUCTIONS_SHOW_DELAY;
	int instruction_show_leds = new_game;
	if(title_prompt(FSTR("ROL FLIP ROR"),
					instruction_times,
					instruction_show_leds,
					instruction_delay,
					TITLE_PANEL_LEDS_STYLE2,
					TITLE_PANEL_LEDS_SHOW_TIME2,
					TITLE_PANEL_LEDS_BLANK_TIME2))
		return -1;
	new_game = false;

	int scramble_moves = choose_word(rude);
	int player_moves = 0;

    if(title_prompt_int(FSTR("BEAT %d MOVES"), scramble_moves, false, BEAT_SHOW_DELAY))
		return -1;

	pay_house(use_purse(WORD_GAME_PLAY_BET));

	unsigned long idle_timeout = millis() + option_idle_time;
	unsigned long time;

	while((time = millis()) < idle_timeout){
		format_scamble_word_display(display_buffer);
		const bool states[] = {false, true, true, true};
		switch(button_led_prompt(display_buffer, states, true)){
			case -1:
				return -1;
			case 0:
				return -1;
			case 1:
				rotate_left(scramble_word, SCRAMBLE_SIZE);
				break;
			case 2:
				flip(scramble_word, WORD_WIDTH);
				break;
			case 3:
				rotate_right(scramble_word, SCRAMBLE_SIZE);
				break;
		}

		idle_timeout = millis() + option_idle_time;

		while(button_pressed())
			;

		if(player_moves > MAX_MOVES){
			return -2;
		} else {
			player_moves++;
		}

		char show_word[WORD_BUFFER_SIZE];
		format_scamble_word(show_word);

		if(strcmp(show_word, chosen_word) == 0){
			// word found
            title_prompt_string(FSTR("    %s    "), chosen_word, false, CORRECT_WORD_SHOW_TIME);

            title_prompt_int(FSTR("  MOVES %d"), player_moves, false, MOVES_SHOW_DELAY);

			// compute winning factor
			int factor = scramble_moves - player_moves;
			if(factor < 0)
				factor = 0;
			return factor;
		}
	}
	return -1;
}

bool word_game(){
	// reset this flag to show extra instruction on the first play
	new_game = true;

	if(title_prompt(FSTR("The WordGame"), TITLE_SHOW_TIMES, true))
        return false;

    bool rude = false;
	switch(prompt_nice_or_rude()){
        case -1:
            return false;
        case 1:
            rude = true;
    }

	if(title_prompt(FSTR("BUTTONS ROTATE/FLIP"), INSTRUCTIONS_SHOW_TIMES, false, ROUND_DELAY))
		return false;

    if(show_instr_long_press()){
        return false;
    }
    reset_streak();

	unsigned long idle_timeout = millis() + option_idle_time;
	unsigned long time;

	while((time = millis()) < idle_timeout){
		long win = 0;
		int round_result = word_game_round(rude);

		idle_timeout = millis() + option_idle_time;

		switch(round_result){
			case -2:
				// exceeded max moves
                cancel_streak();
				title_prompt(FSTR("OUT OF MOVES"), EXCEEDED_SHOW_TIMES, false, EXCEEDED_SHOW_DELAY);
				break;
			case -1:
				// timed out or long press
				// refund their bet
				add_to_purse(house_payout(WORD_GAME_PLAY_BET));
				save_data();
				return false;
			case 0:
				// player didn't beat the moves
                cancel_streak();
				break;
			default:
                title_prompt_string3(FSTR("%s%s%s"), chosen_word, chosen_word, chosen_word, true, SUCCESS_SHOW_DELAY);

				win = (round_result) * (WORD_GAME_WIN_UNIT);

				// apply the current streak bonus before showing next activation
                win *= streak_bonus();

				if(win > 0){
					display_win(win, WG_WIN_SHOW_DELAY);
                    add_streak();
				}

				add_to_purse(house_payout(win));
                save_data();
                display_purse(WG_WIN_SHOW_DELAY);
                display_bonus();

				break;
		}

        display_bonus_gone();
	}
	return false;
}
