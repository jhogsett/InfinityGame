#ifndef __PROMPTS_H__
#define __PROMPTS_H__

#include "billboards_handler.h"

typedef void (*voidFuncPtr)(void);

// Billboard display related
// number of times to show home billboard "InfinityGame" (effectively time of display because no scroll needed)
#define HOME_TIMES 5
// blacking time between billboards
#define BLANKING_TIME 600

// Prompt related
// number of times to show a title, such as a game title (or effective time of display if no scroll needed)
#define TITLE_SHOW_TIMES 3

// Display handling
// show a display string for 700ms before beginning scrolling for ease of reading
#define DISPLAY_SHOW_TIME 700
// scroll the display every 90ms for ease of reading
#define DISPLAY_SCROLL_TIME 90
// scroll flipped options every 100ms
#define OPTION_FLIP_SCROLL_TIME 100

#define NUM_BILLBOARDS 8
const char template0[] PROGMEM = "InfinityGame";
const char template1[] PROGMEM = "Press A Button to Play";
const char template2[] PROGMEM = "Play Silly Slots";
const char template3[] PROGMEM = "Play The WordGame";
const char template4[] PROGMEM = "Play The TimeGame";
const char template5[] PROGMEM = "Your CASH %s";
const char template6[] PROGMEM = "BEST Time %s";
const char template7[] PROGMEM = "LONG PRESS for OPTIONS";
const char *const templates[] PROGMEM = { template0, template1, template2, template3, template4, template5, template6, template7 };

// Identify array position for placing fill-in data
#define BILLBOARD_CASH 5
#define BILLBOARD_TIME 6

extern BillboardsHandler billboards_handler;
extern char *billboard_data[NUM_BILLBOARDS];

void run_billboard(char **data);
void billboard_prompt(voidFuncPtr on_time_out, voidFuncPtr on_press, voidFuncPtr on_long_press);
int button_led_prompt(const char * prompt, const bool *states = NULL);
void title_prompt(const char * title, byte times = 1, bool show_panel_leds = false, int show_delay=0);
int panel_led_prompt();
void branch_prompt(const char * prompt, voidFuncPtr on_option_1, voidFuncPtr on_option_2, voidFuncPtr on_option_3, voidFuncPtr on_long_press = NULL, const bool *states = NULL);
int toggle_prompt(const char * prompt, const char **labels, byte current_choice, byte toggle_position, byte num_choices);

#endif
