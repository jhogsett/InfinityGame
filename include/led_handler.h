#ifndef __LED_HANDLER_H__
#define __LED_HANDLER_H__

// for STYLE_MIRROR, the secondary LEDs mirroring the first are expected to be in the upper half of the set of LED pins

class LEDHandler
{
public:
	explicit LEDHandler(int first_pin, int num_leds, const int *intensity, int show_time=0, int blank_time=0);
	void begin(unsigned long time, int style, int show_time=0, int blank_time=0);
	void step(unsigned long time);

	void activate_leds(const volatile bool * states, bool mirror=false);
	void activate_all(bool state, bool mirror=false);
	void deactivate_leds(bool mirror=false);
	void flash_leds(bool mirror=false, long time=0);

	static const int STYLE_PLAIN    = 0x00; // one LED at a time sequentially
	static const int STYLE_RANDOM   = 0x01; // one LED at a time randomly
	static const int STYLE_BLANKING = 0x02; // blanking period between LED activations
	static const int STYLE_MIRROR   = 0x04; // same output for panel and button LEDs
	// static const int STYLE_STALLING = 0x08; // blanking period between rounds

	static const int DEFAULT_SHOW_TIME  = 250;
	static const int DEFAULT_BLANK_TIME = 250;
	static const int DEFAULT_FLASH_TIME = 50;

private:
	void deactivate_led(int virtual_pin, bool mirror=false);
	void activate_led(int virtual_pin, bool mirror=false);

	int _first_pin;
	int _num_leds;
	const int *_intensity;  // array of ints matching led count, 0 for digitalWrite, 1-255 for analogWrite
	int _show_time;
	int _blank_time;
	int _style;

	char _frame;
	unsigned long _next_frame;
	int _num_frames;
	int _num_states;
	bool _blanking;
	char _active;
		   // virtual current active led or other state

};

#endif
