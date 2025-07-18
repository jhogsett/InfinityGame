#include <Arduino.h>
#include "bank.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "motor.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "speaker.h"
#include "string_constants.h"
#include "utils.h"
#include "time_game.h"
#include "timeouts.h"
#include "debug.h"
#include "time_game_class.h"

bool time_game() {
    TimeGame game;
    return game.run();
}
