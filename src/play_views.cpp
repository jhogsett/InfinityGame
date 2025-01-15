#include <Arduino.h>
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"

void display_purse(){
	sprintf(display_buffer, FSTR("CASH $%s"), format_long(get_purse()));
	title_prompt(display_buffer, CASH_SHOW_TIMES, false, CASH_SHOW_DELAY);
}

void display_win(unsigned long win){
	sprintf(display_buffer, FSTR("*WIN $%s"), format_long(win));
	title_prompt(display_buffer, WIN_SHOW_TIMES, true, ROUND_DELAY);
}

void display_jackpot(unsigned long win){
	sprintf(display_buffer, FSTR("** JACKPOT $%s"), format_long(win));
	title_prompt(display_buffer, JACKPOT_SHOW_TIMES, true, ROUND_DELAY);
}

char *format_long(long num, long basis){
	int negate = (num < 0L) ? -1 : 1;
	if(negate < 0)
		num *= -1L;

	if(basis == 0)
		basis = MONEY_BASIS;

	// this captures and removes the lower three digits in the expanded basis of the number
	// plus removes the basis factor and 1000 for the remaining splits
	long basis_factor = 1000L / basis;
	int units = num % (basis_factor);
	num = (num - units) / (basis_factor);
	units *= basis;

	int thous = ((num % 1000L) - 0) / 1L;
	int mills = ((num % 1000000L) - thous) / 1000L;
	int bills = ((num % 1000000000L) - mills) / 1000000L;

	if(bills > 0)
		sprintf(copy_buffer, FSTR("%d,%03d,%03d,%03d"), bills*negate, mills, thous, units);
	else if(mills > 0)
		sprintf(copy_buffer, FSTR("%d,%03d,%03d"), mills*negate, thous, units);
	else if(thous > 0)
		sprintf(copy_buffer, FSTR("%d,%03d"), thous*negate, units);
	else
		sprintf(copy_buffer, FSTR("%d"), units*negate);

	return copy_buffer;
}

char *standard_bet_str(byte bet){
	if(bet == BET_ALL){
		sprintf(copy_buffer, FSTR("ALL"));
		return copy_buffer;
	} else if(bet == BET_REPEAT){
		sprintf(copy_buffer, FSTR("RPT"));
		return copy_buffer;
	} else
		return format_long(bet_amounts[bet]);
}
