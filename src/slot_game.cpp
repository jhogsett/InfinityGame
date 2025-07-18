#include <Arduino.h>
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "timeouts.h"
#include "utils.h"
#include "seeding.h"
#include "slot_game.h"
#include "slot_game_class.h"
#include "word_lists.h"

bool slots_game(){
    SlotGame game;
    return game.run();
}
