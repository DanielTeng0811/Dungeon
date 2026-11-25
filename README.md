# Final Project: Dungeon

> [!CAUTION]
> **Due: TBD**

In this project you write a text-based RPG in C: the player explores a dungeon, picks up items, fights monsters, trades with NPCs, and defeats the boss at the exit. C has no classes, so you build the object hierarchy with **structs and function pointers**.

This project is an optional bonus. You build the game in three parts, each one on top of the previous one:

| Part | What you build |
|---|---|
| 1 | The player and items in one room |
| 2 | The map, monsters, NPCs, winning and losing |
| 3 | Your own Hunger System and Room System |

Every task is in a green **Implementation** box. Each part ends with a check you can run.

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

The template compiles as-is. Until you implement Part 1, it prints `dungeon_create_player() is not implemented yet.`

### The repository

```text
Dungeon/
├── Makefile
└── src/
    ├── main.c                  handles --seed and --echo (do not change)
    ├── core/                   provided, no changes needed
    │   ├── object.h/.c         the parent Object, object_trigger(), object_destroy()
    │   ├── io.h/.c             read_line(), read_option()
    │   └── rng.h/.c            rng_range(), rng_chance()
    ├── entities/
    │   ├── item.h/.c           Part 1, Part 3 (item_create() is the example)
    │   ├── game_character.h/.c Part 1
    │   ├── player.h/.c         Parts 1–3
    │   ├── monster.h/.c        Part 2
    │   └── npc.h/.c            Part 2
    └── world/
        ├── room.h/.c           Part 1, Part 3
        ├── dungeon.h/.c        Parts 1–3: the game loop and the map
        └── record.h/.c         optional: save / load
```

You may rename functions, change signatures, and add files and fields, as long as you follow the rules below.

### Objects in C

A child struct "inherits" its parent by putting it as the **first member**. A struct has the same address as its first member, so an `Item *` can be used as an `Object *` and back:

```c
struct Item {
    Object base;              /* must be the FIRST member */
    int health, attack, defense;
};

Item *sword = item_create("Iron Sword", 0, 12, 0);
Object *obj = &sword->base;    /* Item*   -> Object* */
Item *same  = (Item *)obj;     /* Object* -> Item*, only if it really is an Item */
```

```text
Object                    trigger_event does:
├── Item                  get picked up
└── GameCharacter
    ├── Player            show the status
    ├── Monster           start a fight
    └── NPC               talk and trade
```

`Object` holds a function pointer `trigger_event`. Each `*_create()` stores its own function there, so the caller does not need to know what the object is:

```c
object_trigger(room->objects[i], &player->base.base);   /* runs the right trigger_event */
```

> [!IMPORTANT]
> **Rules.** A project that breaks them is not graded.
> 1. `Item`, `Player`, `Monster` and `NPC` inherit from `Object` / `GameCharacter` by putting the parent struct as the first member.
> 2. Interactions with objects in a room go through `object_trigger()`. Do not use a `switch (obj->tag)` or an `if` chain to pick the interaction code. Using `tag` only for menu text ("Talk to" or "Pick up") is fine.
> 3. Read all input with `read_line()` / `read_option()` (not `scanf`), and all randomness with `rng_range()` / `rng_chance()` (not `rand()`), so that the TAs can replay your game. When `read_option()` returns `'\0'`, the input has ended: exit the game.

---

## Part 1: The Player and Items

Build a game with **one room** that contains a few items. The player can check their status and pick the items up.

> [!TIP]
> **Implementation**
>
> - `src/entities/game_character.c`: `game_character_init()`, `game_character_is_dead()`, `game_character_take_damage()`.
> - `src/entities/player.c`: `player_create()` (follow `item_create()` in `item.c`), the backpack, and `player_trigger_event()`, which prints the status.
> - `src/entities/item.c`: `item_trigger_event()`: the player picks the item up and gains its stats.
> - `src/world/room.c`: `room_add_object()`, `room_pop_object()`, `room_free_objects()`.
> - `src/world/dungeon.c`:
>   - `dungeon_create_player()`: read the name with `read_line()`.
>   - `dungeon_create_map()`: one room with at least two items.
>   - `dungeon_choose_action()`: the Actions Menu below.
>   - `dungeon_handle_event()`: call `object_trigger()`, and remove the item from the room if it returns `true`.

