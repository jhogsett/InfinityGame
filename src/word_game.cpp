#include "betting.h"
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
char scramble_word[WORD_BUFFER_SIZE];

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

#define SHUFFLE_TIMES_MIN 3
#define SHUFFLE_TIMES_MAX 12

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

	const char **words;
	if(rude)
		words = rude_words;
	else
		words = nice_words;

	sprintf(display_buffer, FSTR("<-- Flip -->"));
	title_prompt(display_buffer, INSTRUCTION_SHOW_TIMES, false, ROUND_DELAY);

	randomizer.randomize();
	int word_choice = random(NUM_WORDS);
	strcpy(chosen_word, words[word_choice]);
	strcpy(scramble_word, words[word_choice]);

	int scramble_moves = 0;
	while(strcmp(scramble_word, chosen_word) == 0){
		scramble_moves = shuffle_word(scramble_word, WORD_WIDTH, SHUFFLE_TIMES_MIN, SHUFFLE_TIMES_MAX);
	}
	int player_moves = 0;

	unsigned long idle_timeout = millis() + IDLE_TIMEOUT;
	unsigned long time;

	while((time = millis()) < idle_timeout){
		// bet_amounts[BET_ALL] = purse;
		sprintf(display_buffer, FSTR("<-- %s -->"), scramble_word);
		const bool states[] = {false, true, true, true};
		switch(button_led_prompt(display_buffer, states)){
			case -1:
				return;
			case 0:
				return;
			case 1:
				player_moves++;
				rotate_left(scramble_word, WORD_WIDTH);
				break;
			case 2:
				player_moves++;
				flip(scramble_word, WORD_WIDTH);
				break;
			case 3:
				player_moves++;
				rotate_right(scramble_word, WORD_WIDTH);
				break;
		}

		idle_timeout = millis() + IDLE_TIMEOUT;

		// int win = 0;
		// bool jackpot = false;
		// purse -= bet_amounts[current_bet];
		// save_data();

		// slots_round(rude);

		while(button_pressed())
			;

		// const char **words;
		// if(rude)
		// 	words = rude_words;
		// else
		// 	words = nice_words;

		if(strcmp(scramble_word, chosen_word) == 0){
			sprintf(display_buffer, FSTR("    %s    "), scramble_word);
			title_prompt(display_buffer, SUCCESS_SHOW_TIMES, true, ROUND_DELAY);
		}


		// if(jackpot_words_chosen(jackpot_choice1, jackpot_choice2, jackpot_choice3)){
		// 	win = WIN_JACKPOT;
		// 	jackpot = true;
		// } else if(triple_word_chosen()){
		// 	win = WIN_TRIPLE;
		// 	if(special_word_chosen())
		// 		win *= WIN_WORD_BONUS;
		// } else if(double_word_chosen()){
		// 	win = WIN_DOUBLE;
		// 	if(special_word_chosen())
		// 		win *= WIN_WORD_BONUS;
		// } else if(choice1 < WIN_WORD_CUTOFF || choice2 < WIN_WORD_CUTOFF || choice3 < WIN_WORD_CUTOFF) {
		// 	win = WIN_WORD;
		// }

		// win *= bet_amounts[current_bet];

		// if(jackpot)
		// 	display_jackpot(win);
		// else if(win)
		// 	display_win(win);
		// else
		// 	// see the non-winning results in lieu of being told you lost
		// 	delay(ROUND_DELAY);

		// purse += win;
		// save_data();

		// display_purse();
	}

}


// pick a word from the word list
//   have an optio for which word list
// do a random number of transformations on the letters
//   make sure the next one doesn't undo the previous one
// shift left by one, shift right by one, flip
//   ensure the scrambled word doesn't match the original
// flash the user a display showing which button does which
//   green=shift left, amber=flip, red=shift right
// show the scrambled word and light up all three buttons
// do the transformations to the shown scambled word
//   if it happens to match the real word, stop and score
// 	 stop and score on prompt timeout

//