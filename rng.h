#ifndef RNG_H
#define RNG_H

#include <stdbool.h>

/*
 * Our own random number generator. We do not use rand() because it gives different
 * results on Windows / macOS / Linux, so the same seed would play differently on
 * another computer and the TAs could not replay your game.
 */
void rng_seed(unsigned int seed);
int rng_range(int lo, int hi);   /* Returns an integer from lo to hi (inclusive) */
bool rng_chance(int percent);    /* Returns true with a probability of percent% */

#endif
