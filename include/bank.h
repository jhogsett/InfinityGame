#ifndef __BANK_H__
#define __BANK_H__

#define HOUSE_BANK_WITHDRAWL 1000L
#define PLAYER_LOAN 1000L
#define BANK_MINIMUM 1000000L
#define HOUSE_MINIMUM 1000L
#define PLAYER_MINIMUM 0L
#define GANG_MIMUMUM 1000L
#define BANK_ROBBERY_MINIMUM 10000L
#define BANK_ROBBERY_MAXIMUM 500000L
#define HOUSE_BURGLARY_MINIMUM 5000L
#define HOUSE_BURGLARY_MAXIMUM 100000L
#define PURSE_SCAM_MINIMUM 10000L
#define PURSE_SCAM_MAXIMUM 25000L
#define BANK_STEAL_MIN 100000L
#define HOUSE_STEAL_MIN 100000L
#define PURSE_STEAL_MIN 25000L
#define ALERT_SHOW_TIME 1000

extern bool crime_wave;

// returns amount deposited
extern long bank_deposit(long dollars);

// returns the amount withdrawn
extern long bank_widthdrawl(long dollars);

extern long bank_robbery(long min_dollars=BANK_ROBBERY_MINIMUM, long max_dollars=BANK_ROBBERY_MAXIMUM);

// returns the amount paid
extern long pay_house(long dollars);

// returns the amount paid out
extern long house_payout(long dollars);

extern long bust_house();

extern long burglarize_house(long min_dollars=HOUSE_BURGLARY_MINIMUM, long max_dollars=HOUSE_BURGLARY_MAXIMUM);

// returns the amount used
extern long use_purse(long dollars);

// returns the amount added
extern long add_to_purse(long dollars);

extern long scam_purse(long min_dollars=PURSE_SCAM_MINIMUM, long max_dollars=PURSE_SCAM_MAXIMUM);

extern long gang_payout(long dollars);

extern long bust_gang();

extern long steal_money();

extern long recover_money();

#endif
