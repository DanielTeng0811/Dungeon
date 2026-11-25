#ifndef ITEM_H
#define ITEM_H

#include "object.h"

struct Item {
    Object base; /* Must be the FIRST member: Item "inherits" Object */
    int health, attack, defense;
    /* TODO: add fields for your Hunger System (e.g. how much hunger/thirst it restores, whether it is milk...) */
};

/* Create an item (already written; use it as an example) */
Item *item_create(const char *name, int health, int attack, int defense);

/*
 * Item's version of trigger_event: being picked up.
 * self is this item, other is the Player picking it up.
 * Add the item's stats to the player, or put it in the player's backpack.
 * Return true if the player now owns the item and the room should remove it.
 */
bool item_trigger_event(Object *self, Object *other);

#endif
