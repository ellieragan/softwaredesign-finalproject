/*
* player.c
* Module for the player data structure 
* 
*
* Team 7: Gitting an A
* (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
* CS 50, Fall 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include "grid.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct player{
    char* realName; 
    char* ID; 
    grid_t* grid; 
    tuple_t* currentPos; 
    int gold; 
    bool spectator; 
} player_t; 

typedef struct tuple{
    int x; 
    int y; 
} tuple_t; 

/**************** global functions ****************/
/* that is, visible outside this file */
/* see player.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
// TODO: init here as necessary 

/**************** getRealName ****************/
char* getRealName(player_t* player) { return player->realName; }

/**************** setRealName ****************/
void setRealName(player_t* player, char* realName) { player->realName = realName; }

/**************** getID ****************/
char* getID(player_t* player) { return player->ID; }

/**************** setID ****************/
void setID(player_t* player, char* ID) { player->ID = ID; }

/**************** getGrid ****************/
grid_t* getGrid(player_t* player) { return player->grid; }

/**************** getGrid ****************/
void getGrid(player_t* player, grid_t* grid) { player->grid = grid; }

/**************** getCurrentPos ****************/
tuple_t* getCurrentPos(player_t* player) { return player->currentPosition; }

/**************** setCurrentPos ****************/
void setCurrentPos(player_t* player, tuple_t* currentPosition) { player->currentPosition = currentPosition; } 

/**************** getGold ****************/
int getGold(player_t* player) { return player->gold; }

/**************** setGold ****************/
void setGold(player_t* player, int gold) { player->gold = gold; }

/**************** getSpectatorStatus ****************/
bool getSpectatorStatus(player_t* player) { return player->spectator; }

/**************** setSpectatorStatus ****************/
void setSpectatorStatus(player_t* player, bool spectator) { player->spectator = spectator; }
