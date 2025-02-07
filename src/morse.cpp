#include <Arduino.h>
#include "play_data.h"
#include "speaker.h"
#include "morse.h"

const unsigned char morsedata[] PROGMEM = {
    0b10100000, // A
    0b00011000, // B
    0b01011000, // C
    0b00110000, // D
    0b01000000, // E
    0b01001000, // F
    0b01110000, // G
    0b00001000, // H
    0b00100000, // I
    0b11101000, // J
    0b10110000, // K
    0b00101000, // L
    0b11100000, // M
    0b01100000, // N
    0b11110000, // O
    0b01101000, // P
    0b10111000, // Q
    0b01010000, // R
    0b00010000, // S
    0b11000000, // T
    0b10010000, // U
    0b10001000, // V
    0b11010000, // W
    0b10011000, // X
    0b11011000, // Y
    0b00111000, // Z
    0b11111100, // 0
    0b11110100, // 1
    0b11100100, // 2
    0b11000100, // 3
    0b10000100, // 4
    0b00000100, // 5
    0b00001100, // 6
    0b00011100, // 7
    0b00111100, // 8
    0b01111100  // 9
    };


void _send_dot(int time){
    beep(DEFAULT_MORSE_FREQ, time * DOT_FACTOR);
    delay(time * ELEMENT_SPACE_FACTOR);
}

void _send_dash(int time){
    beep(DEFAULT_MORSE_FREQ, time * DASH_FACTOR);
    delay(time * ELEMENT_SPACE_FACTOR);
}

void _send_char_space(int time){
    delay(time * CHAR_SPACE_FACTOR);
}

void _send_word_space(int time){
    delay(time * WORD_SPACE_FACTOR);
}

void _send_morse(int c, int time){
    byte morse = pgm_read_byte(morsedata + c);
    bool start_bit = false;
    for(int i = 0; i < 7; i++){
        morse = morse >> 1;
        byte bit = morse & 0x1;

        if(!start_bit){
            if(bit == 1){
                start_bit = true;
            }
            continue;
        }
        if(bit == 1){
            _send_dash(time);
        } else {
            _send_dot(time);
        }
    }
}

void send_morse(char c, int time){
    int offset = -1;
    if(c == ' '){
        _send_word_space(time);
        return;
    }
    if(c >= '0' && c <= 'z'){
        if(c >= '0' && c <= '9'){
            c -= '0';
            offset = 26;
        } else if(c >= 'A' && c <= 'Z'){
            c -= 'A';
            offset = 0;
        } else if(c >= 'a' && c <= 'z'){
            c -= 'a';
            offset = 0;
        }
        if(offset >= 0){
            _send_morse(c + offset, time);
        }
    }
}

void send_morse(const char *s, int wpm){
    if(wpm == 0)
        wpm = option_wpm;
    int time = MORSE_TIME_FROM_WPM(wpm);
    int l = strlen(s);
    for(int i = 0; i < l; i++){
        send_morse(s[i], time);
        _send_char_space(time);
    }
}
