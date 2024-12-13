#ifndef __BILLBOARDS_HANDLER_H__
#define __BILLBOARDS_HANDLER_H__
#include <HT16K33Disp.h>
#include "billboard.h"

class BillboardsHandler
{
public:
	explicit BillboardsHandler(char * buffer, int num_templates, const char *const *templates, int blanking_time, int home_times, bool random=true, int show_delay=0, int frame_delay=0);
	void update_buffer(char *string);
	void run(unsigned long time, HT16K33Disp * display, char *string);
	void reset();
	bool handle_blanking(HT16K33Disp * display, unsigned long time);
	bool terminate_blanking(unsigned long time);
	void process_display(HT16K33Disp * display, char *string);
	void process_billboard_switch();
	byte run_times();

private:
	char *_buffer;
	int _num_billboards;
	Billboard *_billboard;
	const char * const* _templates;
	int _blanking_time;
	byte _home_times;
	bool _random;

	bool _running;
	byte _n_current_template;
	byte _n_next_template;
	bool _showing_home;
	bool _blanking;
	unsigned long _blanking_until;
};

#endif
