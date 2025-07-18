## Nano Every Fixes Needed

- Pin 11 is not a PWM pin
    - Move Red Panel LED to Pin 5
    - Move Red Button input to Pin 11
    - Fix assumption that panel LED pins are sequential
        - #define LAST_PANEL_LED 11
