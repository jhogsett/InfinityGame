#ifndef __PROMPTS_H__
#define __PROMPTS_H__

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

BillboardsHandler billboards_handler(display_buffer, NUM_BILLBOARDS, templates, BLANKING_TIME, HOME_TIMES, false, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);

void run_billboard(char *data) {
  unsigned long time = millis();
  panel_leds.step(time);
  // button_leds.step(time);
  // all_leds.step(time);
  billboards_handler.run(time, &display, data);
}

// show the billboard and cycle waiting for any button press
void billboard_prompt(voidFuncPtr on_time_out, voidFuncPtr on_press, voidFuncPtr on_long_press) {
  while (button_pressed())
    ;
  unsigned long sleep_timeout = millis() + SLEEP_TIMEOUT;
  unsigned long time;

  all_leds.deactivate_leds(true);
  billboards_handler.reset();
  panel_leds.begin(time, LEDHandler::STYLE_RANDOM, 750, 350);

  micros_to_ms(display_buffer, best_time);
  sprintf(copy_buffer, load_f_string(F("Cash $%ld Best Time %s ms")), purse, display_buffer);

  while ((time = millis()) < sleep_timeout) {
    run_billboard(copy_buffer);

    if (button_pressed()) {
      all_leds.activate_leds(button_states, true);

      int long_press_state;
      while ((long_press_state = wait_on_long_press()) == 0)
        ;
      all_leds.deactivate_leds(true);

      if (long_press_state == 1) {
        on_long_press();
        return;
      } else {
        on_press();
        return;
      }
    }
  }
  on_time_out();
  return;
}

// prompt with text and cycle waiting for a button response
// returns -1=timed out, 0=long press, button ID otherwise
int button_led_prompt(char *prompt, bool *states = NULL) {
  unsigned long time;
  unsigned long timeout_time = millis() + PROMPT_TIMEOUT;

  all_leds.deactivate_leds(true);
  if (states)
    button_leds.activate_leds(states);

  display.begin_scroll_loop();
  while ((time = millis()) < timeout_time) {
    display.loop_scroll_string(time, prompt, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);

    if (button_pressed()) {
      all_leds.activate_leds(button_states, true);

      int long_press_state;
      while ((long_press_state = wait_on_long_press()) == 0)
        ;
      all_leds.deactivate_leds(true);

      if (long_press_state == 1)
        return 0;
      else {
        if (button_states[GREEN_ID])
          return GREEN_ID;
        else if (button_states[AMBER_ID])
          return AMBER_ID;
        else if (button_states[RED_ID])
          return RED_ID;
      }
    }
    // restore button leds in case of debounce time failure
    if (states)
      button_leds.activate_leds(states);
  }
  return -1;
}

// prompt with text showing, no cycle waiting for a response
// but cancelable with a button press
bool title_prompt(char *title, byte times = 1, int show_panel_leds = false) {
  all_leds.deactivate_leds(true);
  display.begin_scroll_loop(times);
  if (show_panel_leds)
    panel_leds.begin(millis(), TITLE_PANEL_LEDS_STYLE, TITLE_PANEL_LEDS_SHOW_TIME, TITLE_PANEL_LEDS_BLANK_TIME);
  // breaking out of the loop is handled by the display call
  while (true) {
    unsigned long time = millis();
    if (display.loop_scroll_string(time, title, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME)) {
      if (show_panel_leds)
        panel_leds.step(time);
      if (button_pressed())
        break;
    } else
      break;
  }
}

// prompt with panel leds showing only and cycle waiting for any button press
int panel_led_prompt() {
  all_leds.deactivate_leds(true);
  panel_leds.begin(millis(), LEDHandler::STYLE_RANDOM | LEDHandler::STYLE_BLANKING, 1500, 1500);

  // the sleep mode never times out
  while (true) {
    panel_leds.step(millis());

    if (!button_pressed())
      continue;

    all_leds.activate_leds(button_states, true);

    while (button_still_pressed())
      ;
    all_leds.deactivate_leds(true);

    return 1;
  }

  return -1;
}

