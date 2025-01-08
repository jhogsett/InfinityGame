#include <Arduino.h>
#include "buffers.h"
#include "displays.h"
#include "motor.h"
#include "play_data.h"
#include "prompts.h"
#include "speaker.h"
#include "utils.h"
#include "bank.h"

bool crime_wave = false;
unsigned long crime_wave_started = 0L;

// Properties

long get_bank(){
	return bank * MONEY_BASIS;
}

long get_house(){
	return house * MONEY_BASIS;
}

long get_purse(){
	return purse * MONEY_BASIS;
}

long get_gang(){
	return gang * MONEY_BASIS;
}

// Bank operations

// returns the amount deposited
long bank_deposit(long dollars){
	bank += dollars / MONEY_BASIS;
	return dollars;
}

// returns the amount withdrawn
long bank_widthdrawl(long dollars){
	bank -= dollars / MONEY_BASIS;
	return dollars;
}

// returns the amount of the take
long bank_robbery(long min_dollars, long max_dollars){
	long take = random(min_dollars, max_dollars+1);
	return bank_widthdrawl(take);
}

// House operations

// returns the amount paid
long pay_house(long dollars){
	house += dollars / MONEY_BASIS;
	return dollars;
}

// returns the amount paid out
long house_payout(long dollars){
	house -= dollars / MONEY_BASIS;

	while(house * MONEY_BASIS < HOUSE_MINIMUM)
		house += bank_widthdrawl(HOUSE_BANK_WITHDRAWL) / MONEY_BASIS;

	return dollars;
}

// returns the amount burglarized
long burglarize_house(long min_dollars, long max_dollars){
	long take = random(min_dollars, max_dollars+1);
	return house_payout(take);
}

// Player Cash operations

// returns the amount used
long use_purse(long dollars){
	purse -= dollars / MONEY_BASIS;

	// TODO do the math in MONEY_BASIS units
	long total_loan = 0;
	while((purse * MONEY_BASIS) + total_loan < PLAYER_MINIMUM)
		total_loan += gang_payout(PLAYER_LOAN);
	purse += total_loan / MONEY_BASIS;

	// finish and reset a possible crime wave
	unsigned long crime_wave_timeout = crime_wave_started + MINIMUM_CRIME_WAVE_TIME;
	unsigned long time;
	while((time = millis()) < crime_wave_timeout)
		button_leds.step(time);
	crime_wave = false;

	if(total_loan){
		ltoa(total_loan, copy_buffer, 10);
		sprintf(display_buffer, FSTR("$%s GANG LOAN"), copy_buffer);
		title_prompt(display_buffer, 1, false, ALERT_SHOW_TIME);
	}

	return dollars;
}

// // returns the amount burglarized
// long scam_purse(long min_dollars, long max_dollars){
// 	// quantize to 100s
// 	min_dollars /= 100;
// 	max_dollars /= 100;
// 	long take = random(min_dollars, max_dollars+1);
// 	min_dollars *= 100;
// 	max_dollars *= 100;

// 	return house_payout(take);
// }


// returns the amount added
long add_to_purse(long dollars){
	purse += dollars / MONEY_BASIS;
	return dollars;
}

// Gang operations

// returns the amount paid out
long gang_payout(long dollars){
	gang -= dollars / MONEY_BASIS;

	if(gang * MONEY_BASIS < GANG_MIMUMUM){
		if(!crime_wave){
			unsigned long time = millis();
			crime_wave = true;
			crime_wave_started = time;
			load_f_string(F("*Crime Wave*"), display_buffer);
			display.simple_show_string(display_buffer);
			bool led_enables[] = {true, false, true};
			button_leds.begin(time, ALERT_LEDS_STYLE, ALERT_LEDS_SHOW_TIME, ALERT_LEDS_BLANK_TIME, led_enables);
		}

		long total_take = 0;
		while((gang * MONEY_BASIS) + total_take < GANG_MIMUMUM){
			total_take += steal_money();
		}
		unsigned long time = millis();
		button_leds.step(time);
		gang += total_take / MONEY_BASIS;
	}

	return dollars;
}

long steal_money(){
	long take = 0L;

	bool steal_from_bank = false;
	bool steal_from_house = false;
	// bool steal_from_purse = false;

	if(bank * MONEY_BASIS >= BANK_STEAL_MIN)
		steal_from_bank = true;

	if(house * MONEY_BASIS >= HOUSE_STEAL_MIN)
		steal_from_house = true;

	// if(purse * MONEY_BASIS >= PURSE_STEAL_MIN)
	// 	steal_from_purse = true;

	switch(random(2)){
		case 0:
			if(steal_from_bank)
				take = bank_robbery();
			break;
		case 1:
			if(steal_from_house)
				take = burglarize_house();
			break;
		// case 2:
		// 	if(steal_from_purse)
		// 		take = scam_purse();
		// 	break;
	}

	return take;
}
