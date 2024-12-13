#include "billboard.h"

Billboard::Billboard(char * buffer, int show_delay, int frame_delay){
	_buffer = buffer;
	_show_delay = show_delay;
	_frame_delay = frame_delay;
	_frames = 0;
	_num_times = 0;
}

void Billboard::begin(HT16K33Disp * display, int times){
	_num_times = times;
	reset_round(display);
}

void Billboard::reset_round(HT16K33Disp * display){
	_frames = display->begin_scroll_string(_buffer, _show_delay, _frame_delay);
}

bool Billboard::step(HT16K33Disp * display, unsigned long time){
	if(display->step_scroll_string(time)){
		return true;
	} else {
		_num_times -= 1;
		if(_num_times == 0)
			return false;
		else {
			reset_round(display);
			return true;
		}
	}
}
