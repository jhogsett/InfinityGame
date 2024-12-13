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
void title_prompt(const char * title, byte times = 1, int show_panel_leds = false);
int panel_led_prompt();
void branch_prompt(const char * prompt, voidFuncPtr on_option_1, voidFuncPtr on_option_2, voidFuncPtr on_option_3, voidFuncPtr on_long_press = NULL, const bool *states = NULL);
int toggle_prompt(const char * prompt, const char **labels, byte current_choice, byte toggle_position, byte num_choices);
long time_to_seconds(byte second, byte minute, byte hour);
void seconds_to_time(long seconds, byte &second, byte &minute, byte &hour);

#define DEFAULT_TIME_BASIS (1000L * 12L * 3600L)

extern unsigned long clock_basis;
extern unsigned long time_basis;
extern byte clock_hour, clock_minute, clock_second;

void establish_clock_basis(byte seconds, byte minutes, byte hours);

#define MAX_MS_PER_DAY (1000L * 24L * 60L * 60L)

extern long time_in_seconds();
extern void increment_timer(byte &second, byte &minute, byte &hour, byte seconds = 1, byte minutes = 0, byte hours = 0);
extern bool decrement_timer(byte &second, byte &minute, byte &hour, int seconds = 1, int minutes = 0, int hours = 0);
extern void increment_time_basis(byte &second, byte &minute, byte &hour, byte seconds = 1, byte minutes = 0, byte hours = 0);
extern void render_clock_string(byte seconds, byte minutes, byte hours);
extern void clock_prompt(byte seconds, byte minutes, byte hours, byte settable = true);

extern byte timer_hour, timer_minute, timer_second;

extern void render_timer_string(byte seconds, byte minutes, byte hours, bool running);
extern void timer_prompt(byte seconds = 0, byte minutes = 0, byte hours = 0);

#endif
