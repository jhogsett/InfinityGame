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

	// while(bank < BANK_MINIMUM)
	// 	bank += recover_money();

	return dollars;
}

// returns the amount of the take
long bank_robbery(long min_dollars, long max_dollars){
	long take = random(min_dollars, max_dollars+1);

	// char buf[15];
	// ltoa(take, buf, 10);
	// sprintf(display_buffer, FSTR("BANK VAULT BLASTED for $%s"), buf);
	// title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

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

	// char buf[15];
	// ltoa(take, buf, 10);
	// sprintf(display_buffer, FSTR("HOUSE SAFE CRACKED for $%s"), buf);
	// title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

	return house_payout(take);
}

// // seizes the entire amount, returned
// long bust_house(){
// 	long take = house;

// 	// char buf[15];
// 	// ltoa(take, buf, 10);
// 	// sprintf(display_buffer, FSTR("HOUSE BUSTED all $%s SEIZED"), buf);
// 	// title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

// 	return house_payout(take);
// }

// returns the amount used
long use_purse(long dollars){
	purse -= dollars;

	long total_loan = 0;
	while(purse + total_loan < PLAYER_MINIMUM)
		total_loan += gang_payout(PLAYER_LOAN);
	purse += total_loan;

	if(total_loan){
		char buf[15];
		ltoa(total_loan, buf, 10);
		sprintf(display_buffer, FSTR("GANG LOAN for $%s"), buf);
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

	// char buf[15];
	// ltoa(take, buf, 10);
	// sprintf(display_buffer, FSTR("PLAYER SCAMMED for $%s"), buf);
	// title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

	return house_payout(take);
}


// returns the amount added
long add_to_purse(long dollars){
	purse += dollars;
	return dollars;
}

// returns the amount paid out
long gang_payout(long dollars){
	gang -= dollars;

	long total_take = 0;
	while(gang + total_take < GANG_MIMUMUM)
		total_take += steal_money();
	gang += total_take;

	return dollars;
}



// // seizes the entire amount, returned
// long bust_gang(){
// 	long take = gang;

// 	// char buf[15];
// 	// ltoa(take, buf, 10);
// 	// sprintf(display_buffer, FSTR("GANG BUSTED $%s SEIZED"), buf);
// 	// title_prompt(display_buffer, 1, true, ALERT_SHOW_TIME);

// 	return gang_payout(take);
// }


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
		switch(random(3)){
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

// long recover_money(){
// 	bool recover_from_house = false;
// 	bool recover_from_gang = false;

// 	long take = 0L;

// 	if(house > 0L)
// 		recover_from_house = true;

// 	if(gang >= 0L)
// 		recover_from_gang = true;

// 	while(take == 0L){
// 		switch(random(2)){
// 			case 0:
// 				if(recover_from_gang)
// 					take = bust_gang();
// 				break;
// 			case 1:
// 				if(recover_from_house)
// 					take = bust_house();
// 				break;
// 		}
// 	}

// 	return take;
// }
