#ifndef __SPEAKER_H__
#define __SPEAKER_H__

#define SPEAKER_PIN 13
#define INV_FREQUENCY 339
#define BEEP_PULSES 110
#define BEEPS_TIMES 4
#define BEEPS_DELAY 1250

extern void beep(int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);
extern void beep_gap(int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);
extern void beeps(int times = BEEPS_TIMES, int gap = BEEPS_DELAY, int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);

#endif
