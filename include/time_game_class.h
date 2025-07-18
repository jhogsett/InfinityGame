#ifndef TIME_GAME_CLASS_H
#define TIME_GAME_CLASS_H

#include <Arduino.h>
#include "game_base.h"
#include "play_data.h" // For MONEY_BASIS

// Constants moved from time_game.h
#define MIN_DELAY 2500
#define MAX_DELAY 5000
#define TIME_GAME_ROUNDS 3
#define ROUNDS_SHOW_TIME 350
#define BEAT_SHOW_TIME 550
#define TIME_WIN (1000L / MONEY_BASIS)
#define MODE_FLASH 0
#define MODE_SOUND 1
#define MODE_VIBRATION 2
#define TIMEGAME_MOTOR_ON 12
#define TIMEGAME_MOTOR_OFF 12
#define TIMEGAME_PULSES 6

class TimeGame : public GameBase {
public:
    TimeGame();
    void setup() override;
    bool loop() override;

private:
    // Member variables
    byte mode;
    byte round;
    unsigned long mean_time;
    
    // Helper method
    unsigned long renderBestTimePerMode(int mode);
};

#endif // TIME_GAME_CLASS_H
