#ifndef __PROMPTS_H__
#define __PROMPTS_H__

#include "billboards_handler.h"
#include "leds.h"

// typedef void (*voidFuncPtr)(void);
typedef bool (*boolFuncPtr)(void);

// Billboard display related
// number of times to show home billboard "InfinityGame" (effectively time of display because no scroll needed)
#define HOME_TIMES 5
// blacking time between billboards
#define BLANKING_TIME 500

// Prompt related
// number of times to show a title, such as a game title (or effective time of display if no scroll needed)
#define TITLE_SHOW_TIMES 3

// Display handling
// show a display string for 700ms before beginning scrolling for ease of reading
#define DISPLAY_SHOW_TIME 900
// scroll the display every 90ms for ease of reading
#define DISPLAY_SCROLL_TIME 70
// scroll flipped options every 100ms
#define OPTION_FLIP_SCROLL_TIME 100


#define NUM_BILLBOARDS 11
const char template0[] PROGMEM = "InfinityGame";
const char template1[] PROGMEM = "Press A Button to Play";
const char template2[] PROGMEM = "Your WALLET $%s";
const char template3[] PROGMEM = "Your Best Time %s ms";
const char template4[] PROGMEM = "House SAFE $%s";
const char template5[] PROGMEM = "Bank VAULT $%s";
const char template6[] PROGMEM = "GANG STASH $%s";
const char template7[] PROGMEM = "Play Silly Slots";
const char template8[] PROGMEM = "Play The WordGame";
const char template9[] PROGMEM = "Play The TimeGame";
const char template10[] PROGMEM = "LONG PRESS for OPTIONS";
const char *const templates[] PROGMEM = { template0, template1, template2, template3, template4, template5, template6, template7, template8, template9, template10 };

// Identify array position for placing fill-in data
#define BILLBOARD_CASH 2
#define BILLBOARD_TIME 3
#define BILLBOARD_HOUSE 4
#define BILLBOARD_BANK 5
#define BILLBOARD_GANG 6

extern BillboardsHandler billboards_handler;
extern char *billboard_data[NUM_BILLBOARDS];

void run_billboard(char **data);
void billboard_prompt(boolFuncPtr on_time_out, boolFuncPtr on_press, boolFuncPtr on_long_press);
int button_led_prompt(const char * prompt, const bool *states = NULL);
void title_prompt(const char * title, byte times = 1, bool show_panel_leds = false, int show_delay=0, int leds_style=TITLE_PANEL_LEDS_STYLE, int leds_show_time=TITLE_PANEL_LEDS_SHOW_TIME, int leds_blank_time=TITLE_PANEL_LEDS_BLANK_TIME);
int panel_led_prompt();
bool branch_prompt(const char * prompt, boolFuncPtr on_option_1, boolFuncPtr on_option_2, boolFuncPtr on_option_3, boolFuncPtr on_long_press = NULL, const bool *states = NULL);
int toggle_prompt(const char * prompt, const char **labels, byte current_choice, byte toggle_position, byte num_choices);

#endif
