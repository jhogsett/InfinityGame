#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_PULSE_ON_TIME 6
#define MOTOR_PULSE_OFF_T1ME 4
#define VIBRATE_PULSES 5

extern void vibrate(int pulses=VIBRATE_PULSES, int on_time=MOTOR_PULSE_ON_TIME, int off_time=MOTOR_PULSE_OFF_T1ME);

#endif
