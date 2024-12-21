#ifndef __PROMPTS_H__
#define __PROMPTS_H__

#include "billboards_handler.h"

typedef void (*voidFuncPtr)(void);

// Billboard display related
#define HOME_TIMES 5
#define BLANKING_TIME 600

// Prompt related
#define TITLE_SHOW_TIMES 3

// display handling
#define DISPLAY_SHOW_TIME 700
#define DISPLAY_SCROLL_TIME 90
#define OPTION_FLIP_SCROLL_TIME 100

#define NUM_BILLBOARDS 6
const char template0[] PROGMEM = "InfinityGame";
const char template1[] PROGMEM = "Press Any Button to Play";
const char template2[] PROGMEM = "Play Silly Slots";
const char template3[] PROGMEM = "Play The TimeGame";
const char template4[] PROGMEM = "%s";
const char template5[] PROGMEM = "LONG PRESS for OPTIONS";
const char *const templates[] PROGMEM = { template0, template1, template2, template3, template4, template5 };

extern BillboardsHandler billboards_handler;

void run_billboard(char *data);
void billboard_prompt(voidFuncPtr on_time_out, voidFuncPtr on_press, voidFuncPtr on_long_press);
int button_led_prompt(const char * prompt, const bool *states = NULL);
void title_prompt(const char * title, byte times = 1, bool show_panel_leds = false);
int panel_led_prompt();
void branch_prompt(const char * prompt, voidFuncPtr on_option_1, voidFuncPtr on_option_2, voidFuncPtr on_option_3, voidFuncPtr on_long_press = NULL, const bool *states = NULL);
int toggle_prompt(const char * prompt, const char **labels, byte current_choice, byte toggle_position, byte num_choices);

#endif
