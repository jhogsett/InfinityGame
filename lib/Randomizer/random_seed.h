#ifndef RANDOM_SEED_H
#define RANDOM_SEED_H
#include "Arduino.h"

#define RANDOM_SEED_SAMPLES 16

template<byte pin>
class RandomSeed
{
	public:
	void randomize(void);
};

template<byte pin>
void RandomSeed<pin>::randomize(void){
	int seed = analogRead(pin);
	for(byte i = 0; i < RANDOM_SEED_SAMPLES; i++)
		seed = (seed << 1) ^ analogRead(0);
	randomSeed(seed);
}


#endif
