#ifndef __SPEAKER_H__
#define __SPEAKER_H__

#define INV_FREQUENCY 339
#define BEEP_PULSES 90
#define BEEPS_TIMES 4
#define ALERT_TIMES 2
#define ALERT_DELAY 600

extern void beep(int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);
extern void beep_gap(int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);
extern void beeps(int times = BEEPS_TIMES, int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);
extern void alert(int times = ALERT_TIMES, int gap = ALERT_DELAY, int beep_times = BEEPS_TIMES, int inv_freq = INV_FREQUENCY, int pulses = BEEP_PULSES);

#endif
