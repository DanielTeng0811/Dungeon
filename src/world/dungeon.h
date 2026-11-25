#ifndef DUNGEON_H
#define DUNGEON_H

#include "object.h"
#include "player.h"
#include "room.h"

#define MAX_ROOMS 16

typedef enum { GAME_CONTINUE, GAME_WIN, GAME_LOSE, GAME_QUIT } GameState;

struct Dungeon {
    Player *player;
    Room rooms[MAX_ROOMS]; /* Fixed-size array: room addresses never change, so Room* links stay valid */
    int room_count;
    bool quit;
};

void dungeon_init(Dungeon *d);

/* Create the player with starting stats */
void dungeon_create_player(Dungeon *d);

/* Create the map: rooms, links between rooms, objects placed in rooms */
void dungeon_create_map(Dungeon *d);

/* Handle movement (up / down / left / right) */
void dungeon_handle_movement(Dungeon *d);

/* Interact with an object in the room: call object_trigger and handle its return value */
void dungeon_handle_event(Dungeon *d, Object *obj);

/* Game setup: create the player and the map */
void dungeon_start_game(Dungeon *d);

/* Show the actions available in this room (Actions Menu), read the choice, and run it */
void dungeon_choose_action(Dungeon *d);

/* Check whether the player has won or lost */
GameState dungeon_check_game_logic(Dungeon *d);

/* The whole game loop */
void dungeon_run(Dungeon *d);

/* Free everything that was malloc'ed */
void dungeon_free(Dungeon *d);

#endif
