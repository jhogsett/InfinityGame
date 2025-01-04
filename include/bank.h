#ifndef __BANK_H__
#define __BANK_H__

// returns amount deposited
extern long bank_deposit(long dollars);

// returns the amount withdrawn
extern long bank_widthdrawl(long dollars);

// returns the amount paid
extern long pay_house(long dollars);

// returns the amount paid out
extern long house_payout(long dollars);

// returns the amount used
extern long use_purse(long dollars);

// returns the amount added
extern long add_to_purse(long dollars);

#endif
