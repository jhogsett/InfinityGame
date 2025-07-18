#ifndef TIME_GAME_CLASS_H
#define TIME_GAME_CLASS_H

#include "game_base.h"

class TimeGame : public GameBase {
public:
    TimeGame();
    void setup() override;
    bool loop() override;

private:
    int mode;
    unsigned long best_time;
    unsigned long mean_time;
    byte current_round;

    unsigned long render_best_time_per_mode(char * buffer, int mode);
};

#endif // TIME_GAME_CLASS_H
