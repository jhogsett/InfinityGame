#include "bank.h"
#include "play_data.h"

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
	int take = random(min_dollars, max_dollars+1);
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

// returns the amount used
long use_purse(long dollars){
	purse -= dollars;

	while(purse < PLAYER_MINIMUM)
		purse += loan_money(PLAYER_LOAN);

	return dollars;
}

// returns the amount added
long add_to_purse(long dollars){
	purse += dollars;
	return dollars;
}

long loan_money(long dollars){
	gang -= dollars;

	while(gang < GANG_MIMUMUM)
		gang += bank_robbery();

	return dollars;
}
