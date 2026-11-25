# Final Project: Dungeon in C

> [!CAUTION]
> **Due: TBD**

In this project you write a text-based RPG in C: the player explores a dungeon, picks up items, fights monsters, trades with NPCs, and defeats the boss at the exit. C has no classes, so you build the object hierarchy with **structs and function pointers**.

This project is an optional bonus.

## Setup

You need a C compiler (`gcc` or `clang`) and `make`.

```bash
git clone https://github.com/DanielTeng0811/Dungeon.git
cd Dungeon
make
./dungeon                 # play
./dungeon --seed 42       # fixed seed: the random events are the same every run
```

Without `make`:

```bash
gcc -std=c11 -Wall -Wextra -Isrc/core -Isrc/entities -Isrc/world -o dungeon src/*.c src/*/*.c
```

The template compiles as-is. Until you implement it, it prints `dungeon_create_player() is not implemented yet.`

### The repository

```text
Dungeon/
├── Makefile
└── src/
    ├── main.c                 handles --seed and --echo (do not change)
    ├── core/                  provided, no changes needed
    │   ├── object.h/.c        the parent Object, object_trigger(), object_destroy()
    │   ├── io.h/.c            read_line(), read_option()
    │   └── rng.h/.c           rng_range(), rng_chance()
    ├── entities/
    │   ├── item.h/.c          ← item_create() is the example; you write item_trigger_event()
    │   ├── game_character.h/.c ← TODO
    │   ├── player.h/.c        ← TODO
    │   ├── monster.h/.c       ← TODO
    │   └── npc.h/.c           ← TODO
    └── world/
        ├── room.h/.c          ← TODO
        ├── dungeon.h/.c       ← TODO: the main game loop
        └── record.h/.c        optional: save / load
```

You may rename functions, change signatures, and add files and fields, as long as you follow the rules below.

---

## Part 1: Object-Oriented Programming in C

### 1.1 Inheritance: the parent struct is the first member

```c
struct Object {
    char name[NAME_LEN];
    ObjectTag tag;
    bool (*trigger_event)(Object *self, Object *other);   /* "virtual function" */
    void (*destroy)(Object *self);
};

struct Item {
    Object base;              /* must be the FIRST member */
    int health, attack, defense;
};
```

A struct has the same address as its first member, so an `Item *` can be used as an `Object *` and back:

```c
Item *sword = item_create("Iron Sword", 0, 12, 0);
Object *obj = &sword->base;    /* Item*   -> Object* */
Item *same  = (Item *)obj;     /* Object* -> Item*, only if it really is an Item */
```

```text
Object
├── Item
└── GameCharacter
    ├── Player
    ├── Monster
    └── NPC
```

### 1.2 Virtual functions: function pointers

Each `*_create()` stores its own function in `trigger_event`:

| Object | `trigger_event` | What it does |
|---|---|---|
| Item | `item_trigger_event` | gets picked up |
| Player | `player_trigger_event` | shows the status |
| Monster | `monster_trigger_event` | starts a fight |
| NPC | `npc_trigger_event` | talks and trades |

The caller does not need to know what the object is:

```c
object_trigger(room->objects[i], &player->base.base);
```

Read `src/entities/item.c`, then write `player_create`, `monster_create` and `npc_create` the same way.

> [!IMPORTANT]
> **Rules.** A project that breaks them is not graded.
> 1. `Item`, `Player`, `Monster` and `NPC` inherit from `Object` / `GameCharacter` by putting the parent struct as the first member.
> 2. Interactions with objects in a room go through `object_trigger()`. Do not use a `switch (obj->tag)` or an `if` chain to pick the interaction code. Using `tag` only for menu text ("Talk to" or "Pick up") is fine.
> 3. Read all input with `read_line()` / `read_option()` (not `scanf`), and all randomness with `rng_range()` / `rng_chance()` (not `rand()`), so that the TAs can replay your game. When `read_option()` returns `'\0'`, the input has ended: exit the game.

---

## Part 2: The Game

### 2.1 Basic functions

Each turn, list the actions available in the current room and let the player choose one:

```text
What do you want to do?
A. Move
B. Check Status
C. Talk to Merchant
D. Open Backpack
```

| Feature | Requirement |
|---|---|
| Movement | Move between rooms up / down / left / right (`NULL` means no room that way). |
| Status | Show the player's name, health, attack, defense and items. |
| Items | Pick up items and equip them to raise the player's stats. |
| Fighting | Attack until the monster or the player dies, or retreat to the previous room. |
| NPC | Show the NPC's script, and let the player get items from it. |
| Game logic | The player can win (beat the boss) and lose (die). |

### 2.2 Hunger system

The template has no code for this. Design where the data lives and how it changes.

- **Hunger** decreases as the player moves. At 0 the player loses health. At least **three foods**, with different amounts and sources (animals, NPCs, monsters, ...).
- **Thirst** decreases as the player moves. At 0 the player loses health and gets at least one debuff (e.g. lower attack). At least **one water source**.
- **Poison** costs the same health every turn while it lasts. At least **two poisons** with different damage, duration and source. At least **one way to get milk**, which cures poison.

### 2.3 Room system

At least three room types, each with its own effects and hunger / thirst cost:

| Room | Requirement |
|---|---|
| Desert | Thirst drops much faster. Occasional sandstorms drain hunger and thirst until they end. Rarely, an oasis refills thirst. |
| Forest | Hunger drops much faster. Wild animals (bears, tigers) may appear. A lake refills thirst. |
| Swamp | Your own design. |

Your map must contain every room type, plus a monster, an NPC, an item, and the exit with the boss.

### 2.4 Optional enhancements

Anything that keeps the game working, for example: MP, cooldowns, type counters, classes, save / load (`record.h`), or no memory leaks checked with Valgrind or AddressSanitizer. Describe them in the report and the video.

---

## Part 3: Report and Video

**Report (PDF):**

- How you implemented each feature.
- A diagram of your structs: which struct is embedded in which, which fields are pointers, and which are function pointers.
- Screenshots of the game.

**Video:** at most 300 seconds. Show every feature you implemented and explain your struct design.

Record the video with a fixed seed, and submit what you typed:

- `demo_seed.txt`: the seed (one number).
- `demo_input.txt`: every input you typed, one per line.

The TAs run the command below and compare its output with your video:

```bash
./dungeon --seed $(cat demo_seed.txt) --echo < demo_input.txt
```

Write `demo_input.txt` first, check it with this command, then record while typing the same inputs.

---

## Submission

Upload one zip file to E3:

```text
FP_<student_id>.zip
├── Dungeon_<student_id>/
│   ├── Makefile
│   ├── src/
│   ├── demo_seed.txt
│   └── demo_input.txt
├── report_<student_id>.pdf
└── demo_<student_id>.mp4
```

Before you submit, check that it builds and replays from a clean copy:

```bash
make clean && make
./dungeon --seed $(cat demo_seed.txt) --echo < demo_input.txt
```

## FAQ

**Do I free an item after picking it up?** No. It moves from the room to the backpack, and must belong to only one place, or it is freed twice. Free a monster after it dies.

**Can I make a different game?** Yes, if it follows the rules in Part 1 and covers the requirements in Part 2.
