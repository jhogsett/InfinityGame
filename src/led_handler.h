#ifndef __LED_HANDLER_H__
#define __LED_HANDLER_H__

// for STYLE_MIRROR, the secondary LEDs mirroring the first are expected to be in the upper half of the set of LED pins

class LEDHandler
{
public:
  LEDHandler(int first_pin, int num_leds, const int *intensity, int show_time=0, int blank_time=0);
  void begin(unsigned long time, int style, int show_time=0, int blank_time=0);
  void step(unsigned long time);

  void activate_leds(volatile bool *states, bool mirror=false);
  void activate_all(bool state, bool mirror=false);
  void deactivate_leds(bool mirror=false);
  void flash_leds(bool mirror=false, long time=0);

  static const int STYLE_PLAIN    = 0x00; // one LED at a time sequentially
  static const int STYLE_RANDOM   = 0x01; // one LED at a time randomly
  static const int STYLE_BLANKING = 0x02; // blanking period between LED activations
  static const int STYLE_MIRROR   = 0x04; // same output for panel and button LEDs
  // static const int STYLE_STALLING = 0x08; // blanking period between rounds

  static const int DEFAULT_SHOW_TIME  = 250;
  static const int DEFAULT_BLANK_TIME = 250;
  static const int DEFAULT_FLASH_TIME = 50;

private:
  void deactivate_led(int virtual_pin, bool mirror=false);
  void activate_led(int virtual_pin, bool mirror=false);

  int _first_pin;
  int _num_leds;
  const int *_intensity;  // array of ints matching led count, 0 for digitalWrite, 1-255 for analogWrite
  int _show_time;
  int _blank_time;
  int _style;

  char _frame;
  unsigned long _next_frame;
  int _num_frames;
  int _num_states;
  bool _blanking;
  char _active;                // virtual current active led or other state

};

LEDHandler::LEDHandler(int first_pin, int num_leds, const int *intensity, int show_time, int blank_time){
  _first_pin = first_pin;
  _num_leds = num_leds;
  _intensity = intensity;
  _show_time = show_time;
  _blank_time = blank_time;

  _style = 0;
}

void LEDHandler::begin(unsigned long time, int style, int show_time, int blank_time){
  _show_time = show_time ? show_time : DEFAULT_SHOW_TIME;
  _blank_time = blank_time ? blank_time : DEFAULT_BLANK_TIME;

  _style = style;

  // with one LED the random style cannot be used
  if(_num_leds < 2)
    _style &= ~STYLE_RANDOM;

  // with an odd number of LEDs the mirror style cannot be used
  if(_num_leds % 2)
    _style &= ~STYLE_MIRROR;

  _frame = -1;
  // blanking = false;
  _next_frame = time;

  _num_frames = _num_leds;
  if(_style & STYLE_BLANKING)
    _num_frames *= 2;
  if(_style & STYLE_MIRROR)
    _num_frames /= 2;

  _num_states = _num_leds;
  if(_style & STYLE_MIRROR)
    _num_states /= 2;

  _active = -1;
  _blanking = false;
}

void LEDHandler::deactivate_led(int virtual_pin, bool mirror){
  digitalWrite(virtual_pin + _first_pin, LOW);
  if(mirror)
    deactivate_led(virtual_pin + (_num_leds / 2));
}

void LEDHandler::activate_led(int virtual_pin, bool mirror){
  if(_intensity[virtual_pin] == 0)
    digitalWrite(virtual_pin + _first_pin, HIGH);
  else
    analogWrite(virtual_pin + _first_pin, _intensity[virtual_pin]);
  if(mirror)
    activate_led(virtual_pin + (_num_leds / 2));
}

void LEDHandler::step(unsigned long time){
  if(time >= _next_frame){
    if(_active != -1)
      deactivate_led(_active, _style & STYLE_MIRROR);

    _frame++;
    if(_frame >= _num_frames)
      _frame = 0;

    bool blanking_period = (_style & STYLE_BLANKING) && (_frame % 2);
    if(!blanking_period){
      if(_style & STYLE_RANDOM){
        int r;
        while( (r = random(_num_states)) == _active );
        _active = r;
      } else {
        _active++;

        if(_active >= _num_states)
        _active = 0;
      }

      activate_led(_active, _style & STYLE_MIRROR);
    }

    _next_frame = time + (blanking_period ? _blank_time : _show_time);
  }
}

// states points to a bool array with the [0] position 'any' ignored
void LEDHandler::activate_leds(volatile bool *states, bool mirror){
  int effective_pins = mirror ? (_num_leds / 2) : _num_leds;
  for(int virtual_pin = 0; virtual_pin < effective_pins; virtual_pin++){
    if(states[1 + virtual_pin])
      activate_led(virtual_pin, mirror);
    else
      deactivate_led(virtual_pin, mirror);
  }
}

// turn all on or off
void LEDHandler::activate_all(bool state, bool mirror){
  int effective_pins = mirror ? (_num_leds / 2) : _num_leds;
  for(int virtual_pin = 0; virtual_pin < effective_pins; virtual_pin++){
    if(state)
      activate_led(virtual_pin, mirror);
    else
      deactivate_led(virtual_pin, mirror);
  }
}

void LEDHandler::deactivate_leds(bool mirror){
  int effective_pins = mirror ? (_num_leds / 2) : _num_leds;
  for(int virtual_pin = 0; virtual_pin < effective_pins; virtual_pin++)
      deactivate_led(virtual_pin, mirror);
}

void LEDHandler::flash_leds(bool mirror, long time){
  long effective_time = time ? time : DEFAULT_FLASH_TIME;
  int effective_pins = mirror ? (_num_leds / 2) : _num_leds;

  for(int virtual_pin = 0; virtual_pin < effective_pins; virtual_pin++){
    digitalWrite(virtual_pin + _first_pin, HIGH);
    if(mirror)
      digitalWrite(virtual_pin + _first_pin + (_num_leds / 2), HIGH);
  }

  delay(effective_time);

  for(int virtual_pin = 0; virtual_pin < effective_pins; virtual_pin++){
    digitalWrite(virtual_pin + _first_pin, LOW);
    if(mirror)
      digitalWrite(virtual_pin + _first_pin + (_num_leds / 2), LOW);
  }
}

#endif
