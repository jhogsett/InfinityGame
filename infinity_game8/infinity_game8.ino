#include <Wire.h>
#include <HT16K33Disp.h>
#include <random_seed.h>
#include "billboard.h"
#include "billboards_handler.h"
#include "led_handler.h"
#include "hardware.h"
#include "timeouts.h"
#include "buttons.h"
#include "buffers.h"
#include "utils.h"
#include "leds.h"
#include "displays.h"
#include "play_data.h"
#include "prompts.h"
#include "betting.h"
#include "play_views.h"
#include "time_game.h"
#include "slot_game.h"

// #define USE_SERIAL

#define RANDOM_SEED_PIN A1
static RandomSeed<RANDOM_SEED_PIN> randomizer;

/////////////////////////////////////
// INITIALIZATION
/////////////////////////////////////

void setup_display(){
  Wire.begin();
  display.init(display_brightnesses); 
  /* the duplicated displays do not need reinitialization
  disp1.init(brightness+0); 
  disp2.init(brightness+1); 
  disp3.init(brightness+2); */
  display.clear();
}

void setup_leds(){
  for(byte i = FIRST_LED; i <= LAST_LED; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  unsigned long time = millis();
  panel_leds.begin(time, LEDHandler::STYLE_RANDOM, DEFAULT_PANEL_LEDS_SHOW_TIME, DEFAULT_PANEL_LEDS_BLANK_TIME);
  button_leds.begin(time, LEDHandler::STYLE_PLAIN | LEDHandler::STYLE_BLANKING, DEFAULT_BUTTON_LEDS_SHOW_TIME, DEFAULT_BUTTON_LEDS_BLANK_TIME);
  all_leds.begin(time, LEDHandler::STYLE_RANDOM | LEDHandler::STYLE_BLANKING | LEDHandler::STYLE_MIRROR, DEFAULT_ALL_LEDS_SHOW_TIME, DEFAULT_ALL_LEDS_BLANK_TIME);
}

void setup_buttons(){
  for(byte i = 0; i < NUM_BUTTONS; i++){
    pinMode(i + FIRST_BUTTON, 0x03); // INPUT_PULLDOWN ?!
    button_states[i] = false;
  }
  // while(button_pressed());
}

void setup(){
#ifdef USE_SERIAL
  Serial.begin(115200);
#endif
  randomizer.randomize();
 
  setup_leds();
  setup_display();
  setup_buttons();
  
  load_save_data();

  attachInterrupt(digitalPinToInterrupt(ANY_BUTTON), button_pressed_i, RISING);
  button_states[ANY_COLOR_ID] = false;

  // display.show_string(load_f_string(F("Version 0.0")));
  // delay(1000);
}

void idle_mode(){
  all_leds.deactivate_leds(true);
  if(option_clock_on_idle)
    clock_prompt(0, 0, 12, false);
  else
    sleep_mode();
}

void sleep_mode(){
  display.clear();
  while(panel_led_prompt() == -1);
}

void clock_mode(){
  clock_prompt(0, 0, 12);
}

void timer_mode(){
  timer_prompt(0, 0, 0);
}

void options_menu(){
  char *labels[] = {"Off", "On", "12H", "24H", "SLP", "CLK"};
  option_sound = toggle_prompt(load_f_string(F("SOUND   %s")), labels, option_sound ? 1 : 0, 3, 2) ? true : false;
  option_vibrate = toggle_prompt(load_f_string(F("VIBRATE %s")), labels, option_vibrate ? 1 : 0, 3, 2) ? true : false;
  option_clock_24h = toggle_prompt(load_f_string(F("CLOCK   %s")), labels+2, option_clock_24h ? 1 : 0, 3, 2) ? true : false;
  option_clock_on_idle = toggle_prompt(load_f_string(F("IDLE    %s")), labels+4, option_clock_on_idle ? 1 : 0, 3, 2) ? true : false;
  save_data();
}

void tools_menu(){
  switch(button_led_prompt(load_f_string(F("CLOCK   Go  ")))){
    case -1:
      return;
    case 3:
      clock_mode();
      return;
  }
    
  switch(button_led_prompt(load_f_string(F("TIMER   Go  ")))){
    case -1:
      return;
    case 3:
      timer_mode();
      return;
  }
    
  switch(button_led_prompt(load_f_string(F("SLEEP   Go  ")))){
    case -1:
      return;
    case 3:
      sleep_mode();
      return;
  }

  switch(button_led_prompt(load_f_string(F("RESET   Go  ")))){
    case -1:
      return;
    case 3:
      reset_options();
      return;
  }
}

void options_mode(){
  // title_prompt(load_f_string(F("* OPTIONS")));
  bool buttons[] = {false, true, false, true};
  branch_prompt(load_f_string(F("SET    TOOLS")), options_menu, NULL, tools_menu, NULL, buttons);
}

void main_menu(){
  bool buttons[] = {false, true, false, true};
  branch_prompt(load_f_string(F("Slot    Time")), slots_game, NULL, time_game, NULL, buttons);
}

void loop()
{
  while(button_pressed());
  billboard_prompt(idle_mode, main_menu, options_mode);
}

// have segments disappear randomly or go into place randomly

// keep timer running in the background

// if title show times < 3 doesn't show it

// last frame of scroll animation seems to skip ahead 1

// time game choose a color and the average is stored with it, allowing for three profiles

// tests - button closure time while pressed, glitchyness etc., timing of millis

// options for idle time

// setting for clock speed

// the clock should look different in idle mode so you know it's ok not to use a long press (or in idle mode long press to exit)

