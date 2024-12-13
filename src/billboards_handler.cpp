#include <Arduino.h>
#include "billboard.h"
#include "billboards_handler.h"

BillboardsHandler::BillboardsHandler(char * buffer, int num_templates, const char *const *templates, int blanking_time, int home_times, bool random, int show_delay, int frame_delay){
	_buffer = buffer;
	_num_billboards = num_templates;
	_billboard = new Billboard(buffer, show_delay, frame_delay);
	_templates = templates;
	_blanking_time = blanking_time;
	_home_times = home_times;
	_random = random;

	reset();
}

void BillboardsHandler::reset(){
	_running = false;
	_n_current_template = 0;
	_n_next_template = _home_times == 0 ? 0 : 1;
	_showing_home = false;
	_blanking = true;
	_blanking_until = 0;;
}

void BillboardsHandler::update_buffer(char *string) {
	sprintf_P(_buffer, (char *)pgm_read_ptr(&(_templates[_n_current_template])), string);
}

void BillboardsHandler::run(unsigned long time, HT16K33Disp * display, char *string){
	if(handle_blanking(display, time))
		return;
	if(terminate_blanking(time))
		return;
	process_display(display, string);
	_running = _billboard->step(display, time);
}

// start blanking period when the current billboard stops running
bool BillboardsHandler::handle_blanking(HT16K33Disp * display, unsigned long time){
	if (!_running && !_blanking) {
		display->clear();
		_blanking = true;
		_blanking_until = time + _blanking_time;
		return true;
	}
	return false;
}

// end blanking period
bool BillboardsHandler::terminate_blanking(unsigned long time){
	if(_blanking) {
		if (time > _blanking_until)
			_blanking = false;
		else
			return true;
	}
	return false;
}

// start next billboard, alternating home (first billboard) and random other billboards
void BillboardsHandler::process_display(HT16K33Disp * display, char *string){
	if (!_running) {
		process_billboard_switch();
		update_buffer(string);
		_billboard->begin(display, run_times());
	}
}

void BillboardsHandler::process_billboard_switch(){
	if(_home_times){
		if (!_showing_home) {
			_showing_home = true;
			_n_current_template = 0;
		} else {
			_showing_home = false;
			if(_random)
				_n_current_template = random(1, _num_billboards);
			else {
				_n_current_template = _n_next_template;
				_n_next_template++;
				if(_n_next_template >= _num_billboards)
					_n_next_template = 1;
				}
		}
	} else {
		if(_random)
			_n_current_template = random(0, _num_billboards);
		else {
			_n_current_template = _n_next_template;
			_n_next_template++;
			if(_n_next_template >= _num_billboards)
				_n_next_template = 0;
		}
	}
}

byte BillboardsHandler::run_times(){
	byte times;
	if(_showing_home)
		times = _home_times;
	else
		times = 1;
	return times;
}
