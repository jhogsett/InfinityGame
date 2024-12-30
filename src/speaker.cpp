#include <Arduino.h>
#include "hardware.h"
#include "speaker.h"

void beep(int inv_freq, int pulses){
	for(int j = 0; j < pulses; j++){
		digitalWrite(SPEAKER_PIN, HIGH);
		delayMicroseconds(inv_freq);
		digitalWrite(SPEAKER_PIN, LOW);
		delayMicroseconds(inv_freq);
	}
}

void beep_gap(int inv_freq, int pulses){
	for(int j = 0; j < pulses; j++){
		digitalWrite(SPEAKER_PIN, LOW);
		delayMicroseconds(inv_freq);
		digitalWrite(SPEAKER_PIN, LOW);
		delayMicroseconds(inv_freq);
	}
}

void beeps(int times, int inv_freq, int pulses){
	for(int i = 0; i < times; i++){
		beep(inv_freq, pulses);
		beep_gap(inv_freq, pulses);
	}
}

void alert(int times, int gap, int beep_times, int inv_freq, int pulses){
	for(int i = 0; i < times; i++){
		beeps(beep_times, inv_freq, pulses);
		delay(gap);
	}
}
