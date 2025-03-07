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

long get_sum(){
    return get_bank() + get_house() + get_gang() + get_purse();
}

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

long get_vig(){
	return vig;
}

#ifdef SHOW_BANK_FLASHES
void flash_led(int pin){
    digitalWrite(pin, HIGH);
    delay(TRANSACTION_FLASH_TIME);
    digitalWrite(pin, LOW);
}
#endif

// Bank operations

// returns the amount deposited
long bank_deposit(long money){
	bank += money;
#ifdef SHOW_BANK_FLASHES
    flash_led(GREEN_PANEL_LED);
#endif
	return money;
}

// returns the amount withdrawn
long bank_widthdrawl(long money){
	bank -= money;

    if(bank < BANK_MINIMUM){
        reset_bank();
    }
#ifdef SHOW_BANK_FLASHES
    flash_led(GREEN_PANEL_LED);
#endif
	return money;
}

// returns the amount of the take
long bank_robbery(long min_money, long max_money){
	long take = random(min_money, max_money+1);
	return bank_widthdrawl(take);
}

#define RESETTING_SHOW_TIMES 2
#define RESETTING_COUNTDOWN_TIME 450
#define RESET_BONUS (1000000000L / MONEY_BASIS)

bool reset_bank(){
    title_prompt(FSTR("BANK INSOLVANT. RESETTING"), RESETTING_SHOW_TIMES);

    for(int i = 10; i >= 0; i--){
        sprintf_P(display_buffer, PSTR("%7d"), i);
        beep();
        title_prompt(display_buffer, 1, false, RESETTING_COUNTDOWN_TIME);
        display.clear();
        delay(RESETTING_COUNTDOWN_TIME);
    }

	purse = DEFAULT_PURSE + RESET_BONUS;
	bank = DEFAULT_BANK - RESET_BONUS;
	house = DEFAULT_HOUSE;
	gang = DEFAULT_GANG;
    vig = DEFAULT_VIG;
    auto_play_enabled = true;

    // ##DATA Reset new persisted play data veriables to default variables here

	save_data();

	reset_device();
	return false;
}


// House operations

// returns the amount paid
long pay_house(long money){
    long deposit = money / 2L;

    // deposit half earnings to the bank
    bank_deposit(deposit);

    // keep the rest in the vault
	house += (money - deposit);

#ifdef SHOW_BANK_FLASHES
    flash_led(AMBER_PANEL_LED);
#endif
	return money;
}

// returns the amount paid out
long house_payout(long money){
	house -= money;
#ifdef SHOW_BANK_FLASHES
    flash_led(AMBER_PANEL_LED);
#endif

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
#ifdef SHOW_BANK_FLASHES
    flash_led(RED_PANEL_LED);
#endif

    if(purse < PLAYER_MINIMUM){
        title_prompt(FSTR("Finding Cash"), 1, false, FINDING_SHOW_TIME);
    }

	long total_loan = 0;
	while(purse + total_loan < PLAYER_MINIMUM)
		total_loan += gang_loan(PLAYER_LOAN);
	purse += total_loan;
    vig += total_loan + (total_loan / VIG_DIVISOR);

	// finish and reset a possible crime wave
	unsigned long crime_wave_timeout = crime_wave_started + MINIMUM_CRIME_WAVE_TIME;
	unsigned long time;
	while((time = millis()) < crime_wave_timeout)
		button_leds.step(time);
	button_leds.deactivate_leds();
	crime_wave = false;

	if(total_loan){
        title_prompt_string(FSTR("GANG LOAN $%s"), format_long(total_loan), false, LOAN_SHOW_TIME);
	}

	return money;
}

// returns the amount added
long add_to_purse(long money){
    if(vig > 0){
        long cut = money / VIG_DIVISOR;
        if(vig <= cut)
            cut = vig;
        vig -= cut;
        money -= cut;
        gang += cut;
    }
	purse += money;
#ifdef SHOW_BANK_FLASHES
    flash_led(RED_PANEL_LED);
#endif

	return money;
}

// Gang operations

// returns the amount paid out
long gang_loan(long money){
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

    gang -= money;
	return money;
}

long steal_money(){
	long take = 0L;

	bool steal_from_bank = false;
	bool steal_from_house = false;
	// bool steal_from_purse = false;

	if(bank >= BANK_STEAL_MIN)
		steal_from_bank = true;

	if(house >= HOUSE_STEAL_MIN)
		steal_from_house = true;

	// if(purse  >= PURSE_STEAL_MIN)
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
