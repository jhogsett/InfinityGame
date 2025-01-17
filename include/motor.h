#ifndef __MOTOR_H__
#define __MOTOR_H__


#define MOTOR_PULSE_ON_TIME_LOW 6
#define MOTOR_PULSE_OFF_T1ME_LOW 6
#define VIBRATE_PULSES_LOW 8

#define MOTOR_PULSE_ON_TIME_HIGH 8
#define MOTOR_PULSE_OFF_T1ME_HIGH 8
#define VIBRATE_PULSES_HIGH 8

#define MOTOR_PULSE_ON_TIME_DEFAULT MOTOR_PULSE_ON_TIME_LOW
#define MOTOR_PULSE_OFF_T1ME_DEFAULT MOTOR_PULSE_OFF_T1ME_LOW
#define VIBRATE_PULSES_DEFAULT VIBRATE_PULSES_LOW

#define VIBRATE_GAP_TIME 125

extern void vibrate();
extern void vibrate_raw(int pulses=VIBRATE_PULSES_DEFAULT, int on_time=MOTOR_PULSE_ON_TIME_DEFAULT, int off_time=MOTOR_PULSE_OFF_T1ME_DEFAULT);
extern void vibrate_gap();
// extern void vibes(int times, int pulses=VIBRATE_PULSES);

#endif
