#ifndef __BUTTON_HANDLER_H__
#define __BUTTON_HANDLER_H__

#define ANY_COLOR_ID 0
#define FIRST_COLOR_ID 1
#define LAST_COLOR_ID 3
#define NUM_COLOR_IDS 3
#define GREEN_ID 1
#define AMBER_ID 2
#define RED_ID 3
#define NO_COLOR_ID -1

#define DEBOUNCE_TIME 5
#define LONG_PRESS_TIME 1500

extern volatile bool button_states[];
extern volatile unsigned long press_time;

extern void button_pressed_i();
extern bool button_pressed();
extern bool button_still_pressed();
extern int wait_on_long_press();

#endif
