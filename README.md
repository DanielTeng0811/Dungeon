# Final Project: Dungeon

**Release: Monday, December 14, 2026, 13:30 · Due: Friday, December 25, 2026, 23:59** (tentative)

In this project you write a text-based RPG in C: the player explores a dungeon, picks up items, fights monsters, trades with NPCs, and defeats the boss at the exit. C has no classes, so you build the object hierarchy with **structs and function pointers**.

This project is an optional bonus. You build the basic game in three parts, each one on top of the previous one, then add your own features:

| Part | What you build |
|---|---|
| 1 | The player and items in one room |
| 2 | The map, monsters, NPCs, winning and losing |
| 3 | Your own Hunger System and Room System |
| Extra | Features you design yourself |

The basic part (80%) is Parts 1–3 with the report and the video. Extra features are 20%.

Each part lists what to **implement**, a **check** you can run, and **questions** to answer in your report. Answer from your own code: name the functions and fields you wrote.

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

### Rules

A project that breaks these rules is not graded.

1. `Item`, `Player`, `Monster` and `NPC` inherit from `Object` / `GameCharacter` by putting the parent struct as the first member.
2. Interactions with objects in a room go through `object_trigger()`. Do not use a `switch (obj->tag)` or an `if` chain to pick the interaction code. Using `tag` only for menu text ("Talk to" or "Pick up") is fine.
3. Read all input with `read_line()` / `read_option()` (not `scanf`), and all randomness with `rng_range()` / `rng_chance()` (not `rand()`), so that the TAs can replay your game. When `read_option()` returns `'\0'`, the input has ended: exit the game.

---

## Part 1: The Player and Items

Build a game with **one room** that contains a few items. The player can check their status and pick the items up.

### Implementation

- `src/entities/game_character.c`: `game_character_init()`, `game_character_is_dead()`, `game_character_take_damage()`.
- `src/entities/player.c`: `player_create()` (follow `item_create()` in `item.c`), the backpack, and `player_trigger_event()`, which prints the status.
- `src/entities/item.c`: `item_trigger_event()`: the player picks the item up and gains its stats.
- `src/world/room.c`: `room_add_object()`, `room_pop_object()`, `room_free_objects()`.
- `src/world/dungeon.c`:
  - `dungeon_create_player()`: read the name with `read_line()`.
  - `dungeon_create_map()`: one room with at least two items.
  - `dungeon_choose_action()`: the Actions Menu below.
  - `dungeon_handle_event()`: call `object_trigger()`, and remove the item from the room if it returns `true`.

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

### Questions

- **Q1.1** Draw how a `Player` is laid out in memory (`Player` → `GameCharacter` → `Object`). With a `Player *p`, how do you reach the player's name? Why would `(Item *)obj` give a wrong pointer if `base` were not the first member of `Item`?
- **Q1.2** The player chooses "Pick up Iron Sword". List the functions that run, in order, from `dungeon_choose_action()` to the line that changes the player's stats. How does the program know to call `item_trigger_event()` and not another function?
- **Q1.3** After an item is picked up, which pointers point to it? Where in your code is it freed when the game ends?

---

## Part 2: The Dungeon

Add more rooms, monsters and NPCs, and let the player win or lose.

### Implementation

- `src/world/dungeon.c`:
  - `dungeon_create_map()`: several rooms linked up / down / left / right (link both ways; `NULL` means no room that way), and an exit room with a boss.
  - `dungeon_handle_movement()`: list the directions that have a room, and move with `player_change_room()` (in `player.c`; it also remembers the previous room for retreating).
  - `dungeon_check_game_logic()`: `GAME_LOSE` when the player dies, `GAME_WIN` when the boss is defeated.
  - `dungeon_run()`: print a win or lose screen.
- `src/entities/monster.c`: `monster_create()` and `monster_trigger_event()`, the fight loop: **Attack** hits the monster, which fights back if it survives, until one of them dies; **Retreat** goes back to the previous room. A dead monster is removed from the room and freed.
- `src/entities/npc.c`: `npc_create()`, the commodities, and `npc_trigger_event()`: show the NPC's script and let the player get an item from it. Whether and how the player pays is up to you.

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

### Questions

- **Q2.1** The rooms are in a fixed array `Room rooms[MAX_ROOMS]` and linked with `Room *` pointers. What would go wrong if they were in an array enlarged with `realloc()` while the map is built?
- **Q2.2** When a monster dies, where and in which order do you remove it from the room and free it? What happens if the order is reversed, or one of the two steps is missing?
- **Q2.3** You want to add a new kind of object, e.g. a Chest that gives a random item. Which files do you add or change? Does the menu code in `dungeon.c` need to change? Explain why, using function pointers.

