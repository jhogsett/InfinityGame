#ifndef __HARDWARE_H__
#define __HARDWARE_H__

// Arduino Nano pins for all 6 LEDs
// The first three are the button LEDs (digital only pins)
// The next three are the panel LEDs (PWM pins to allow adjusting brightness down to match display)
#define NUM_LEDS 6
#define FIRST_LED 6
#define LAST_LED 11

// Arduino Nano pins for the panel LEDs
// PWM pins to allow adjusting brightness down to match display
#define NUM_PANEL_LEDS 3
#define FIRST_PANEL_LED 9
#define LAST_PANEL_LED 11

// Arduino Nano pins for the button LEDs
// digital pins
#define NUM_BUTTON_LEDS 3
#define FIRST_BUTTON_LED 6
#define LAST_BUTTON_LED 8

// Arduino Nano pins for the 'any' button and three arcade buttons
// The 'any' button is wired by diodes to the three other buttons
// It is used to trigger the interupt service routine to process the button event
#define NUM_BUTTONS 4
#define FIRST_BUTTON 2
#define LAST_BUTTON 5
#define ANY_BUTTON 2
#define GREEN_BUTTON 3
#define AMBER_BUTTON 4
#define RED_BUTTON 5
#define NUM_PLAY_BUTTONS 3
#define FIRST_PLAY_BUTTON 3

// Arduino Nano pin for the vibration feedback motor MOSFET
#define MOTOR_PIN 12

// Arduino Nano pin for the speaker coil MOSFET
#define SPEAKER_PIN 13

#endif
