#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dungeon.h"
#include "io.h"
#include "rng.h"

/*
 * Usage:
 *   ./dungeon                         normal play (seed taken from the clock)
 *   ./dungeon --seed 42               fixed seed: same random events every run
 *   ./dungeon --seed 42 --echo < demo_input.txt   how the TAs replay your demo
 */
int main(int argc, char *argv[])
{
    unsigned int seed = (unsigned int)time(NULL);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc)
            seed = (unsigned int)strtoul(argv[++i], NULL, 10);
        else if (strcmp(argv[i], "--echo") == 0)
            io_set_echo(true);
    }
    rng_seed(seed);
    printf("(seed = %u)\n", seed);

    Dungeon dungeon;
    dungeon_init(&dungeon);
    dungeon_run(&dungeon);
    dungeon_free(&dungeon);
    return 0;
}
