# Nuggets
## README
### Team 7 (Ellie Boyd, Jeffrey Liu, Brian Chun Yin Ng, Donia Tung), Fall, 2021

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets. The rooms and passages are defined by a map loaded by the server at the start of the game. The gold nuggets are randomly distributed in piles within the rooms. Up to 26 players, and one spectator, may play a given game. Each player is randomly dropped into a room when joining the game. Players move about, collecting nuggets when they move onto a pile. When all gold nuggets are collected, the game ends and a summary is printed.

## Usage

To build `nuggets`, run `make`
To clean up, run `make clean`
Run server and client by following the specs in the requirements. 


We have individual unit tests for the `grid` and `player` data modules.

gridtest is built and run with the following
```bash
make gridtest
./gridtest
```

playertest is built and run with the following
```bash
make playertest
./playertest
```

`nuggets` includes:

Server:

```c
static void parseArgs(const int argc, char* argv[]);
bool handleMsg(void* arg, const addr_t from, const char* message);
static char** parseMsg(char* msg);
static int countPlayers(player_t** players);
static bool ifEmpty(char* str);
static char* processName(char* name);
static char* getID(int playerIndex);
```

Client:

```c
bool parseArgs(const int argc, char* argv[]);
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
```

Structs:

```c
typedef struct player player_t;
typedef struct tuple tuple_t;
typedef struct grid grid_t;
```

## Files

`grid.c` - the grid module

`grid.h` - header file for grid

`player.c` - the player module

`player.h` - header file for player

`server.c` - the server module

`client.c` - the client module

`DESIGN.md` - design spec for nuggets

`IMPLEMENTATION.md` - implementation spec for nuggets

`Makefile` - the compilation and testing procedures

## Known bugs

* Newlines are not showing up on the map in player mode, so the map does not render correctly
* Gold does not get picked up by the player when the player moves over it
* There is a core dump when the spectator quits (although not when the player quits, which is odd)

## Materials provided

See the support library for some useful modules.

See the maps for some draft maps.