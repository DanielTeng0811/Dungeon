#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "npc.h"
#include "player.h"

static void npc_destroy(Object *self)
{
    /* TODO: remember to free the commodities */
    (void)self;
}

NPC *npc_create(const char *name, const char *script)
{
    /* TODO: follow item_create */
    (void)name; (void)script;
    (void)npc_destroy;
    return NULL;
}

bool npc_add_commodity(NPC *npc, Item *item)
{
    /* TODO */
    (void)npc; (void)item;
    return false;
}

void npc_list_commodity(const NPC *npc)
{
    /* TODO */
    (void)npc;
}

bool npc_trigger_event(Object *self, Object *other)
{
    NPC *npc = (NPC *)self;
    Player *p = (Player *)other;

    /* TODO: print the script, list the commodities, let the player pick one
     * Note: one Item must not belong to both the NPC and the player, or it will be
     * freed twice and crash. Either move it from the NPC to the player, or give the
     * player a new copy.
     */
    (void)npc; (void)p;
    return false;
}
