// #include "buttons.h"
// #include "displays.h"
// #include "hardware.h"
// #include "led_handler.h"
#include "leds.h"
#include "play_data.h"
#include "prompts.h"
#include "sleep_mode.h"
// #include "idle_mode.h"

void idle_mode(){
  all_leds.deactivate_leds(true);
  if(option_clock_on_idle)
    clock_prompt(0, 0, 12, false);
  else
    sleep_mode();
}
