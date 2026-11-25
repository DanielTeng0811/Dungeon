# Dungeon (C Version)

> Are you ready for an adventure?

In this assignment you will write a text-based RPG in C. The player explores a dungeon, picks up items, fights monsters, trades with NPCs, and finally defeats the boss guarding the exit.

We provide a template (this repository) with the overall structure and a few helper functions already written. Your job is to complete every part marked `TODO`, and to design your own Hunger System and Room System.

---

## 1. Object-Oriented Programming in C

The original Dungeon assignment was written in C++ using inheritance and virtual functions. C has no classes, but you can achieve the same thing with **structs and pointers**. This is the core idea of the assignment.

### 1.1 Inheritance: put the parent struct as the first member

```c
struct Object {                 // parent "class"
    char name[NAME_LEN];
    ObjectTag tag;
    bool (*trigger_event)(Object *self, Object *other);   // function pointer
    void (*destroy)(Object *self);
};

struct Item {                   // child "class"
    Object base;                // <- must be the FIRST member
    int health, attack, defense;
};
```

The C standard guarantees that the address of a struct is the same as the address of its first member. So an `Item *` can be used as an `Object *`, and the other way around:

```c
Item *sword = item_create("Iron Sword", 0, 12, 0);
Object *obj = &sword->base;      // Item*   -> Object*
Item *again = (Item *)obj;       // Object* -> Item* (only if you know it really is an Item)
```

The inheritance hierarchy (same as the original C++ UML):

```
Object
├── Item
└── GameCharacter
    ├── Player
    ├── Monster
    └── NPC
```

`Player` contains `GameCharacter base`, and `GameCharacter` contains `Object base`, so the player's name is `player->base.base.name`.

### 1.2 Virtual functions: function pointers

`trigger_event` inside `Object` is a **function pointer**. When each kind of object is created, it stores its own version of the function there:

| Object | Function stored | What it does |
|---|---|---|
| Item | `item_trigger_event` | Gets picked up |
| Player | `player_trigger_event` | Shows the status |
| Monster | `monster_trigger_event` | Starts a fight |
| NPC | `npc_trigger_event` | Talks and trades |

The caller does not need to know what kind of object it has:

```c
Object *obj = room->objects[i];               // could be an item, a monster, or an NPC
object_trigger(obj, &player->base.base);      // runs the matching function automatically
```

`item_create` in `item.c` is already written as an example. Write `player_create`, `monster_create`, and `npc_create` the same way.

### 1.3 Rules (breaking these gives 0 points for the whole assignment)

1. `Item`, `Player`, `Monster`, and `NPC` must inherit from `Object` / `GameCharacter` by putting the parent struct as the first member.
2. Interactions with objects in a room must go through `object_trigger()`, i.e. through the function pointer. You **must not** use a big `switch (obj->tag)` or `if` chain to decide which function to call. (Using `tag` to decide whether the menu says "Talk to" or "Pick up" is fine. Using it to choose which interaction code runs is not.)
3. Your report must include a diagram of your structs and explain where you use function pointers.

---

## 2. The Template

| File | Status | Description |
|---|---|---|
| `object.h/.c` | Done | Parent `Object`, `object_trigger()`, `object_destroy()` |
| `io.h/.c` | Done | Input helpers `read_line()` and `read_option()` |
| `rng.h/.c` | Done | Random numbers: `rng_range()`, `rng_chance()` |
| `main.c` | Done | Handles the `--seed` and `--echo` arguments |
| `Makefile` | Done | Run `make` to build |
| `item.h/.c` | Partly done | `item_create` is the example; write `item_trigger_event` |
| `game_character.h/.c` | TODO | |
| `player.h/.c` | TODO | |
| `monster.h/.c` | TODO | |
| `npc.h/.c` | TODO | |
| `room.h/.c` | TODO | |
| `dungeon.h/.c` | TODO | Main game flow |
| `record.h/.c` | Optional | Save / load |

The template is a starting point, not a restriction. You may rename functions, change parameters or return types, and add new files and fields, as long as you follow the rules in 1.3 and meet the requirements below.

### Building and running

```bash
make
./dungeon                 # normal play
./dungeon --seed 42       # fixed seed: random events are the same every run
```

Without `make` (e.g. on Windows):

```bash
gcc -std=c11 -Wall -Wextra -o dungeon *.c
```

Code::Blocks users: create an empty C project and add all the `.c` and `.h` files.

### Rules for input and randomness

So that the TAs can replay your game:

- Read all player input with `read_line()` or `read_option()`. Do not call `scanf` directly.
- Use `rng_range()` / `rng_chance()` for all random events. Do not use `rand()`.
- Do not remove the `--seed` / `--echo` handling in `main.c`.
- When `read_option()` returns `'\0'`, input has ended. Your game must then exit normally instead of looping forever.

---

## 3. Requirements

### 3.1 Basic Functions (70%)

#### Actions Menu

Each turn, list what the player can do and let them choose:

```
What do you want to do?
A. Move
B. Check Status
C. Talk to Merchant
D. Open Backpack
```

#### Movement (10%)

The player can move from one room to another, e.g. from Room 1 to Room 2 and back to Room 1.

```
Where do you want to go?
A. Go up
B. Go down
C. Go left
D. Go right
```

Rooms are linked with `Room *up_room / down_room / left_room / right_room`. A direction with no room is `NULL`.

#### Showing Status (5%)

Print the player's information:

```
Status:
[Player's Name]
> Health: 100/100
> Attack: 30
> Defense: 0
> Items: (if you have)
> (All you want to add...)
```

#### Pick up Items (10%)

The player can pick up items and equip them (increasing their stats).

#### Fighting System (10%)

