#ifndef __SLOT_GAME_H__
#define __SLOT_GAME_H__

#define SLOTS_SHOW_TIME   20
#define SLOTS_SCROLL_TIME 20

#define WIN_TRIPLE 5
#define WIN_DOUBLE 2
#define WIN_WORD  1
#define WIN_WORD_CUTOFF 1
#define DEFAULT_BET 10
#define WIN_WORD_BONUS 2
#define WIN_JACKPOT 100

// SLOT game

#define NUM_WORDS 10
static const char *rude_words[NUM_WORDS] = {"FUCK", "SHIT", "CUNT", "COCK", "PISS", "TITS", "FART", "POOP", "DICK", "BOOB"};
static const char *nice_words[NUM_WORDS] = {"WEED", "VAPE", "BEER", "WINE", "TACO", "GOLD", "MINT", "GIFT", "JADE", "CAKE"};
byte choice1, choice2, choice3;

void slots_round(bool rude){
  disp1.begin_scroll_loop(1);
  disp2.begin_scroll_loop(2);
  disp3.begin_scroll_loop(3);

  char * text;
  char **words;
  if(rude){
    text = load_f_string(F("    FUCK  SHIT  CUNT  COCK  PISS  TITS  FART  POOP  DICK  BOOB"));
    words = rude_words;
  } else {  
    text = load_f_string(F("    WEED  VAPE  BEER  WINE  TACO  GOLD  MINT  GIFT  JADE  CAKE"));
    words = nice_words;
  }

  bool running1 = true;
  bool running2 = true;
  bool running3 = true;
  while(running1 || running2 || running3){
    unsigned long time = millis();

    if(running1){
      running1 = disp1.loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME); 
      if(!running1){
        choice1 = random(NUM_WORDS);
        disp1.show_string(words[choice1]);
      }
    }

    if(running2){
      running2 = disp2.loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME); 
      if(!running2){
        choice2 = random(NUM_WORDS);
        disp2.show_string(words[choice2]);
      }
    }

    if(running3){
      running3 = disp3.loop_scroll_string(time, text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME); 
      if(!running3){
        choice3 = random(NUM_WORDS);
        disp3.show_string(words[choice3]);
      }
    }
  }
}

bool triple_word_chosen(){
  return choice1 == choice2 && choice2 == choice3;
}

bool double_word_chosen(){
  return choice1 == choice2 || choice2 == choice3 || choice1 == choice3;
}

bool special_word_chosen(){
  return choice1 < WIN_WORD_CUTOFF || choice2 < WIN_WORD_CUTOFF || choice3 < WIN_WORD_CUTOFF;
}

bool jackpot_words_chosen(byte word1, byte word2, byte word3){
  return choice1 == word1 && choice2 == word2 && choice3 == word3;
}

void slots_game(){
  title_prompt(load_f_string(F("Silly Slots")), TITLE_SHOW_TIMES, true);
  
  byte jackpot_choice1 = random(NUM_WORDS);
  byte jackpot_choice2 = random(NUM_WORDS);
  byte jackpot_choice3 = random(NUM_WORDS);
   
  bool rude;
  bool buttons[] = {false, true, false, true};
  switch(button_led_prompt(load_f_string(F("NICE    RUDE")), buttons)){
    case -1:
      return;
    case 0:
      return;
    case 1:
      rude = false;
      break;
    case 2:
      rude = random(2) == 0 ? true : false;
      break;
    case 3:
      rude = true;
      break;
  }

  unsigned long sleep_timeout = millis() + SLEEP_TIMEOUT;
  unsigned long time;
  
  while((time = millis()) < sleep_timeout){
    bet_amounts[BET_ALL] = purse;
    sprintf(display_buffer, load_f_string(F("Bet %s Back")), standard_bet_str(current_bet));
    bool states[] = {false, true, false, false};
    switch(button_led_prompt(display_buffer, states)){
      case -1:
        return;
      case 0:
        return;
      case 1:
        break;
      case 2:
        current_bet++;
        if(current_bet >= NUM_BET_AMOUNTS)
          current_bet = 0;
        sprintf(display_buffer, load_f_string(F("    %s")), standard_bet_str(current_bet));
        disp2.scroll_string(display_buffer, 1, OPTION_FLIP_SCROLL_TIME);
        continue;
      case 3:
        return;
    }

    sleep_timeout = millis() + SLEEP_TIMEOUT;

    int win = 0;
    bool jackpot = false;
    purse -= bet_amounts[current_bet];
    save_data();

    slots_round(rude);  

    while(button_pressed());

    char **words;
    if(rude){
      words = rude_words;
    } else {  
      words = nice_words;
    }

    sprintf(display_buffer, "%s%s%s", words[choice1], words[choice2], words[choice3]);
    title_prompt(display_buffer);

    if(jackpot_words_chosen(jackpot_choice1, jackpot_choice2, jackpot_choice3)){
      win = WIN_JACKPOT;
      jackpot = true;
    } else if(triple_word_chosen()){
      win = WIN_TRIPLE;
      if(special_word_chosen())
        win *= WIN_WORD_BONUS;
    } else if(double_word_chosen()){
      win = WIN_DOUBLE;
      if(special_word_chosen())
        win *= WIN_WORD_BONUS;
    } else if(choice1 < WIN_WORD_CUTOFF || choice2 < WIN_WORD_CUTOFF || choice3 < WIN_WORD_CUTOFF) {
      win = WIN_WORD;
    } 

    win *= bet_amounts[current_bet];

    if(jackpot)
      display_jackpot(win);
    else if(win)
      display_win(win);
    else 
      // see the non-winning results in lieu of being told you lost
      delay(ROUND_DELAY);

    purse += win;
    save_data();
    
    display_purse();
  }
}

#endif
