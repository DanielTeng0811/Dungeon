#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "monster.h"
#include "player.h"

static void monster_destroy(Object *self)
{
    /* TODO */
    (void)self;
}

Monster *monster_create(const char *name, int max_health, int attack, int defense)
{
    /* TODO: follow item_create */
    (void)name; (void)max_health; (void)attack; (void)defense;
    (void)monster_destroy;
    return NULL;
}

bool monster_trigger_event(Object *self, Object *other)
{
    Monster *m = (Monster *)self;
    Player *p = (Player *)other;

    /* TODO: the fight loop
     *   for (;;) {
     *       print both sides' health and the options A. Attack / B. Retreat
     *       char c = read_option();
     *       if (c == '\0') return false;   // end of input
     *       ...
     *   }
     */
    (void)m; (void)p;
    return false;
}
