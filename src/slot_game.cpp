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

bool run_slot_reel(HT16K33Disp * disp, unsigned long time, char * text, const char **words, byte &choice){
	bool running = disp->loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
	if(!running){
		randomizer.randomize();
		choice = random(NUM_WORDS);
		disp->show_string(words[choice]);
	}
	return running;
}

void slots_round(char * text, const char **words){
	disp1.begin_scroll_loop(1);
	disp2.begin_scroll_loop(2);
	disp3.begin_scroll_loop(3);

	bool running1 = true;
	bool running2 = true;
	bool running3 = true;
	while(running1 || running2 || running3){
		unsigned long time = millis();

		if(running1){
			running1 = run_slot_reel(&disp1, time, text, words, choice1);
		}

		if(running2){
			running2 = run_slot_reel(&disp2, time, text, words, choice2);
		}

		if(running3){
			running3 = run_slot_reel(&disp3, time, text, words, choice3);
		}
	}
}

bool triple_word_chosen(){
	return choice1 == choice2 && choice2 == choice3;
}

bool double_word_chosen(){
	return choice1 == choice2 || choice2 == choice3 || choice1 == choice3;
}

bool bonus_word_chosen(byte bonus_word){
	return (choice1 == bonus_word) || (choice2 == bonus_word) || (choice3 == bonus_word);
}

bool jackpot_words_chosen(byte word1, byte word2, byte word3){
	return choice1 == word1 && choice2 == word2 && choice3 == word3;
}

bool slots_game(){
	if(title_prompt(FSTR("Silly Slots"), TITLE_SHOW_TIMES, true))
        return false;

	randomizer.randomize();
	byte bonus_word = random(NUM_WORDS);
	randomizer.randomize();
	byte jackpot_choice1 = random(NUM_WORDS);
	randomizer.randomize();
	byte jackpot_choice2 = random(NUM_WORDS);
	randomizer.randomize();
	byte jackpot_choice3 = random(NUM_WORDS);

    bool rude = false;
	switch(prompt_nice_or_rude()){
        case -1:
            return false;
        case 1:
            rude = true;
    }

    const char **words = rude ? rude_words : nice_words;

#ifndef USE_ALL_WORDS
    int reel_words[REEL_WORDS];
    random_unique(REEL_WORDS, NUM_WORDS, reel_words);

    char text[REEL_BUFFER_LEN];
	sprintf_P(text,
            PSTR("    %s  %s  %s  %s  %s  %s  %s  %s  %s  %s"),
			words[reel_words[0]],
			words[reel_words[1]],
			words[reel_words[2]],
			words[reel_words[3]],
			words[reel_words[4]],
			words[reel_words[5]],
			words[reel_words[6]],
			words[reel_words[7]],
			words[reel_words[8]],
			words[reel_words[9]]);

#else
    char text[REEL_BUFFER_LEN];
	sprintf_P(text,
            PSTR("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s "),
			words[0],
			words[1],
			words[2],
			words[3],
			words[4],
			words[5],
			words[6],
			words[7],
			words[8],
			words[9],
			words[10],
			words[11],
			words[12],
			words[13],
			words[14]);
#endif

    title_prompt(" Bonus Word", 1, false, ROUND_DELAY);
    sprintf_P(display_buffer, PSTR("%s%s%s"), words[bonus_word], words[bonus_word], words[bonus_word]);
    title_prompt(display_buffer, 1, true, BONUS_SHOW_TIME);
    display.clear();
    delay(ROUND_DELAY);

	unsigned long idle_timeout = millis() + option_idle_time;
	unsigned long time;
	long last_bet_amount = 0L;

    bool auto_play = false;
	while((time = millis()) < idle_timeout){
		bet_amounts[BET_ALL] = get_purse();
		bet_amounts[BET_REPEAT] = last_bet_amount;

        if(auto_play){
            display.clear();
            delay(AUTOPLAY_BLANK_TIME);
            current_bet = (bet_amounts[BET_ALL] > bet_amounts[BET_REPEAT]) ? BET_ALL : BET_REPEAT;
            sprintf_P(display_buffer, PSTR("AUTO BET $%s"), format_long(bet_amounts[current_bet]));
            if(title_prompt(display_buffer, 1, true, AUTOPLAY_SHOW_TIME, LEDHandler::STYLE_RANDOM, AUTOPLAY_LEDS_TIME)){
                return false;
            }
        } else {
            sprintf_P(display_buffer, PSTR("BET %s Back"), standard_bet_str(current_bet));
            const bool states[] = {false, true, false, false};
            int response = button_led_prompt(display_buffer, states, true);
            switch(response){
                case -1:
                    // timeout
                    return false;
                case 0:
                    // long press
                    if(validated_button_states[GREEN_ID]){
                        // set the bet to repeat and go
                        current_bet = BET_REPEAT;
                        // break;
                    } else if(validated_button_states[AMBER_ID]){
                        // set the bet to all and go
                        current_bet = BET_ALL;
                        // break;
                    } else if(validated_button_states[RED_ID]){
                        if(auto_play_enabled){
                            auto_play = true;
                            current_bet = BET_REPEAT;
                        } else {
                            // go back
                            return false;
                        }
                    }
                    break;
                case 1:
                    break;
                case 2:
                    current_bet++;
                    if(current_bet >= NUM_BET_AMOUNTS)
                        current_bet = 0;

                    sprintf_P(display_buffer, PSTR("    %s"), standard_bet_str(current_bet));
                    disp2.scroll_string(display_buffer, 1, OPTION_FLIP_SCROLL_TIME);
                    continue;
                case 3:
                    return false;
            }
        }

		idle_timeout = millis() + option_idle_time;

		bool jackpot = false;

		last_bet_amount = bet_amounts[current_bet];
		pay_house(use_purse(last_bet_amount));
		save_data();

		slots_round(text, words);

		while(button_pressed())
			;

        title_prompt_string3(FSTR("%s%s%s"), words[choice1], words[choice2], words[choice3]);

		long win_factor = 0;
        bool word_bonus = bonus_word_chosen(bonus_word);
		if(jackpot_words_chosen(jackpot_choice1, jackpot_choice2, jackpot_choice3)){
			win_factor = WIN_JACKPOT;
			jackpot = true;
		} else if(triple_word_chosen()){
			win_factor = WIN_TRIPLE;
			if(word_bonus)
				win_factor *= WIN_WORD_BONUS;
		} else if(double_word_chosen()){
			win_factor = WIN_DOUBLE;
			if(word_bonus)
				win_factor *= WIN_WORD_BONUS;
		} else if(word_bonus) {
			win_factor = WIN_WORD;
		}

		long win = bet_amounts[current_bet] * win_factor;

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
