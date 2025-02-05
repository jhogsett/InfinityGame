#include <Arduino.h>
#include "hardware.h"
#include "speaker.h"

void beep(int freq, int time){
    int pulse_width = (int)((1000000L / freq) / 2L);
    int pulses = (int)((time * 1000L) / (pulse_width * 2L));
	pinMode(SPEAKER_PIN, OUTPUT);
	for(int j = 0; j < pulses; j++){
		digitalWrite(SPEAKER_PIN, HIGH);
		delayMicroseconds(pulse_width);
		digitalWrite(SPEAKER_PIN, LOW);
		delayMicroseconds(pulse_width);
	}
}

void beep_gap(int freq, int time){
    int pulse_width = (int)((1000000L / freq) / 2L);
    int pulses = (int)((time * 1000L) / (pulse_width * 2L));

	for(int j = 0; j < pulses; j++){
		delayMicroseconds(pulse_width * 2);
	}
}

void beeps(int times, int freq, int time){
	for(int i = 0; i < times; i++){
		beep(freq, time);
		beep_gap(freq, time);
	}
}

void alert(int times, int gap, int beep_times, int freq, int time){
	for(int i = 0; i < times; i++){
		beeps(beep_times, freq, time);
		delay(gap);
	}
}
