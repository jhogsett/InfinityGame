#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <HT16K33Disp.h>

#define DISPLAY_BRIGHTNESS1 1  // Emerald Green needs minimum brightness to match Red
#define DISPLAY_BRIGHTNESS2 9  // Orange needs balance between Emerald Green and REd
#define DISPLAY_BRIGHTNESS3 15 // Red needs maximum brightness to match minimum Emerald Green brightness

extern HT16K33Disp display;
extern HT16K33Disp disp1;
extern HT16K33Disp disp2;
extern HT16K33Disp disp3;
extern HT16K33Disp displays[];
extern const byte display_brightnesses[];

#endif
