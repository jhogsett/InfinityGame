#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#define DISPLAY_BUFFER 71
#define FSTRING_BUFFER 31
#define COPY_BUFFER 31

// string buffer used to prepare text to go to the display
extern char display_buffer[DISPLAY_BUFFER];

// string buffer used to load string data from program memory (F() strings)
extern char fstring_buffer[FSTRING_BUFFER];

// general purpose buffer used for preparing data for display
extern char copy_buffer[COPY_BUFFER];

#endif