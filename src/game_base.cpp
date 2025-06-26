#include "game_base.h"

GameBase::GameBase(const __FlashStringHelper *name, unsigned long cost) 
    : game_name(name), play_cost(cost), finished(false) {}

void GameBase::run() {
    finished = false;
    setup();
    while (!finished) {
        if (!loop()) {
            finished = true;
        }
    }
}
