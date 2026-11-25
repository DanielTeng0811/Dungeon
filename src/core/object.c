#include <stdio.h>
#include "object.h"

void object_init(Object *obj, const char *name, ObjectTag tag,
                 bool (*trigger_event)(Object *self, Object *other),
                 void (*destroy)(Object *self))
{
    snprintf(obj->name, sizeof obj->name, "%s", name);
    obj->tag = tag;
    obj->trigger_event = trigger_event;
    obj->destroy = destroy;
}

bool object_trigger(Object *self, Object *other)
{
    if (self == NULL || self->trigger_event == NULL)
        return false;
    return self->trigger_event(self, other);
}

void object_destroy(Object *obj)
{
    if (obj != NULL && obj->destroy != NULL)
        obj->destroy(obj);
}

const char *object_tag_name(ObjectTag tag)
{
    switch (tag) {
    case TAG_ITEM:    return "Item";
    case TAG_PLAYER:  return "Player";
    case TAG_MONSTER: return "Monster";
    case TAG_NPC:     return "NPC";
    }
    return "Unknown";
}
