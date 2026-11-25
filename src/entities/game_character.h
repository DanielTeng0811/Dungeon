#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include "object.h"

struct GameCharacter {
    Object base; /* GameCharacter "inherits" Object */
    int max_health;
    int current_health;
    int attack;
    int defense;
};

/* Initialize the shared character part. Called when creating a Player / Monster / NPC */
void game_character_init(GameCharacter *c, const char *name, ObjectTag tag,
                         int max_health, int attack, int defense,
                         bool (*trigger_event)(Object *self, Object *other),
                         void (*destroy)(Object *self));

bool game_character_is_dead(const GameCharacter *c);

/* Hit by the given attack: lose health and return the damage taken (you decide the formula) */
int game_character_take_damage(GameCharacter *c, int attack);

#endif
