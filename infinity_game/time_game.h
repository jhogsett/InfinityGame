#ifndef __TIME_GAME_H
#define __TIME_GAME_H


// TIME GAME related 
#define MIN_DELAY 1500
#define MAX_DELAY 5000
#define ROUNDS 3
#define TIME_WIN 1000

void time_game(){
  title_prompt(load_f_string(F("The TimeGame")), TITLE_SHOW_TIMES, true);

  int response;                                 
  response = button_led_prompt(load_f_string(F("Press to Go")));
  if(response == 0 || response == -1)
    return;

  display.clear();
  delay(ROUND_DELAY);

  unsigned long mean = 0;
  for(byte i = 0; i < ROUNDS; i++){
    button_leds.activate_all(true);
    display.scroll_string(load_f_string(F("Wait 4 FLASH")), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
    delay(ROUND_DELAY);
    display.clear();
    button_leds.activate_all(false);

    int del = random(MIN_DELAY, MAX_DELAY+1);
    delay(del);
    
    unsigned long start_time = micros();
    panel_leds.flash_leds();

    while(digitalRead(ANY_BUTTON) == LOW);
    unsigned long reaction_time = micros() - start_time;
    while(digitalRead(ANY_BUTTON) == HIGH);

    mean += reaction_time;    

    micros_to_ms(copy_buffer, reaction_time);
    sprintf(display_buffer, "%s ms", copy_buffer);
    display.scroll_string(display_buffer, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
    delay(ROUND_DELAY);
    display.clear();
  }

  while(button_pressed());

  mean /= ROUNDS;
  micros_to_ms(copy_buffer, mean);
  sprintf(display_buffer, "AVERAGE %s ms", copy_buffer);
  title_prompt(display_buffer);
  delay(ROUND_DELAY);

  if(mean < best_time){
    best_time = mean;
    micros_to_ms(copy_buffer, mean);
    sprintf(display_buffer, "NEW BEST * %s ms", copy_buffer);
    title_prompt(display_buffer);
    delay(ROUND_DELAY);

    display_win(TIME_WIN);
    purse += TIME_WIN;
    display_purse();
    
    save_data();

    sprintf(display_buffer, "NEW BEST * %s ms", copy_buffer);
  } else {
    micros_to_ms(copy_buffer, best_time);
    sprintf(display_buffer, "* Best Time %s ms", copy_buffer);
  }
  while(button_led_prompt(display_buffer) == -1);
}

#endif