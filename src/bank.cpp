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

// returns the amount paid
long pay_house(long dollars){
	house += dollars;
	return dollars;
}

// returns the amount paid out
long house_payout(long dollars){
	if(house <= 0L){
		// take money from the bank
		house += bank_widthdrawl(dollars - house);
	}

	house -= dollars;
	return dollars;
}

// returns the amount used
long use_purse(long dollars){
	if(purse <= 0L){
		// take money from the bank
		purse += bank_widthdrawl(dollars - purse);
	}

	purse -= dollars;
	return dollars;
}

// returns the amount added
long add_to_purse(long dollars){
	purse += dollars;
	return dollars;
}
