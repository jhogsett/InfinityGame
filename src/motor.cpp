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