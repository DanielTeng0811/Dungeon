#include "game_character.h"

void game_character_init(GameCharacter *c, const char *name, ObjectTag tag,
                         int max_health, int attack, int defense,
                         bool (*trigger_event)(Object *self, Object *other),
                         void (*destroy)(Object *self))
{
    /* TODO: initialize c->base with object_init, then set health, attack, defense */
    (void)c; (void)name; (void)tag; (void)max_health; (void)attack; (void)defense;
    (void)trigger_event; (void)destroy;
}

bool game_character_is_dead(const GameCharacter *c)
{
    /* TODO */
    (void)c;
    return false;
}

int game_character_take_damage(GameCharacter *c, int attack)
{
    /* TODO: e.g. damage = attack - defense (at least 1), then reduce health */
    (void)c;
    (void)attack;
    return 0;
}
