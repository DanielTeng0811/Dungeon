#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#define NAME_LEN 32

/* Declare all type names first, so headers can use each other's pointers without circular includes */
typedef struct Object Object;
typedef struct Item Item;
typedef struct GameCharacter GameCharacter;
typedef struct Player Player;
typedef struct Monster Monster;
typedef struct NPC NPC;
typedef struct Room Room;
typedef struct Dungeon Dungeon;

typedef enum { TAG_ITEM, TAG_PLAYER, TAG_MONSTER, TAG_NPC } ObjectTag;

/*
 * The "parent class" of every game object.
 * Each child (Item, Player, Monster, NPC) puts Object as the FIRST member of its struct,
 * so an Object* and an Item* point to the same address and can be cast to each other.
 *
 * trigger_event is a function pointer, the C version of a C++ virtual function:
 * each child stores its own version when it is created, and object_trigger() runs that one.
 */
struct Object {
    char name[NAME_LEN];
    ObjectTag tag;
    bool (*trigger_event)(Object *self, Object *other);
    void (*destroy)(Object *self);
};

void object_init(Object *obj, const char *name, ObjectTag tag,
                 bool (*trigger_event)(Object *self, Object *other),
                 void (*destroy)(Object *self));

/* Polymorphic call: which function runs is decided by self->trigger_event */
bool object_trigger(Object *self, Object *other);
void object_destroy(Object *obj);
const char *object_tag_name(ObjectTag tag);

#endif
