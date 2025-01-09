#include <Arduino.h>
#include "buffers.h"
#include "displays.h"
#include "motor.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "speaker.h"
#include "utils.h"
#include "bank.h"

bool crime_wave = false;
unsigned long crime_wave_started = 0L;

// Properties

long get_bank(){
	return bank;
}

long get_house(){
	return house;
}

long get_purse(){
	return purse;
}

long get_gang(){
	return gang;
}

// Bank operations

// returns the amount deposited
long bank_deposit(long money){
	bank += money;
	return money;
}

// returns the amount withdrawn
long bank_widthdrawl(long money){
	bank -= money;
	return money;
}

// returns the amount of the take
long bank_robbery(long min_money, long max_money){
	long take = random(min_money, max_money+1);
	return bank_widthdrawl(take);
}

// House operations

// returns the amount paid
long pay_house(long money){
	house += money;
	return money;
}

// returns the amount paid out
long house_payout(long money){
	house -= money;

	while(house < HOUSE_MINIMUM)
		house += bank_widthdrawl(HOUSE_BANK_WITHDRAWL);

	return money;
}

// returns the amount burglarized
long burglarize_house(long min_money, long max_money){
	long take = random(min_money, max_money+1);
	return house_payout(take);
}

// Player Cash operations

// returns the amount used
long use_purse(long money){
	purse -= money;

	long total_loan = 0;
	while(purse + total_loan < PLAYER_MINIMUM)
		total_loan += gang_payout(PLAYER_LOAN / MONEY_BASIS);
	purse += total_loan;

	// finish and reset a possible crime wave
	unsigned long crime_wave_timeout = crime_wave_started + MINIMUM_CRIME_WAVE_TIME;
	unsigned long time;
	while((time = millis()) < crime_wave_timeout)
		button_leds.step(time);
	crime_wave = false;

	if(total_loan){
		// ltoa(total_loan, copy_buffer, 10);
		sprintf(display_buffer, FSTR("GANG LOAN $%s"), format_long(total_loan));
		title_prompt(display_buffer, 1, false, ALERT_SHOW_TIME);
	}

	return money;
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
long add_to_purse(long money){
	purse += money;
	return money;
}

// Gang operations

// returns the amount paid out
long gang_payout(long money){
	gang -= money;

	if(gang < GANG_MIMUMUM){
		if(!crime_wave){
			unsigned long time = millis();
			crime_wave = true;
			crime_wave_started = time;
			load_f_string(F("*CRIME WAVE*"), display_buffer);
			display.simple_show_string(display_buffer);
			bool led_enables[] = {true, false, true};
			button_leds.begin(time, ALERT_LEDS_STYLE, ALERT_LEDS_SHOW_TIME, ALERT_LEDS_BLANK_TIME, led_enables);
		}

		long total_take = 0;
		while(gang + total_take < GANG_MIMUMUM){
			total_take += steal_money();
		}
		unsigned long time = millis();
		button_leds.step(time);
		gang += total_take;
	}

	return money;
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
