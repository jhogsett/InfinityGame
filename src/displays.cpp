#include <HT16K33Disp.h>

#include "displays.h"

HT16K33Disp display(0x70, 3);
HT16K33Disp disp1(0x70, 1);
HT16K33Disp disp2(0x71, 1);
HT16K33Disp disp3(0x72, 1);
HT16K33Disp displays[] = {disp1, disp2, disp3};
const byte display_brightnesses[] = {DISPLAY_BRIGHTNESS1, DISPLAY_BRIGHTNESS2, DISPLAY_BRIGHTNESS3};
