#ifndef PLAYER_H
#define PLAYER_H

#include "game_character.h"
#include "item.h"

#define MAX_INVENTORY 12

struct Player {
    GameCharacter base; /* Player "inherits" GameCharacter (and so, indirectly, Object) */
    Room *current_room;
    Room *previous_room;
    Item *inventory[MAX_INVENTORY]; /* backpack: pointers to items */
    int inventory_count;
    /* TODO: design the fields for your Hunger System (hunger, thirst, poison) */
};

Player *player_create(const char *name, int max_health, int attack, int defense);
bool player_add_item(Player *p, Item *item);
void player_increase_states(Player *p, int health, int attack, int defense);

/* Change room: remember to update previous_room, it is needed for retreating */
void player_change_room(Player *p, Room *next);

/* Player's version of trigger_event: print the player's status (Showing Status) */
bool player_trigger_event(Object *self, Object *other);

#endif
