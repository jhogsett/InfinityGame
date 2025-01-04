#include "buttons.h"
#include "buffers.h"
#include "displays.h"
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

#define MOVE_NONE 0
#define MOVE_ROL 1
#define MOVE_FLIP 2
#define MOVE_ROR 3
#define MOVE_MIN 1
#define MOVE_MAX 3

int shuffle_word(char *buffer, int size, int min_times, int max_times){
	int times = random(min_times, max_times+1);
	int last_move = MOVE_NONE;

	for(int i = 0; i < times; i++){
		int move;
		while((move = random(MOVE_MIN, MOVE_MAX+1)) == last_move)
			;
		last_move = move;
		switch(move){
			case MOVE_ROL:
				rotate_left(buffer, size);
				break;
			case MOVE_FLIP:
				flip(buffer, size);
				break;
			case MOVE_ROR:
				rotate_right(buffer, size);
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
	strcpy(letters, FSTR("ETAOINSRHDLU"));

	char add_chars[ADD_CHARS_BUFFER_SIZE];
	for(int i = 0; i < ADD_CHARS; i++)
	{
		int letter = random(LETTERS_LEN);
		add_chars[i] = letters[letter];
		// add_chars[i] = (char)random((int)'A', (int)'Z' + 1);
	}
	add_chars[ADD_CHARS_BUFFER_SIZE-1] = '\0';

	sprintf(scramble_word, "%s%s", chosen_word, add_chars);

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
	sprintf(display_buffer, FSTR("ROL FLIP ROR"));
	title_prompt(display_buffer, INSTRUCTION_SHOW_TIMES, false, ROUND_DELAY);

	int scramble_moves = choose_word(rude);
	int player_moves = 0;

	unsigned long idle_timeout = millis() + IDLE_TIMEOUT;
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

		idle_timeout = millis() + IDLE_TIMEOUT;

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

void word_game(){
	title_prompt(FSTR("The WordGame"), TITLE_SHOW_TIMES, true);

	bool rude;
	const bool buttons[] = {false, true, false, true};
	switch(button_led_prompt(FSTR("NICE    RUDE"), buttons)){
	case -1:
		return;
	case 0:
		return;
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

	unsigned long idle_timeout = millis() + IDLE_TIMEOUT;
	unsigned long time;

	while((time = millis()) < idle_timeout){
		int round_result = word_game_round(rude);
		switch(round_result){
			case -1:
				// timed out of long press
				return;
			case 0:
				// exceeded max moves
				sprintf(display_buffer, FSTR("Out Of Moves"));
				title_prompt(display_buffer, EXCEEDED_SHOW_TIMES, false, ROUND_DELAY);
				continue;
		}
		sprintf(display_buffer, FSTR("    %s    "), chosen_word);
		title_prompt(display_buffer, SUCCESS_SHOW_TIMES, false, ROUND_DELAY);

		sprintf(display_buffer, FSTR("%s%s%s"), chosen_word, chosen_word, chosen_word);
		title_prompt(display_buffer, SUCCESS_SHOW_TIMES, true, ROUND_DELAY);

		int win = 0;
#ifdef ENABLE_WIN_IN_1
		if(round_result == -2){
			// win in one move
			win = WIN_IN_1_BONUS * BASE_WIN_CASE;
			display_win(win);
		} else
#endif
		{
			// subtract one, added in the found to ensure every round is not a win
			win = (round_result-1) * BASE_WIN_CASE;
			if(win > 0)
				display_win(win);
		}

		if(win > 0){
			purse += win;
			save_data();
			display_purse();
		}
	}
}
