#ifndef __DISPLAY_H__
#define __DISPLAY_H__

HT16K33Disp display(0x70, 3);
HT16K33Disp disp1(0x70, 1);
HT16K33Disp disp2(0x71, 1);
HT16K33Disp disp3(0x72, 1);
HT16K33Disp displays[] = {disp1, disp2, disp3};
static const byte display_brightnesses[] = {1, 9, 15}; 

#endif
