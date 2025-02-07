#ifndef __BANK_H__
#define __BANK_H__

#define MONEY_BASIS 50L
#define HOUSE_BANK_WITHDRAWL (100000L / MONEY_BASIS)
#define PLAYER_LOAN (2000L / MONEY_BASIS)
#define BANK_MINIMUM (1000000000L / MONEY_BASIS)
#define HOUSE_MINIMUM (100000L / MONEY_BASIS)
#define PLAYER_MINIMUM (0L / MONEY_BASIS)
#define GANG_MIMUMUM (100000L / MONEY_BASIS)
#define BANK_ROBBERY_MINIMUM (25000L / MONEY_BASIS)
#define BANK_ROBBERY_MAXIMUM (250000L / MONEY_BASIS)
#define HOUSE_BURGLARY_MINIMUM (10000L / MONEY_BASIS)
#define HOUSE_BURGLARY_MAXIMUM (100000L / MONEY_BASIS)
#define PURSE_SCAM_MINIMUM (10000L / MONEY_BASIS)
#define PURSE_SCAM_MAXIMUM (25000L / MONEY_BASIS)
#define BANK_STEAL_MIN (100000L / MONEY_BASIS)
#define HOUSE_STEAL_MIN (100000L / MONEY_BASIS)
#define PURSE_STEAL_MIN (25000L / MONEY_BASIS)

// for self-doc but code only works if this is set to 2
#define VIG_DIVISOR 2L

#define ALERT_SHOW_TIME 1000
#define MINIMUM_CRIME_WAVE_TIME 2000

#ifdef SHOW_BANK_FLASHES
#define TRANSACTION_FLASH_TIME 1
#endif

extern bool crime_wave;
extern unsigned long crime_wave_started;

extern long get_sum();
extern long get_bank();
extern long get_house();
extern long get_purse();
extern long get_gang();
extern long get_vig();

// returns amount deposited
extern long bank_deposit(long money);

// returns the amount withdrawn
extern long bank_widthdrawl(long money);

extern long bank_robbery(long min_money=BANK_ROBBERY_MINIMUM, long max_dollars=BANK_ROBBERY_MAXIMUM);

extern bool reset_bank();

// returns the amount paid
extern long pay_house(long dollars);

// returns the amount paid out
extern long house_payout(long dollars);

extern long burglarize_house(long min_dollars=HOUSE_BURGLARY_MINIMUM, long max_dollars=HOUSE_BURGLARY_MAXIMUM);

// returns the amount used
extern long use_purse(long dollars);

// returns the amount added
extern long add_to_purse(long dollars);

// extern long scam_purse(long min_dollars=PURSE_SCAM_MINIMUM, long max_dollars=PURSE_SCAM_MAXIMUM);

extern long gang_loan(long dollars);

extern long steal_money();

#endif
