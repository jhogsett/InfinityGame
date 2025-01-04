#ifndef __DEBUG_H__
#define __DEBUG_H__

#define ENABLE_DEBUG_FEATURES

#ifdef ENABLE_DEBUG_FEATURES
// arbitrary use variable that can be used to check for causes directly on the device
extern unsigned long debug_marker;

extern void set_debug_marker(unsigned long marker);
extern void clear_debug_marker();
extern void debug_mode();
#endif

#endif
