#ifndef __BUTTON_HANDLER_H__
#define __BUTTON_HANDLER_H__

// button handling
#define ANY_COLOR_ID 0
#define FIRST_COLOR_ID 1
#define LAST_COLOR_ID 3
#define NUM_COLOR_IDS 3
#define GREEN_ID 1
#define AMBER_ID 2
#define RED_ID 3
#define NO_COLOR_ID -1

// #define DEBOUNCE_TIME 10
// #define LONG_PRESS_TIME 1500
// #define PROMPT_TIMEOUT 20000
// // #define SLEEP_TIMEOUT 300000
// #define SLEEP_TIMEOUT 30000

volatile bool button_states[NUM_BUTTONS];
volatile unsigned long press_time;

/////////////////////////////////////
// LOW-LEVEL BUTTON HAHDLING
/////////////////////////////////////

// interupt if any button is pressed
// saves the time pressed for debouncing, long press etc
// saves all the button states
void button_pressed_i(){
  press_time = millis();
  button_states[ANY_COLOR_ID] = true;
  button_states[GREEN_ID] = digitalRead(GREEN_BUTTON);
  button_states[AMBER_ID] = digitalRead(AMBER_BUTTON);
  button_states[RED_ID] = digitalRead(RED_BUTTON);
}

// use in conjunction with the ISR
bool button_pressed(){
  // do nothing if no button has been presssed according to the ISR
  if(!button_states[ANY_COLOR_ID])
    return false;

  // enforce a debounce period
  // if the button is unpressed during this time, cancel the press
  // this depends on being called immediately after the ISR has recorded a press,
  //   ie. in a loop
  unsigned long debounce_timeout =  press_time + DEBOUNCE_TIME;
  while(millis() < debounce_timeout){
    if(!digitalRead(ANY_BUTTON)){
      button_states[ANY_COLOR_ID] = false;
      return false;
    }
  }
  // int count = 0, hits = 0;
  // while(millis() < debounce_timeout)
  // {
  //   count++;    
  //   hits += digitalRead(ANY_BUTTON) == HIGH ? 1 : 0;
  // }

  // return if off more than half the time during the debounce period
  // if(count / 2 > hits)
  //   return false;

  // if(!digitalRead(ANY_BUTTON)){
  //   button_states[ANY_COLOR_ID] = false;
  //   return false;
  // }

  button_states[ANY_COLOR_ID] = false;
  button_states[GREEN_ID] = digitalRead(GREEN_BUTTON);
  button_states[AMBER_ID] = digitalRead(AMBER_BUTTON);
  button_states[RED_ID] = digitalRead(RED_BUTTON);
  return true;
}

bool button_still_pressed(){
  return digitalRead(ANY_BUTTON);
}

// use in conjunction with button_pressed()
// 0=still waiting, 1=long press, -1=canceled
int wait_on_long_press(){
  unsigned long target_time = press_time + LONG_PRESS_TIME;

  if(millis() < target_time){
    // still waiting on long press
    if(button_still_pressed())
      return 0;
    else
      return -1;
      // return 0;
  } else {
    // long press time has been met
    if(button_still_pressed())
      return 1;
    else
      return -1;
  }
}



#endif