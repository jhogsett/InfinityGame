#ifndef GAME_BASE_H
#define GAME_BASE_H

#include <Arduino.h>

class GameBase {
public:
    GameBase(const __FlashStringHelper *name, unsigned long cost);
    virtual ~GameBase() {}

    // Main game runner - calls setup() then loop() until finished
    void run();

    // Pure virtual methods that each game must implement
    virtual void setup() = 0;
    virtual bool loop() = 0; // Returns true to continue, false to exit

protected:
    const __FlashStringHelper *game_name;
    unsigned long play_cost;
    bool finished;

    // Helper method for games to signal they're done
    void setFinished(bool done = true) { finished = done; }
};

#endif // GAME_BASE_H
