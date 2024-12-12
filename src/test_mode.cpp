#include <Arduino.h>
#include "buffers.h"
#include "displays.h"
#include "hardware.h"
#include "prompts.h"
#include "utils.h"
#include "test_mode.h"

#define TEST_TIMEOUT 1000

void test_mode(){
    while(true){
        if(button_led_prompt(load_f_string(F("Press to start"))) < 1)
            return;

        display.scroll_string(load_f_string(F("HOLD A BUTTON")), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
        while(digitalRead(ANY_BUTTON) == LOW);

        unsigned long time_out = millis() + TEST_TIMEOUT;
        unsigned long low_count = 0;
        while(millis() < time_out){
            if(digitalRead(ANY_BUTTON) == LOW)
                low_count++;
        }
        display.scroll_string(load_f_string(F("    DONE")), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
        while(digitalRead(ANY_BUTTON) == HIGH);

        if(low_count == 0)
            sprintf(display_buffer, load_f_string(F("BUTTON   OK")), low_count);
        else
            sprintf(display_buffer, load_f_string(F("%d DROPS")), low_count);

        button_led_prompt(display_buffer);
    }
}
