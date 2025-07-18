#include <Arduino.h>
#include "time_game.h"
#include "time_game_class.h"

bool time_game(){
    TimeGame game;
    game.run();
    return false;
}
