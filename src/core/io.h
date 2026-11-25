#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stddef.h>

/*
 * All input goes through these functions, so that:
 *   1. A game can be replayed from a file: ./dungeon --seed 42 --echo < demo_input.txt
 *   2. The game can exit normally at end of input (EOF) instead of looping forever
 */

/* Read one line into buf (newline removed). Returns false at EOF */
bool read_line(char *buf, size_t size);

/* Read one line and return its first non-space character, uppercased. Returns '\0' at EOF */
char read_option(void);

/* Whether the end of input has been reached */
bool io_eof(void);

/* When on, every input line is printed, so a replay shows what the player typed */
void io_set_echo(bool on);

#endif
