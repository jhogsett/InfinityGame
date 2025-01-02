#ifndef __TEST_MODE_H__
#define __TEST_MODE_H__

// #define ENABLE_TEST_FEATURES

#ifdef ENABLE_TEST_FEATURES
#define GLITCH_TEST_TIMEOUT 1000
#define NOISE_TEST_TIMEOUT 60000
#define GLITCH_TEST_TIMES 3
#define LED_SHOW_TIME 5
#define LED_BLANK_TIME 5

extern void test_mode();
#endif

#endif
