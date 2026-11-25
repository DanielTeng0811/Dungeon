#ifndef RECORD_H
#define RECORD_H

#include <stdbool.h>
#include "dungeon.h"

/*
 * (Optional) Save / load system.
 * Use fopen / fprintf / fscanf to write the player's state and each room's remaining
 * objects to a file, and read them back in the same order when loading.
 * Note: you cannot save pointers to a file (addresses differ on the next run).
 * Save information you can rebuild from instead, such as room indexes.
 */
bool record_save(const Dungeon *d, const char *filename);
bool record_load(Dungeon *d, const char *filename);

#endif
