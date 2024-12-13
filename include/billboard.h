#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__
#include <HT16K33Disp.h>

class Billboard
{
public:
	explicit Billboard(char * buffer, int show_delay=0, int frame_delay=0);
	void begin(HT16K33Disp * display, int times);
	void reset_round(HT16K33Disp * display);
	bool step(HT16K33Disp * display, unsigned long time);

	char * _buffer;
	int _show_delay;
	int _frame_delay;

private:
	int _frames;
	int _num_times;
};

#endif
