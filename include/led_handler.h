#ifndef __LED_HANDLER_H__
#define __LED_HANDLER_H__

#include <Arduino.h>

// Handles activation and animation of panel and button LEDs
class LEDHandler
{
public:
	explicit LEDHandler(int first_pin, int num_leds, const int *intensity, int show_time=0, int blank_time=0);
	void begin(unsigned long time, int style, int show_time=0, int blank_time=0, bool *enabled= NULL);
	void step(unsigned long time);

	void activate_leds(const volatile bool * states, bool mirror=false);
	void activate_all(bool state, bool mirror=false);
	void deactivate_leds(bool mirror=false);
	void flash_leds(bool mirror=false, long time=0);

	void begin_flash(bool mirror, int on_time);
	bool step_flash(unsigned long time);

	static const int STYLE_PLAIN    = 0x00; // one LED at a time sequentially
	static const int STYLE_RANDOM   = 0x01; // one LED at a time randomly
	static const int STYLE_BLANKING = 0x02; // blanking period between LED activations
	static const int STYLE_MIRROR   = 0x04; // same output for panel and button LEDs
	// for STYLE_MIRROR, the secondary LEDs mirroring the first are expected to be in the upper half of the set of LED pins
	// static const int STYLE_STALLING = 0x08; // blanking period between rounds
    // TODO add a style for all activated dur the show period
    // TODO add a style for make before break showing style

	static const int DEFAULT_SHOW_TIME  = 250;
	static const int DEFAULT_BLANK_TIME = 250;
	static const int DEFAULT_FLASH_TIME = 100;

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
	int _active;			// virtual current active led or other state
	bool *_enabled;			// array of enabled leds for animation matching LED count

	bool _flash_mirror;
	int _flash_on_time;
	bool _flash_state;
	int _flash_pins;
	unsigned long _next_flash_change;
};

// used to manage non-block LED flash
// TODO move into the class
#define FLASH_STATE_START 0
#define FLASH_STATE_RUN   1
#define FLASH_STATE_DONE  2

#endif
