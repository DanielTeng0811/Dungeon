#ifndef MONSTER_H
#define MONSTER_H

#include "game_character.h"

struct Monster {
    GameCharacter base; /* Monster "inherits" GameCharacter */
    /* Add any fields you want: dropped item, gold, whether its attack poisons... */
};

Monster *monster_create(const char *name, int max_health, int attack, int defense);

/*
 * Monster's version of trigger_event: the Fighting System.
 * self is the monster, other is the player.
 *   - The player chooses Attack or Retreat
 *   - Attack: the player hits the monster; if it survives, it fights back.
 *     Repeat until one of them dies or the player retreats.
 *   - Retreat: the player goes back to the previous room (previous_room)
 * Return true if the monster died, so it is removed from the room and freed.
 */
bool monster_trigger_event(Object *self, Object *other);

#endif
