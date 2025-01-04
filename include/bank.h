#ifndef __BANK_H__
#define __BANK_H__

#define HOUSE_BANK_WITHDRAWL 1000L
#define PLAYER_LOAN 500L
#define HOUSE_MINIMUM 1000L
#define PLAYER_MINIMUM 0L
#define GANG_MIMUMUM 1000L
#define BANK_ROBBERY_MINIMUM 5000
#define BANK_ROBBERY_MAXIMUM 25000

// returns amount deposited
extern long bank_deposit(long dollars);

// returns the amount withdrawn
extern long bank_widthdrawl(long dollars);

extern long bank_robbery(long min_dollars=BANK_ROBBERY_MINIMUM, long max_dollars=BANK_ROBBERY_MAXIMUM);

// returns the amount paid
extern long pay_house(long dollars);

// returns the amount paid out
extern long house_payout(long dollars);

// returns the amount used
extern long use_purse(long dollars);

// returns the amount added
extern long add_to_purse(long dollars);

extern long loan_money(long dollars);

#endif