The player attacks the monster. If the monster is still alive, it fights back. The fight loops until one of them dies or the player retreats.

- **Attack**: hit the monster; it either dies or fights back
- **Retreat**: go back to the previous room

#### NPC (10%)

- Show the NPC's script (what the NPC says)
- Trade with the player (the player can get item(s) from the NPC)

#### Game Logic (10%)

Handle the player winning and losing.

#### Hunger System (7.5%)

The template does not include a design for this. Decide where the data lives and how it is calculated, and explain your design in the report.

**Hunger**
- Keep a value for how hungry the player is.
- It decreases as the player progresses. When it reaches 0, the player starts losing health.
- At least three kinds of food, each restoring a different amount and obtained in a different way (e.g. from animals, NPCs, or monsters).

**Thirst**
- Keep a value for how thirsty the player is.
- It decreases as the player progresses. When it reaches 0, the player starts losing health and gets at least one debuff (a negative effect, e.g. lower attack).
- At least one way to get water (e.g. a lake, an NPC, the environment).

**Poison**
- Keep a value for the poison the player has received.
- While poisoned, the player loses the same amount of health every turn.
- At least two different poisons, with different damage, duration, and source (e.g. monster attacks, bad food or water, the environment).
- At least one way to get milk to cure poison (e.g. from an NPC or an animal).

You may add more statuses and describe them under Optional Enhancement in your report.

#### Room System (7.5%)

Also designed by you. At least three kinds of rooms, each with different effects and different hunger/thirst costs:

**Desert**
- Thirst decreases much faster than in other rooms.
- Occasional sandstorms make both hunger and thirst drop sharply until the sandstorm ends.
- Rarely, the player finds an oasis and can refill thirst.

**Forest**
- Hunger decreases much faster than in other rooms.
- The player may meet dangerous wildlife such as bears and tigers.
- The player may find a lake and refill thirst.

**Swamp**
- Design it yourself.

You may add more room types and describe them under Optional Enhancement in your report.

### 3.2 Map requirement

In both the video and the report, your map must contain every kind of room so you can show all features. For example: a monster room, an NPC room, a chest (item) room, an exit (boss) room, plus Desert, Forest, and Swamp rooms.

### 3.3 Optional Enhancement (10%)

You may add any features you like, as long as the game still works. Describe them clearly in your video and report. Examples:

- MP system
- Cooldown time (CD)
- Type counters (water beats fire, fire beats grass, grass beats water)
- Classes (DPS, tank, ...)
- Save / load (`record.h`)
- Memory management: every `malloc` has a matching `free`, and you explain how you checked it (e.g. Valgrind, AddressSanitizer)

---

## 4. Report (15%)

A PDF that is clear and well organized, explaining how you met each requirement. It must include:

- **Implementation Details**: how each feature is implemented
- **Structure Design**: a diagram of your structs (which struct is embedded in which, which fields are pointers, which are function pointers). This replaces the UML Design section of the original.
- **Results**: screenshots of the game
- **Discussion**
- **Conclusion**

---

## 5. Video (15%)

- No longer than **300 seconds**.
- Show every working feature and explain your structure design.
- Edit the video and add labels to highlight your features.
- **No faking.** Record the video with a fixed seed, and submit:
  - `demo_seed.txt`: the seed used for the recording (a single number)
  - `demo_input.txt`: everything you typed during the recording, one line per input

  The TAs will run `./dungeon --seed <seed> --echo < demo_input.txt` and compare the output with your video. If they do not match, it is treated as faking.

  Tip: write `demo_input.txt` first, check the result with the command above, then record while typing the same inputs.

---

## 6. Grading

| Item | Weight |
|---|---|
| Basic Functions | 70% |
| Optional Enhancement | 10% |
| Report | 15% |
| Video | 15% |
| **Total (maximum)** | **110%** |

- Completing all basic requirements gives the basic score of 70%.
- Extra features can raise your score up to 110%.
- Breaking the rules in 1.3: 0 points.
- There is no live demo; grading is based on the video and report.
- Plagiarism: 0 points.

---

## 7. Submission

Upload one `HW1_<studentID>.zip` to E3 containing:

```
HW1_<studentID>.zip
├── Dungeon_<studentID>/      all your code and resources
│   ├── *.c, *.h
│   ├── Makefile (if any)
│   ├── demo_seed.txt
│   └── demo_input.txt
├── report_<studentID>.pdf
└── demo_<studentID>.mp4
```

- Any missing file: 0 points.
- Wrong submission format: 5% penalty.
- Deadline: **(TBD)**
- Late penalty: 10% per day.

### Self-check before submitting

```bash
gcc -std=c11 -Wall -Wextra -o dungeon *.c
./dungeon --seed $(cat demo_seed.txt) --echo < demo_input.txt
```

If it compiles, runs to the end, and matches your video, you are good.

---

## 8. FAQ

**Q: Why must `Object base` be the first member?**
Only the first member has the same address as the whole struct. If it is anywhere else, `(Item *)obj` points to the wrong place.

**Q: Why can't I use `rand()`?**
`rand()` gives different results on Windows, macOS, and Linux. The same seed would produce a different game on another computer, so the TAs could not replay yours.

**Q: Do I free an item after picking it up?**
No. It just moves from the room to the player's backpack. An item may belong to only one place at a time, otherwise it will be freed twice at the end. Free a monster after it dies, because nothing uses it anymore.

**Q: Can I make a completely different game?**
Yes, but it must still follow the structure rules in 1.3, and your report must explain your structure in detail. It will be checked more strictly than a Dungeon game.

---

## Get Help

- Lab time: (TBD)
- E3 forum: others may have the same question
- E-mail: cc all TAs