---

## Part 3: Hunger and Room Systems

The template has no code for this part: you decide which fields to add (to `Player`, `Item`, `Room`, or new structs) and when they change. Explain your design in the report.

### Implementation: Hunger System

- **Hunger** decreases as the player moves. At 0 the player loses health. At least **three foods**, with different amounts and sources (animals, NPCs, monsters, ...).
- **Thirst** decreases as the player moves. At 0 the player loses health and gets at least one debuff (e.g. lower attack). At least **one water source**.
- **Poison** costs the same health every turn while it lasts. At least **two poisons** with different damage, duration and source. At least **one way to get milk**, which cures poison.
- Show hunger, thirst and poison in the status.

### Implementation: Room System

At least three room types, each with its own effects and hunger / thirst cost:

| Room | Requirement |
|---|---|
| Desert | Thirst drops much faster. Occasional sandstorms drain hunger and thirst until they end. Rarely, an oasis refills thirst. |
| Forest | Hunger drops much faster. Wild animals (bears, tigers) may appear. A lake refills thirst. |
| Swamp | Your own design. |

Your final map contains every room type, plus a monster, an NPC, an item, and the exit with the boss.

Use `rng_range()` / `rng_chance()` for every random event (sandstorms, animals, ...).

**Check:** with a fixed `--seed`, starve, dehydrate and get poisoned once each, and cure the poison with milk. Visit each room type and see its effect.

### Questions

- **Q3.1** Which structs and fields hold hunger, thirst and poison? Why did you put them there and not in another struct?
- **Q3.2** List what happens during one turn, in order (for example: move, room effect, hunger and thirst, poison, death check). Give one case where a different order would give a different result.
- **Q3.3** Pick one random event. Which `rng_*` call decides it, with what probability? How did you use `--seed` to test it?

---

## Extra Features (20%)

After Parts 1–3 work, add features of your own design. They must keep the basic game working and follow the rules above. For example: MP and skills, cooldowns, type counters, classes, shops and gold, save / load (`record.h`), or no memory leaks checked with Valgrind or AddressSanitizer.

### Questions

For each feature: what it does, which structs and functions you added or changed, and at what time it appears in your video.

---

## AI Collaboration

### Questions

- **AI.1** Which AI tools did you use, and for what (design, writing code, debugging, the report)? If you did not use AI, say so and answer AI.2 and AI.3 about your own work.
- **AI.2** Describe one time the AI's answer was wrong, or did not fit the template or the rules. How did you notice, and how did you fix it?
- **AI.3** What was the hardest problem you met in this project, and how did you solve it?
- **AI.4** Rate your collaboration with AI from 1 to 10, and explain your score.

## Report and Video

### Report format

- One PDF, at most **10 pages**: A4, 11-pt font or larger. Screenshots count toward the pages.
- Write in English or Chinese.
- Label every answer with its question number (Q1.1, Q1.2, ..., AI.1, ...), so the grader can find it.
- Answer from your own code: name the structs, fields and functions you wrote. Short code excerpts are fine; do not paste whole files.
- Add a screenshot where it shows something the text cannot, e.g. the status with hunger and poison, or a sandstorm.

The skeleton below is only a suggestion; you may organize the report your own way.

```markdown
# Dungeon Report: [student ID] [name]

## 1. Overview
[Map of your dungeon: the rooms, their types, and what is in each one.]
[A table of the features you implemented, Basic and Extra.]

## 2. Part 1: The Player and Items
### Struct diagram
[Which struct is embedded in which, which fields are pointers, which are function pointers.]
### Q1.1  ### Q1.2  ### Q1.3

## 3. Part 2: The Dungeon
### Q2.1  ### Q2.2  ### Q2.3

## 4. Part 3: Hunger and Room Systems
### Design
[The rules of your Hunger System and of each room type: the numbers, and when they apply.]
### Q3.1  ### Q3.2  ### Q3.3

## 5. Extra Features
### [Feature name]   (one subsection per feature: what it does, structs and functions, video time)

## 6. AI Collaboration
### AI.1 Tools  ### AI.2 A wrong answer  ### AI.3 Hardest problem  ### AI.4 Rating: [1–10]
```

### Video

At most 300 seconds. Show every feature you implemented and explain your struct design.

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
