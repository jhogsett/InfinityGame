#include <Arduino.h>
#include "HT16K33Disp.h"

// some code borrowed from https://github.com/akuzechie/HT16K33-Display-Library

HT16K33Disp::HT16K33Disp(byte address, byte num_displays){
	set_address(address, num_displays);
	_loop_running = false;
}

void HT16K33Disp::set_address(byte address, byte num_displays){
	_address = address;
	_num_displays = num_displays;
	_num_digits = _num_displays * NUM_DIGITS_PER_DISPLAY;
}

// point to an array of ints specifying brightness levels per display
void HT16K33Disp::init(const byte *brightLevels){
	for(byte i = 0; i < _num_displays; i++){
		Wire.beginTransmission(_address + i);
		Wire.write(0x21); //normal operation mode
		Wire.endTransmission(false);
		Wire.beginTransmission(_address + i);
		Wire.write(0xE0 + *(brightLevels + i));
		Wire.endTransmission(false);
		Wire.beginTransmission(_address + i);
		Wire.write(0x81); //display ON, blinking OFF
		Wire.endTransmission();
		clear();
	}
}



void HT16K33Disp::write(byte digit, unsigned int data){
	int display = digit / NUM_DIGITS_PER_DISPLAY;
	digit -= (display * NUM_DIGITS_PER_DISPLAY);
	Wire.beginTransmission(_address + display);
	Wire.write(digit*2);
	Wire.write(data);
	Wire.write(data >> 8);
	Wire.endTransmission();
}

void HT16K33Disp::segments_test(){
	for(byte i = 0; i < _num_digits; i++)
		write(i, (uint16_t) -1);
}

void HT16K33Disp::clear(){
	for(byte i = 0; i < _num_digits; i++)
		write(i, 0);
}

// determine the displayable length of the string
// taking decimal points into account
int HT16K33Disp::string_length(const char * string){
	int count = 0;
	int num_chars = strlen(string);
	for(int i = 0; i <= num_chars; i++){
		if(*string == 0)
		    break;
		// period chars won't take up a digit position when displayed

		if(*(string + 1) == '.')
		    string++;
		string++;
		count++;
	}
	return count;
}

void HT16K33Disp::show_string(const char * string, bool pad_blanks, bool right_justify){
	byte i = 0;
	if(right_justify)
	{
		char diff = _num_digits - string_length(string);
		if(pad_blanks)
		{
		    if(diff > 0){

		for(i = 0; i < diff; i++)

		    write(i, char_to_segments(' '));
		    }
		}
		else

		    i = diff;
	}

	for(byte j = i; j < _num_digits; j++)
	{
		if(*string == 0)
		{
		    if(pad_blanks && !right_justify)

		write(j, char_to_segments(' '));
		    else

		break;
		}
		else

		{
		    if(*(string + 1) == '.')
		    {

		// take the next char and just light this positions DP LED

		write(j, char_to_segments(*string, true));

		string++;
		    }
		    else

		write(j, char_to_segments(*string));
		    string++;
		}
	}
}

void HT16K33Disp::simple_show_string(const char * string){
	for(byte i = 0; i < _num_digits; i++){
		if(*string == 0)
		    break;
		if(*(string + 1) == '.')
		{
		    write(i, char_to_segments(*string, true));
		    string++;
		}
		else

		    write(i, char_to_segments(*string));
		string++;
	}
}

// save and restore string in case this is used along with a non-blocking scroll
void HT16K33Disp::scroll_string(const char * string, int show_delay, int scroll_delay){
	const char *old_string = _string;
	begin_scroll_string(string, show_delay, scroll_delay);
	while(step_scroll_string(millis()));
	_string = old_string;
}

// returns count of frames
int HT16K33Disp::begin_scroll_string(const char * string, int show_delay, int scroll_delay){
	_string = string;
	_scrollpos = 0;
	_show_delay = show_delay ? show_delay : DEFAULT_SHOW_DELAY;
	_scroll_delay = scroll_delay ? scroll_delay : DEFAULT_SCROLL_DELAY;
	int length = string_length(string);
	_frames = (length - _num_digits) + 1;
	if(_frames < 1)
		_frames = 1;
	_frame = 0;
	_next_frame = 0;
	_short_string = length <= _num_digits;
	return _frames;
}

bool HT16K33Disp::step_scroll_string(unsigned long time){
	if(time >= _next_frame)
	{
		if(_short_string)
		    show_string(_string, true);
		else

		{
		    simple_show_string(_string + _scrollpos);
		    if(_frame < _frames - 1){

		_scrollpos++;
		if(*(_string + _scrollpos) == '.')

		    _scrollpos++;
		    }
		}
		int del = (_frame == 0) || (_frame == _frames - 1) ? _show_delay : _scroll_delay;
		_next_frame = time + del;
		if(_frame < _frames)
		{
		    _frame++;
		    return true;
		} else

		    return false;

	}
	else
    {
		return true;
	}
}

// -1=loop forever
void HT16K33Disp::begin_scroll_loop(int times){
	_loop_running = false;
	_loop_times = times;
}

// returns true if there's more loops to go
bool HT16K33Disp::loop_scroll_string(unsigned long time, const char * string, int show_delay, int scroll_delay){
	if(!_loop_running)
	{
		if(_loop_times == 0)
		    return false;
		if(_loop_times == -1 || _loop_times > 0)
		    begin_scroll_string(string, show_delay, scroll_delay);
		if(_loop_times > 0)
		    _loop_times--;
	}
	_loop_running = step_scroll_string(time);
	return true;
}

uint16_t HT16K33Disp::char_to_segments(char c, bool decimal_point){
	if(c < 32 || c > 127)
		return (uint16_t) -1;
#ifdef HT16K33Disp_USEPROGMEM
    return pgm_read_dword(&HT16K33Disp_FourteenSegmentASCII[c - 32]) | (decimal_point ? DECIMAL_PT_SEGMENT : 0);
#else
    return HT16K33Disp_FourteenSegmentASCII[c - 32] | (decimal_point ? DECIMAL_PT_SEGMENT : 0);
#endif
}
