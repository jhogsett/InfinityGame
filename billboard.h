#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__
#include <HT16K33Disp.h>

// todo billboard types: static, scroll, blink

class Billboard
{
public:
  Billboard(char * buffer, int show_delay=0, int frame_delay=0);
  void begin(HT16K33Disp * display, int times);
  void reset_round(HT16K33Disp * display);
  bool step(HT16K33Disp * display, unsigned long time);

  char * _buffer;
  int _show_delay;
  int _frame_delay;
  
private:
  int _frames;
  int _num_times;
};

Billboard::Billboard(char * buffer, int show_delay=0, int frame_delay=0){
  _buffer = buffer;
  _show_delay = show_delay;
  _frame_delay = frame_delay;
}

void Billboard::begin(HT16K33Disp * display, int times){
  _num_times = times;
  reset_round(display);
}

void Billboard::reset_round(HT16K33Disp * display){
  _frames = display->begin_scroll_string(_buffer, _show_delay, _frame_delay);
}

bool Billboard::step(HT16K33Disp * display, unsigned long time){
  if(display->step_scroll_string(time)){
    return true;
  } else {
    _num_times -= 1;
    if(_num_times == 0){
      return false;
    } else {
      reset_round(display);
      return true;
    }
  }
}

#endif
