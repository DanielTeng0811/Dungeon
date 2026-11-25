#include <stdio.h>
#include "room.h"

void room_init(Room *room, int index, bool is_exit)
{
    room->up_room = room->down_room = room->left_room = room->right_room = NULL;
    room->is_exit = is_exit;
    room->index = index;
    room->object_count = 0;
}

bool room_add_object(Room *room, Object *obj)
{
    /* TODO: return false if the room is full */
    (void)room; (void)obj;
    return false;
}

bool room_pop_object(Room *room, Object *obj)
{
    /* TODO: find obj and shift the following ones forward */
    (void)room; (void)obj;
    return false;
}

void room_free_objects(Room *room)
{
    /* TODO: call object_destroy on each object */
    (void)room;
}
