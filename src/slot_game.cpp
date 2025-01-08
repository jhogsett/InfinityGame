#include <Arduino.h>
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "timeouts.h"
#include "utils.h"
#include "seeding.h"
#include "slot_game.h"
#include "word_lists.h"

byte choice1, choice2, choice3;

void slots_round(bool rude){
	disp1.begin_scroll_loop(1);
	disp2.begin_scroll_loop(2);
	disp3.begin_scroll_loop(3);

	char * text;
	const char **words;
	if(rude){
		text = FSTR("    FUCK  SHIT  CUNT  COCK  PISS  TITS  FART  POOP  DICK  ANAL");
		words = rude_words;
	} else {
		text = FSTR("    WEED  VAPE  BEER  WINE  TACO  GOLD  MINT  PORK  JADE  CAKE");
		words = nice_words;
	}

	bool running1 = true;
	bool running2 = true;
	bool running3 = true;
	while(running1 || running2 || running3){
	unsigned long time = millis();

	if(running1){
		running1 = disp1.loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
		if(!running1){
			randomizer.randomize();
			choice1 = random(NUM_WORDS);
			disp1.show_string(words[choice1]);
		}
	}

	if(running2){
		running2 = disp2.loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
		if(!running2){
			randomizer.randomize();
			choice2 = random(NUM_WORDS);
			disp2.show_string(words[choice2]);
		}
	}

	if(running3){
		running3 = disp3.loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
		if(!running3){
			randomizer.randomize();
			choice3 = random(NUM_WORDS);
			disp3.show_string(words[choice3]);
		}
	}
	}
}

bool triple_word_chosen(){
	return choice1 == choice2 && choice2 == choice3;
}

bool double_word_chosen(){
	return choice1 == choice2 || choice2 == choice3 || choice1 == choice3;
}

bool special_word_chosen(){
	return choice1 < WIN_WORD_CUTOFF || choice2 < WIN_WORD_CUTOFF || choice3 < WIN_WORD_CUTOFF;
}

bool jackpot_words_chosen(byte word1, byte word2, byte word3){
	return choice1 == word1 && choice2 == word2 && choice3 == word3;
}

bool slots_game(){
	title_prompt(FSTR("Silly Slots"), TITLE_SHOW_TIMES, true);

	randomizer.randomize();
	byte jackpot_choice1 = random(NUM_WORDS);
	randomizer.randomize();
	byte jackpot_choice2 = random(NUM_WORDS);
	randomizer.randomize();
	byte jackpot_choice3 = random(NUM_WORDS);

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

	unsigned long idle_timeout = millis() + option_idle_time;
	unsigned long time;
	long last_bet_amount = 0L;

	while((time = millis()) < idle_timeout){
		bet_amounts[BET_ALL] = purse;
		bet_amounts[BET_REPEAT] = last_bet_amount;

		sprintf(display_buffer, FSTR("BET %s Back"), standard_bet_str(current_bet));
		const bool states[] = {false, true, false, false};
		int response = button_led_prompt(display_buffer, states);
		switch(response){
			case -1:
				return false;
			case 0:
				// long press
				if(validated_button_states[GREEN_ID]){
					// set the bet to repeat and go
					current_bet = BET_REPEAT;
					break;
				} else if(validated_button_states[AMBER_ID]){
					// set the bet to all and go
					current_bet = BET_ALL;
					break;
				} else if(validated_button_states[RED_ID]){
					// go back
					return false;
				}
				break;
			case 1:
				break;
			case 2:
				current_bet++;
				if(current_bet >= NUM_BET_AMOUNTS)
					current_bet = 0;

				sprintf(display_buffer, FSTR("    %s"), standard_bet_str(current_bet));
				disp2.scroll_string(display_buffer, 1, OPTION_FLIP_SCROLL_TIME);
				continue;
			case 3:
				return false;
		}

		idle_timeout = millis() + option_idle_time;

		long win = 0;
		bool jackpot = false;

		last_bet_amount = bet_amounts[current_bet];
		pay_house(use_purse(last_bet_amount));
		save_data();

		slots_round(rude);

		while(button_pressed())
			;

		const char **words;
		if(rude)
			words = rude_words;
		else
			words = nice_words;

		sprintf(display_buffer, FSTR("%s%s%s"), words[choice1], words[choice2], words[choice3]);
		title_prompt(display_buffer);

		if(jackpot_words_chosen(jackpot_choice1, jackpot_choice2, jackpot_choice3)){
			win = WIN_JACKPOT;
			jackpot = true;
		} else if(triple_word_chosen()){
			win = WIN_TRIPLE;
			if(special_word_chosen())
				win *= WIN_WORD_BONUS;
		} else if(double_word_chosen()){
			win = WIN_DOUBLE;
			if(special_word_chosen())
				win *= WIN_WORD_BONUS;
		} else if(choice1 < WIN_WORD_CUTOFF || choice2 < WIN_WORD_CUTOFF || choice3 < WIN_WORD_CUTOFF) {
			win = WIN_WORD;
		}

		win *= bet_amounts[current_bet];

		if(jackpot)
			display_jackpot(win);
		else if(win)
			display_win(win);
		else
			// see the non-winning results in lieu of being told you lost
			delay(ROUND_DELAY);

		add_to_purse(house_payout(win));
		save_data();

		display_purse();
	}

	return false;
}
