#include "clock.h"

bool clock_mode(){
	return clock_prompt(0, 0, 12);
}