The Actions Menu lists what the player can do in the current room:

```text
What do you want to do?
A. Check Status
B. Pick up Iron Sword
C. Pick up Wooden Shield
```

The status shows at least the name, health, attack, defense and items:

```text
Status:
Alice
> Health: 100/100
> Attack: 22
> Defense: 0
> Items: Iron Sword
```

**Check:** check the status, pick up an item, then check again: the item is listed and the stats went up. It must no longer appear in the menu. Press `Ctrl+D` (`Ctrl+Z` then Enter on Windows): the game exits.

---

## Part 2: The Dungeon

Add more rooms, monsters and NPCs, and let the player win or lose.

> [!TIP]
> **Implementation**
>
> - `src/world/dungeon.c`:
>   - `dungeon_create_map()`: several rooms linked up / down / left / right (link both ways; `NULL` means no room that way), and an exit room with a boss.
>   - `dungeon_handle_movement()`: list the directions that have a room, and move with `player_change_room()` (in `player.c`; it also remembers the previous room for retreating).
>   - `dungeon_check_game_logic()`: `GAME_LOSE` when the player dies, `GAME_WIN` when the boss is defeated.
>   - `dungeon_run()`: print a win or lose screen.
> - `src/entities/monster.c`: `monster_create()` and `monster_trigger_event()`, the fight loop: **Attack** hits the monster, which fights back if it survives, until one of them dies; **Retreat** goes back to the previous room. A dead monster is removed from the room and freed.
> - `src/entities/npc.c`: `npc_create()`, the commodities, and `npc_trigger_event()`: show the NPC's script and let the player get an item from it. Whether and how the player pays is up to you.

```text
What do you want to do?
A. Move
B. Check Status
C. Fight Wolf
```

```text
Where do you want to go?
A. Go up
B. Go left
```

**Check:** walk to every room and back, retreat from a fight, kill a monster (it is gone when you come back), get an item from an NPC, then play once until you win and once until you lose.

---

## Part 3: Hunger and Room Systems

The template has no code for this part: you decide which fields to add (to `Player`, `Item`, `Room`, or new structs) and when they change. Explain your design in the report.

> [!TIP]
> **Implementation: Hunger System**
>
> - **Hunger** decreases as the player moves. At 0 the player loses health. At least **three foods**, with different amounts and sources (animals, NPCs, monsters, ...).
> - **Thirst** decreases as the player moves. At 0 the player loses health and gets at least one debuff (e.g. lower attack). At least **one water source**.
> - **Poison** costs the same health every turn while it lasts. At least **two poisons** with different damage, duration and source. At least **one way to get milk**, which cures poison.
> - Show hunger, thirst and poison in the status.

> [!TIP]
> **Implementation: Room System**
>
> At least three room types, each with its own effects and hunger / thirst cost:
>
> | Room | Requirement |
> |---|---|
> | Desert | Thirst drops much faster. Occasional sandstorms drain hunger and thirst until they end. Rarely, an oasis refills thirst. |
> | Forest | Hunger drops much faster. Wild animals (bears, tigers) may appear. A lake refills thirst. |
> | Swamp | Your own design. |
>
> Your final map contains every room type, plus a monster, an NPC, an item, and the exit with the boss.

Use `rng_range()` / `rng_chance()` for every random event (sandstorms, animals, ...).

**Check:** with a fixed `--seed`, starve, dehydrate and get poisoned once each, and cure the poison with milk. Visit each room type and see its effect.

### Optional enhancements

Anything that keeps the game working, for example: MP, cooldowns, type counters, classes, save / load (`record.h`), or no memory leaks checked with Valgrind or AddressSanitizer. Describe them in the report and the video.

---

## Report and Video

**Report (PDF):**

- How you implemented each part.
- A diagram of your structs: which struct is embedded in which, which fields are pointers, and which are function pointers.
- Your Hunger System and Room System design.
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

**Can I make a different game?** Yes, if it follows the rules and covers the requirements of Parts 1–3.
