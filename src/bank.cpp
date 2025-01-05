#include <Arduino.h>
#include "buffers.h"
#include "play_data.h"
#include "prompts.h"
#include "utils.h"
#include "bank.h"

// returns the amount deposited
long bank_deposit(long dollars){
	bank += dollars;
	return dollars;
}

// returns the amount withdrawn
long bank_widthdrawl(long dollars){
	bank -= dollars;
	return dollars;
}

// returns the amount of the take
long bank_robbery(long min_dollars, long max_dollars){
	long take = random(min_dollars, max_dollars+1);

	char buf[10];
	ltoa(take, buf, 10);
	sprintf(display_buffer, FSTR("BANK Vault Blasted $%s Taken"), buf);
	title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

	return bank_widthdrawl(take);
}

// returns the amount paid
long pay_house(long dollars){
	house += dollars;
	return dollars;
}

// returns the amount paid out
long house_payout(long dollars){
	house -= dollars;

	while(house < HOUSE_MINIMUM)
		house += bank_widthdrawl(HOUSE_BANK_WITHDRAWL);

	return dollars;
}

// returns the amount burglarized
long burglarize_house(long min_dollars, long max_dollars){
	long take = random(min_dollars, max_dollars+1);

	char buf[10];
	ltoa(take, buf, 10);
	sprintf(display_buffer, FSTR("HOUSE Safe CRACKED $%s Taken"), buf);
	title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

	return house_payout(take);
}

// returns the amount used
long use_purse(long dollars){
	purse -= dollars;

	long total_loan = 0;
	while(purse + total_loan < PLAYER_MINIMUM)
		total_loan += borrow_money(PLAYER_LOAN);
	purse += total_loan;

	if(total_loan){
		char buf[10];
		ltoa(total_loan, buf, 10);
		sprintf(display_buffer, FSTR("Loan from GANG $%s"), buf);
		title_prompt(display_buffer, 1, false, ALERT_SHOW_TIME);
	}

	return dollars;
}

// returns the amount burglarized
long scam_purse(long min_dollars, long max_dollars){
	// quantize to 100s
	min_dollars /= 100;
	max_dollars /= 100;
	long take = random(min_dollars, max_dollars+1);
	min_dollars *= 100;
	max_dollars *= 100;

	char buf[10];
	ltoa(take, buf, 10);
	sprintf(display_buffer, FSTR("Player SCAMMED $%s Taken"), buf);
	title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

	return house_payout(take);
}


// returns the amount added
long add_to_purse(long dollars){
	purse += dollars;
	return dollars;
}

long borrow_money(long dollars){
	gang -= dollars;

	long total_take = 0;
	while(gang + total_take < GANG_MIMUMUM)
		total_take += steal_money();
	gang += total_take;

	// char buf[10];
	// ltoa(total_loan, buf, 10);
	// sprintf(display_buffer, FSTR("Loan from GANG $%s"), buf);
	// title_prompt(display_buffer, 1, false, ALERT_SHOW_TIME);

	return dollars;
}


long steal_money(){
	bool steal_from_bank = false;
	bool steal_from_house = false;
	bool steal_from_purse = false;

	long take = 0L;

	if(bank >= BANK_STEAL_MIN)
		steal_from_bank = true;

	if(house >= HOUSE_STEAL_MIN)
		steal_from_house = true;

	if(purse >= PURSE_STEAL_MIN)
		steal_from_purse = true;

	while(take == 0L){
		switch(random(2)){
			case 0:
				if(steal_from_bank)
					take = bank_robbery();
				break;
			case 1:
				if(steal_from_house)
					take = burglarize_house();
				break;
			case 2:
				if(steal_from_purse)
					take = scam_purse();
				break;
		}
	}

	return take;
}
