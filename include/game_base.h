#ifndef GAME_BASE_H
#define GAME_BASE_H

#include <Arduino.h>

class GameBase {
public:
    GameBase(const __FlashStringHelper* game_name, long play_bet);
    bool run();

protected:
    virtual void setup() = 0;
    virtual bool loop() = 0; // Returns true to continue, false to exit

    void set_finished(bool val);
    bool is_finished() const;
    void reset_timeout();

    const __FlashStringHelper* game_name;
    long play_bet;
    long win;

private:
    bool finished;
    unsigned long timeout_time;
};

#endif // GAME_BASE_H