// TODO button_led_prompt() blocks, so the loop here might not be needed (would be if there were LEDS or the display to run here)
// returns if timed out waiting for input
void branch_prompt(char *prompt, voidFuncPtr on_option_1, voidFuncPtr on_option_2, voidFuncPtr on_option_3, voidFuncPtr on_long_press = NULL, bool *states = NULL) {
  unsigned long prompt_timeout = millis() + PROMPT_TIMEOUT;
  unsigned long time;

  byte choice;
  while ((time = millis()) < prompt_timeout) {
    choice = button_led_prompt(prompt, states);
    switch (choice) {
      case -1:
        return;
      case 0:
        if (on_long_press)
          on_long_press();
        // always return so long pressing is a way to go back
        return;
      case 1:
        if (on_option_1) {
          on_option_1();
          return;
        }
        break;
      case 2:
        if (on_option_2) {
          on_option_2();
          return;
        }
        break;
      case 3:
        if (on_option_3) {
          on_option_3();
          return;
        }
        break;
    }
  }
}

// returns -1 on timeout or long press, otherwise current choice
// current_choice and return value are zero-based
// toggle_position is one-based to be consistent with button states
int toggle_prompt(char *prompt, char **labels, byte current_choice, byte toggle_position, byte num_choices) {
  unsigned long prompt_timeout = millis() + PROMPT_TIMEOUT;
  unsigned long time;

  while ((time = millis()) < prompt_timeout) {
    sprintf(display_buffer, prompt, labels[current_choice]);

    bool states[] = { false, false, false, false };
    states[toggle_position] = true;

    int choice = button_led_prompt(display_buffer, states);
    if (choice == 0 || choice == -1)
      return -1;

    if (choice == toggle_position) {
      current_choice++;
      if (current_choice >= num_choices) {
        current_choice = 0;
      }
      displays[toggle_position].scroll_string(labels[current_choice], 1, OPTION_FLIP_SCROLL_TIME);
      continue;
    } else
      return current_choice;
  }
}

long time_to_seconds(byte second, byte minute, byte hour) {
  long result = (long)second + (60L * (long)minute) + (3600L * (long)hour);
  return result;
}

void seconds_to_time(long seconds, byte &second, byte &minute, byte &hour) {
  hour = seconds / 3600L;
  seconds -= hour * 3600L;
  minute = seconds / 60L;
  seconds -= minute * 60L;
  second = seconds;
}

#define DEFAULT_TIME_BASIS (1000L * 12L * 3600L)

unsigned long clock_basis = 0L;
unsigned long time_basis = DEFAULT_TIME_BASIS;
byte clock_hour, clock_minute, clock_second;

void establish_clock_basis(byte seconds, byte minutes, byte hours) {
  clock_basis = millis();
  long total_seconds = time_to_seconds(seconds, minutes, hours);
  time_basis = total_seconds * 1000L;
}

#define MAX_MS_PER_DAY (1000L * 24L * 60L * 60L)

long time_in_seconds() {
  unsigned long time_in_ms = millis() - clock_basis + time_basis;

  if (time_in_ms > MAX_MS_PER_DAY) {
    time_in_ms -= MAX_MS_PER_DAY;
    clock_basis += MAX_MS_PER_DAY;
  }

  return time_in_ms / 1000L;
}

void increment_timer(byte &second, byte &minute, byte &hour, byte seconds = 1, byte minutes = 0, byte hours = 0) {
  long total_seconds = time_to_seconds(second, minute, hour);
  total_seconds += time_to_seconds(seconds, minutes, hours);
  seconds_to_time(total_seconds, second, minute, hour);
}

bool decrement_timer(byte &second, byte &minute, byte &hour, int seconds = 1, int minutes = 0, int hours = 0) {
  long total_seconds = time_to_seconds(second, minute, hour);
  total_seconds -= time_to_seconds(seconds, minutes, hours);
  if (total_seconds < 0)
    total_seconds = 0;
  seconds_to_time(total_seconds, second, minute, hour);
  return total_seconds > 0;
}

void increment_time_basis(byte &second, byte &minute, byte &hour, byte seconds = 1, byte minutes = 0, byte hours = 0) {
  long total_seconds = time_basis / 1000L;
  total_seconds += time_to_seconds(seconds, minutes, hours);
  seconds_to_time(total_seconds, second, minute, hour);

  if (option_clock_24h) {
    hours %= 24;
  } else {
    hours %= 12;
    if (hour == 0)
      hour = 12;
  }

  total_seconds = time_to_seconds(second, minute, hour);
  time_basis = total_seconds * 1000L;
}

