#ifndef __BUTTON_HANDLER_H__
#define __BUTTON_HANDLER_H__

// Each button, including the 'any' button, have an ID, which is sometimes used as an index to an array of button states
#define NUM_COLOR_IDS 3
#define ANY_COLOR_ID 0
#define FIRST_COLOR_ID 1
#define LAST_COLOR_ID 3
#define GREEN_ID 1
#define AMBER_ID 2
#define RED_ID 3
#define NO_COLOR_ID -1

// Time in milliseconds to check for and possibly reject a noisy button activation
#define DEBOUNCE_TIME 5

// Time in milliseconds for a button to be pressed for it to be a long press and active alternate actions
#define LONG_PRESS_TIME 1500

// Max button LOWs during the debounce period reads on a button in a tight loop (experimental)
// if the supposedly-pressed button is actually low this many times, the press is rejected as unreliable
#define MAX_DROPS 2000

// An array used by the button ISR to capture the states of the 'any' button and the three real buttons
// this array type is used in various places to determine or set the buttons states
// The first array position represents the 'any' button
// When using it to set display states for the button LEDs, the 'any' element must be present, but is unused
extern volatile bool button_states[];

// saves the button state that was validated by the debounce filter an unaltered since
extern bool validated_button_states[];

// the time using millis() that a button was pressed, used by the ISR
extern volatile unsigned long press_time;

extern void button_pressed_i();
extern bool button_pressed();
extern bool button_still_pressed();
extern int wait_on_long_press();
extern void reset_buttons_state();

#endif
