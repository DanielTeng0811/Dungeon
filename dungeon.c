#include <stdio.h>
#include <string.h>
#include "dungeon.h"
#include "io.h"
#include "item.h"
#include "monster.h"
#include "npc.h"

void dungeon_init(Dungeon *d)
{
    d->player = NULL;
    d->room_count = 0;
    d->quit = false;
}

void dungeon_create_player(Dungeon *d)
{
    /* TODO: read the player's name (read_line) and create the player with player_create */
    (void)d;
}

void dungeon_create_map(Dungeon *d)
{
    /* TODO:
     *   1. room_init every room
     *   2. set up_room / down_room / left_room / right_room (remember to link both ways)
     *   3. place monsters, NPCs, and items with room_add_object, e.g.
     *        Monster *wolf = monster_create("Wolf", 40, 9, 2);
     *        room_add_object(&d->rooms[2], &wolf->base.base);
     *   4. you need at least: a monster room, an NPC room, a chest (item) room,
     *      an exit (boss) room, and Desert, Forest, and Swamp rooms
     */
    (void)d;
}

void dungeon_handle_movement(Dungeon *d)
{
    /* TODO: print the available directions, read the choice, change room (player_change_room) */
    (void)d;
}

void dungeon_handle_event(Dungeon *d, Object *obj)
{
    /* TODO:
     *   bool done = object_trigger(obj, &d->player->base.base);   // polymorphic call
     *   If done is true, remove obj from the room; if it is a monster, also object_destroy it.
     *   Careful: after a retreat player->current_room has changed, so remember the room first.
     */
    (void)d; (void)obj;
}

void dungeon_start_game(Dungeon *d)
{
    dungeon_create_player(d);
    dungeon_create_map(d);
    if (d->player != NULL && d->room_count > 0)
        d->player->current_room = &d->rooms[0];
}

void dungeon_choose_action(Dungeon *d)
{
    /* TODO: Actions Menu
     *   What do you want to do?
     *   A. Move
     *   B. Check Status
     *   C. Talk to Merchant / Pick up Iron Sword / ... (depends on the objects in the room)
     *   ...
     * Read with read_option(). If it returns '\0', input has ended: just return.
     */
    (void)d;
}

GameState dungeon_check_game_logic(Dungeon *d)
{
    /* TODO: player dead -> GAME_LOSE; boss defeated / exit reached -> GAME_WIN */
    if (d->quit || io_eof())
        return GAME_QUIT;
    return GAME_CONTINUE;
}

void dungeon_run(Dungeon *d)
{
    dungeon_start_game(d);
    if (d->player == NULL) {
        printf("dungeon_create_player() is not implemented yet.\n");
        return;
    }

    GameState state;
    while ((state = dungeon_check_game_logic(d)) == GAME_CONTINUE)
        dungeon_choose_action(d);

    /* TODO: print a win / lose screen depending on state */
}

void dungeon_free(Dungeon *d)
{
    for (int i = 0; i < d->room_count; i++)
        room_free_objects(&d->rooms[i]);
    if (d->player != NULL)
        object_destroy(&d->player->base.base);
    d->player = NULL;
}
