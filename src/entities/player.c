#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "room.h"

static void player_destroy(Object *self)
{
    /* TODO: free every item in the backpack (object_destroy), then free the player itself */
    (void)self;
}

Player *player_create(const char *name, int max_health, int attack, int defense)
{
    /* TODO: follow item_create in item.c
     *   malloc -> game_character_init(..., player_trigger_event, player_destroy) -> other fields
     */
    (void)name; (void)max_health; (void)attack; (void)defense;
    (void)player_destroy;
    return NULL;
}

bool player_add_item(Player *p, Item *item)
{
    /* TODO: return false if the backpack is full */
    (void)p; (void)item;
    return false;
}

void player_increase_states(Player *p, int health, int attack, int defense)
{
    /* TODO */
    (void)p; (void)health; (void)attack; (void)defense;
}

void player_change_room(Player *p, Room *next)
{
    /* TODO */
    (void)p; (void)next;
}

bool player_trigger_event(Object *self, Object *other)
{
    Player *p = (Player *)self;
    (void)other;

    /* TODO: print the status, for example
     *   Status:
     *   [Alice]
     *   > Health: 100/100
     *   > Attack: 30
     *   > Defense: 0
     *   > Items: Iron Sword, Bread
     *   > (hunger, thirst, poison, current room... anything you want)
     */
    (void)p;
    return false;
}
