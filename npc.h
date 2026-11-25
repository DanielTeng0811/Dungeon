#ifndef NPC_H
#define NPC_H

#include "game_character.h"
#include "item.h"

#define MAX_COMMODITY 8
#define SCRIPT_LEN 256

struct NPC {
    GameCharacter base; /* NPC "inherits" GameCharacter */
    char script[SCRIPT_LEN];        /* what the NPC says */
    Item *commodity[MAX_COMMODITY]; /* items the NPC can give the player */
    int commodity_count;
};

NPC *npc_create(const char *name, const char *script);
bool npc_add_commodity(NPC *npc, Item *item);
void npc_list_commodity(const NPC *npc); /* print all commodities */

/*
 * NPC's version of trigger_event: say the script + trade.
 * The player must be able to get items from the NPC (whether and how they pay is up to you).
 */
bool npc_trigger_event(Object *self, Object *other);

#endif
