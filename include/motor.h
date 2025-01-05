#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_PULSE_ON_TIME 5
#define MOTOR_PULSE_OFF_T1ME 5
#define VIBRATE_PULSES 4
#define VIBRATE_GAP_TIME 125

extern void vibrate(int pulses=VIBRATE_PULSES, int on_time=MOTOR_PULSE_ON_TIME, int off_time=MOTOR_PULSE_OFF_T1ME);
extern void vibrate_gap();
// extern void vibes(int times, int pulses=VIBRATE_PULSES);

#endif
