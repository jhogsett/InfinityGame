#include <Arduino.h>
#include "hardware.h"
#include "motor.h"

void vibrate(int pulses, int on_time, int off_time){
	digitalWrite(MOTOR_PIN, LOW);
	for(int i = 0; i < pulses; i++){
		digitalWrite(MOTOR_PIN, HIGH);
		delay(on_time);
		digitalWrite(MOTOR_PIN, LOW);
		delay(off_time);
	}
}

void vibrate_gap(){
	delay(VIBRATE_GAP_TIME);
}

// void vibes(int times, int pulses){
// 	for(int i = 0; i < times; i++){
// 		vibrate(pulses);
// 		vibrate_gap();
// 	}
// }
