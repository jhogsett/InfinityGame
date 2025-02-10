#ifndef __MORSE_H__
#define __MORSE_H__

#define DOT_FACTOR 1
#define DASH_FACTOR 3
#define ELEMENT_SPACE_FACTOR 1
#define CHAR_SPACE_FACTOR 3
#define WORD_SPACE_FACTOR 7

#define DEFAULT_MORSE_WPM 13
#define DEFAULT_MORSE_FREQ 700
#define MORSE_TIME_FROM_WPM(w) (1000 / w)
#define DEFAULT_MORSE_TIME MORSE_TIME_FROM_WPM(DEFAULT_MORSE_WPM)

extern void _send_dot(int time = DEFAULT_MORSE_TIME);
extern void _send_dash(int time = DEFAULT_MORSE_TIME);
extern void _send_char_space(int time = DEFAULT_MORSE_TIME);
extern void _send_word_space(int time = DEFAULT_MORSE_TIME);
extern void _send_morse(int c, int time = DEFAULT_MORSE_TIME);
extern void _send_morse_char(char c, int time = DEFAULT_MORSE_TIME);
extern void send_morse(char c, int wpm = 0);
extern void send_morse(const char *s, int wpm = 0);

#endif
