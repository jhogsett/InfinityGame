#ifndef __SPEAKER_H__
#define __SPEAKER_H__

// empirically discovered loudest and most pleasant resonant frequency of the particular speaker used in the v2 PLA 3D printed cabinet
// #define INV_FREQUENCY 339
// F#6
#define BEEP_FREQUENCY 1478

// how many pulses to create a beep
// #define BEEP_PULSES 90
#define BEEP_TIME 60

// how many times to beep (with an equal sized pause in between)
#define BEEPS_TIMES 4

// how many times to play the four-beeps to alert the user (that the Timer has elapsed)
#define ALERT_TIMES 4

// delay between the alert beeps
#define ALERT_DELAY 600

extern void beep(int freq = BEEP_FREQUENCY, int time = BEEP_TIME);
extern void beep_gap(int freq = BEEP_FREQUENCY, int time = BEEP_TIME);
extern void beeps(int times = BEEPS_TIMES, int freq = BEEP_FREQUENCY, int time = BEEP_TIME);
extern void alert(int times = ALERT_TIMES, int gap = ALERT_DELAY, int beep_times = BEEPS_TIMES, int freq = BEEP_FREQUENCY, int time = BEEP_TIME);

#endif
