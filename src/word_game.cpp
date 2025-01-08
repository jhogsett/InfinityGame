#include "bank.h"
#include "buttons.h"
#include "buffers.h"
#include "displays.h"
#include "leds.h"
#include "prompts.h"
#include "seeding.h"
#include "play_data.h"
#include "play_views.h"
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
	sprintf(buffer, FSTR("rol %s ror"), show_word);
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
	// strcpy(letters, FSTR("ETAOINSRHDLU"));
	load_f_string(F("ETAOINSRHDLU"), letters);

	char add_chars[ADD_CHARS_BUFFER_SIZE];
	for(int i = 0; i < ADD_CHARS; i++)
	{
		int letter = random(LETTERS_LEN);
		add_chars[i] = letters[letter];
		// add_chars[i] = (char)random((int)'A', (int)'Z' + 1);
	}
	add_chars[ADD_CHARS_BUFFER_SIZE-1] = '\0';

	sprintf(scramble_word, FSTR("%s%s"), chosen_word, add_chars);

	int scramble_moves = 0;

	char show_word[WORD_BUFFER_SIZE];
	format_scamble_word(show_word);

	while(strcmp(show_word, chosen_word) == 0){
		// scramble_moves += shuffle_word(scramble_word, SCRAMBLE_SIZE, SHUFFLE_TIMES_MIN, SHUFFLE_TIMES_MAX);
		scramble_moves = shuffle_word(scramble_word, SCRAMBLE_SIZE, SHUFFLE_TIMES_MIN, SHUFFLE_TIMES_MAX);
		format_scamble_word(show_word);
	}

	// // rotate the real word into the middle
	// for(int i = 0; i < ADD_CHARS / 2; i++){
	// 	rotate_right(scramble_word, SCRAMBLE_SIZE);
	// 	scramble_moves++;

	return scramble_moves;
}

// returns -1 on time out or long press, 0 if player exceeds maximum moves, otherwise winning factor
// returns -2 if user wins in one move
int word_game_round(bool rude){
	int instruction_times = new_game ? CONTROLS_SHOW_TIMES : 1;
	int instruction_show_leds = new_game;
	sprintf(display_buffer, FSTR("ROL FLIP ROR"));
	title_prompt(display_buffer, instruction_times, instruction_show_leds, ROUND_DELAY, TITLE_PANEL_LEDS_STYLE2, TITLE_PANEL_LEDS_SHOW_TIME2, TITLE_PANEL_LEDS_BLANK_TIME2);
	new_game = false;

	int scramble_moves = choose_word(rude);
	int player_moves = 0;

	unsigned long idle_timeout = millis() + option_idle_time;
	unsigned long time;

	while((time = millis()) < idle_timeout){
		format_scamble_word_display(display_buffer);
		const bool states[] = {false, true, true, true};
		switch(button_led_prompt(display_buffer, states)){
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
			return 0;
		} else {
			player_moves++;
		}

		// word found, compute winning factor
		char show_word[WORD_BUFFER_SIZE];
		format_scamble_word(show_word);

		if(strcmp(show_word, chosen_word) == 0){
#ifdef ENABLE_WIN_IN_1
			if(player_moves == 1)
				return -2;
#endif
			int factor = scramble_moves - player_moves;
			if(factor < 1)
				factor = 1;
			return factor + 1; // hack return 1+ the factor and subtract on use, otherwise every time is a win
		}
	}
	return -1;
}

bool word_game(){
	// reset this flag to show extra instruction on the first play
	new_game = true;

	title_prompt(FSTR("The WordGame"), TITLE_SHOW_TIMES, true);

	bool rude;
	const bool buttons[] = {false, true, false, true};
	switch(button_led_prompt(FSTR("NICE or RUDE"), buttons)){
	case -1:
	case 0:
		return false;
	case 1:
		rude = false;
		break;
	case 2:
		rude = random(2) == 0 ? true : false;
		break;
	case 3:
		rude = true;
		break;
	}

	sprintf(display_buffer, FSTR("Buttons ROTATE and FLIP"));
	title_prompt(display_buffer, INSTRUCTIONS_SHOW_TIMES, false, ROUND_DELAY);
	sprintf(display_buffer, FSTR("LONG PRESS EXITS"));
	title_prompt(display_buffer, INSTRUCTIONS_SHOW_TIMES, false, ROUND_DELAY);

	unsigned long idle_timeout = millis() + option_idle_time;
	unsigned long time;

	while((time = millis()) < idle_timeout){
		pay_house(use_purse(WORD_GAME_PLAY_BET));
		long win = 0;
		bool purse_change = false;
		int round_result = word_game_round(rude);

		idle_timeout = millis() + option_idle_time;

		switch(round_result){
			case -1:
				// timed out of long press
				return false;
			case 0:
				// exceeded max moves
				sprintf(display_buffer, FSTR("Out Of Moves"));
				title_prompt(display_buffer, EXCEEDED_SHOW_TIMES, false, ROUND_DELAY);

				// pay_house(use_purse(WORD_GAME_PLAY_BET));
				// purse_change = true;
				break;
			default:
				sprintf(display_buffer, FSTR("    %s    "), chosen_word);
				title_prompt(display_buffer, SUCCESS_SHOW_TIMES, false, CORRECT_WORD_SHOW_TIME);

				sprintf(display_buffer, FSTR("%s%s%s"), chosen_word, chosen_word, chosen_word);
				title_prompt(display_buffer, SUCCESS_SHOW_TIMES, true, ROUND_DELAY);

				win = (round_result-1) * WORD_WIN_UNIT;
				if(win > 0)
					display_win(win);

				// pay_house(use_purse(WORD_GAME_PLAY_BET));
				add_to_purse(house_payout(win));
				purse_change = true;
				break;
		}


		// int win = 0;
#ifdef ENABLE_WIN_IN_1
		// if(round_result == -2){
		// 	// win in one move
		// 	win = WIN_IN_1_BONUS * WORD_WIN_UNIT;
		// 	display_win(win);
		// } else
#endif

		if(purse_change){
			save_data();
			display_purse();
		}
	}
	return false;
}
