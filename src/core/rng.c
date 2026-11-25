#include <stdint.h>
#include "rng.h"

static uint32_t state = 1;

void rng_seed(unsigned int seed)
{
    state = (uint32_t)seed;
    if (state == 0)
        state = 1;
}

static uint32_t next(void)
{
    state = state * 1103515245u + 12345u;
    return (state >> 16) & 0x7fff;
}

int rng_range(int lo, int hi)
{
    if (hi <= lo)
        return lo;
    return lo + (int)(next() % (uint32_t)(hi - lo + 1));
}

bool rng_chance(int percent)
{
    return rng_range(1, 100) <= percent;
}