void render_clock_string(byte seconds, byte minutes, byte hours) {
  byte effective_hours;
  if (option_clock_24h) {
    effective_hours = hours;
  } else {
    effective_hours = (hours % 12);
    if (effective_hours == 0)
      effective_hours = 12;
  }

  if (effective_hours < 10)
    sprintf(display_buffer, load_f_string(F("   %1d %02d %02d  ")), effective_hours, minutes, seconds);
  else
    sprintf(display_buffer, load_f_string(F("  %2d %02d %02d  ")), effective_hours, minutes, seconds);
}

int clock_prompt(byte seconds, byte minutes, byte hours, byte settable = true) {
  // unsigned long next_second = millis() + 1000;
  clock_hour = hours;
  clock_minute = minutes;
  clock_second = seconds;

  render_clock_string(clock_second, clock_minute, clock_hour);
  display.show_string(display_buffer);
  while (true) {
    // unsigned long time = millis();
    seconds_to_time(time_in_seconds(), clock_second, clock_minute, clock_hour);

    // if(time >= next_second){
    // increment_timer(clock_second, clock_minute, clock_hour);
    render_clock_string(clock_second, clock_minute, clock_hour);
    display.show_string(display_buffer);
    // next_second = time + 1000;
    // }

    if (button_pressed()) {
      all_leds.activate_leds(button_states, true);

      int long_press_state;
      while ((long_press_state = wait_on_long_press()) == 0)
        ;
      all_leds.deactivate_leds(true);

      if (long_press_state == 1) {
        return;
      } else {
        if (settable) {
          if (button_states[GREEN_ID]) {
            increment_time_basis(clock_second, clock_minute, clock_hour, 0, 0, 1);
          } else if (button_states[AMBER_ID])
            increment_time_basis(clock_second, clock_minute, clock_hour, 0, 1, 0);
          else if (button_states[RED_ID]) {
            clock_second = 0;
            establish_clock_basis(clock_second, clock_minute, clock_hour);
          }
        } else
          return;
      }
    }
  }
}

byte timer_hour, timer_minute, timer_second;

void render_timer_string(byte seconds, byte minutes, byte hours, bool running) {
  char *indicator;
  if (running)
    indicator = "Stop";
  else
    indicator = " Go ";

  if (timer_hour < 1)
    sprintf(display_buffer, load_f_string(F("%s %02d %02d  ")), indicator, timer_minute, timer_second);
  else {
    sprintf(display_buffer, load_f_string(F("%s %02d. %02d  ")), indicator, timer_hour, timer_minute);
  }
}

int timer_prompt(byte seconds = 0, byte minutes = 0, byte hours = 0) {
  unsigned long next_second = millis() + 1000;
  timer_hour = hours;
  timer_minute = minutes;
  timer_second = seconds;
  bool running = false;
  bool going_up = false;

  render_timer_string(timer_second, timer_minute, timer_hour, running);
  display.show_string(display_buffer);
  while (true) {
    unsigned long time = millis();

    if (running && time >= next_second) {
      if (going_up)
        increment_timer(timer_second, timer_minute, timer_hour);
      else if (!decrement_timer(timer_second, timer_minute, timer_hour))
        running = false;

      render_timer_string(timer_second, timer_minute, timer_hour, running);
      display.show_string(display_buffer);
      next_second = time + 1000;
    }

    if (button_pressed()) {
      all_leds.activate_leds(button_states, true);

      int long_press_state;
      while ((long_press_state = wait_on_long_press()) == 0)
        ;
      all_leds.deactivate_leds(true);

      if (long_press_state == 1) {
        return;
      } else {
        if (button_states[GREEN_ID]) {
          if (!running) {
            running = true;
            going_up = time_to_seconds(timer_second, timer_minute, timer_hour) == 0;
            next_second = millis() + 1000;
          } else {
            running = false;
          }
        } else if (button_states[AMBER_ID]) {
          increment_timer(timer_second, timer_minute, timer_hour, 0, 1, 0);
        } else if (button_states[RED_ID]) {
          increment_timer(timer_second, timer_minute, timer_hour, 1, 0, 0);
        }
        render_timer_string(timer_second, timer_minute, timer_hour, running);
        display.show_string(display_buffer);
      }
    }
  }
}
#endif
