#include "game_base.h"
#include "play_views.h"
#include "prompts.h"
#include "options_mode.h"
#include "buttons.h"
#include "utils.h"
#include "play_data.h"

GameBase::GameBase(const __FlashStringHelper* game_name, long play_bet)
    : game_name(game_name), play_bet(play_bet), win(0), finished(false), timeout_time(0) {}

bool GameBase::run() {
    title_prompt(load_f_string(game_name), TITLE_SHOW_TIMES, true);

    setup();
    if (is_finished()) {
        return false;
    }

    while (!is_finished()) {
        reset_timeout(); // Reset timeout at the start of each round
        if (!loop()) { // loop returns false to exit
            break;
        }
        if (handle_long_press() != -1) { // Check for long press to exit
            break;
        }
    }
    return true; // Indicates game finished
}

void GameBase::set_finished(bool val) {
    finished = val;
}

bool GameBase::is_finished() const {
    return finished;
}

void GameBase::reset_timeout() {
    timeout_time = millis() + option_idle_time;
}
