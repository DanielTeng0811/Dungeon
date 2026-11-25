#ifndef ROOM_H
#define ROOM_H

#include "object.h"

#define MAX_OBJECTS 8

struct Room {
    Room *up_room;    /* NULL if there is no room that way */
    Room *down_room;
    Room *left_room;
    Room *right_room;
    bool is_exit;
    int index;
    Object *objects[MAX_OBJECTS]; /* monsters, NPCs, items in this room (all stored as Object*) */
    int object_count;
    /* TODO: design the fields for your Room System (desert, forest, swamp) */
};

void room_init(Room *room, int index, bool is_exit);
bool room_add_object(Room *room, Object *obj);

/* Remove an object from the room (after an interaction, e.g. a monster died or an item was picked up) */
bool room_pop_object(Room *room, Object *obj);

/* Free every object in the room */
void room_free_objects(Room *room);

#endif
