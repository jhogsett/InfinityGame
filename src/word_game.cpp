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
	// char carry = buffer[0];
	// buffer[0] = buffer[3];
	// buffer[3] = carry;
	// carry = buffer[1];
	// buffer[1] = buffer[2];
	// buffer[2] = carry;
	int half_size = size / 2;
	for(int i = 0; i < half_size; i++){
		char carry = buffer[i];
		buffer[i] = buffer[(size-1) -i];
		buffer[(size-1) -i] = carry;
	}
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
				rotate_left(scramble_word, WORD_WIDTH);
				break;
			case 2:
				flip(scramble_word, WORD_WIDTH);
				break;
			case 3:
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


// sprintf(display_buffer, FSTR("<-- %s -->"), scramble_word);
// title_prompt(display_buffer);

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