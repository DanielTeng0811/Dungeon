#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "player.h"

/*
 * item_create / item_destroy in this file are already written. They show how to do
 * inheritance in C:
 *   1. malloc an Item
 *   2. fill in the parent Object part with object_init, passing Item's own functions
 *   3. fill in Item's own fields
 * Write the create functions for Player, Monster, and NPC the same way.
 */

static void item_destroy(Object *self)
{
    free(self); /* Object is the first member, so self is the address malloc returned */
}

Item *item_create(const char *name, int health, int attack, int defense)
{
    Item *item = malloc(sizeof *item);
    if (item == NULL)
        return NULL;
    object_init(&item->base, name, TAG_ITEM, item_trigger_event, item_destroy);
    item->health = health;
    item->attack = attack;
    item->defense = defense;
    return item;
}

bool item_trigger_event(Object *self, Object *other)
{
    Item *item = (Item *)self;     /* OK because Object is the first member */
    Player *player = (Player *)other;

    /* TODO: pick up the item
     *   - put it in the player's backpack (player_add_item)
     *   - add its stats (player_increase_states)
     *   - print a message so the player knows what they got
     */
    (void)item;
    (void)player;
    return false;
}
