#include <Arduino.h>
#include "hardware.h"
#include "motor.h"
#include "play_data.h"

void vibrate(){
	if(option_vib_str){
		vibrate_raw(VIBRATE_PULSES_HIGH, MOTOR_PULSE_ON_TIME_HIGH, MOTOR_PULSE_OFF_T1ME_HIGH);
	} else {
		vibrate_raw(VIBRATE_PULSES_LOW, MOTOR_PULSE_ON_TIME_LOW, MOTOR_PULSE_OFF_T1ME_LOW);
	}
}

void vibrate_raw(int pulses, int on_time, int off_time){
	digitalWrite(MOTOR_PIN, LOW);
	for(int i = 0; i < pulses; i++){
		digitalWrite(MOTOR_PIN, HIGH);
		delay(on_time);
		digitalWrite(MOTOR_PIN, LOW);
		delay(off_time);
	}
}
