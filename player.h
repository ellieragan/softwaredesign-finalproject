/*
* player.h
* declaration & header file for player struct
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

/**************** global types ****************/
typedef struct player player_t; // opaque to users of the module 
typedef struct tuple tuple_t; // TODO: this could also be defined in grid, which might make more sense 

/**************** functions ****************/

/**************** initPlayer ****************/
/*
* 
* 
*/
void initPlayer(char* realName, char* ID, grid_t* grid, tuple_t* currentPos, bool spectator); 

/**************** addGold ****************/
/*
* 
* 
*/
int addGold(player_t* player, int goldCollected, int* remainingGold);

/**************** movePlayer ****************/
/*
* 
* 
*/
grid_t* movePlayer(player_t* player, char* keyPressed, player_t** otherPlayers); 

/**************** updateSpectator ****************/
/*
* 
* 
*/
grid_t* updateSpectator(player_t* player, char* keyPressed, player_t** otherPlayers, player_t* spectator); 

/**************** deletePlayer ****************/
/*
* 
* 
*/
void deletePlayer(player_t* player); 


/**************** getters and setters ****************/

/**************** getRealName ****************/
/*
* 
* 
*/
char* getRealName(player_t* player); 

/**************** setRealName ****************/
/*
* 
* 
*/
void setRealName(player_t* player, char* realName); 

/**************** getID ****************/
/*
* 
* 
*/
char* getID(player_t* player); 

/**************** setID ****************/
/*
* 
* 
*/
void setID(player_t* player, char* ID); 

/**************** getGrid ****************/
/*
* 
* 
*/
grid_t* getGrid(player_t* player); 

/**************** getGrid ****************/
/*
* 
* 
*/
void getGrid(player_t* player, grid_t* grid); 

/**************** getCurrentPos ****************/
/*
* 
* 
*/
tuple_t* getCurrentPos(player_t* player); 

/**************** setCurrentPos ****************/
/*
* 
* 
*/
void setCurrentPos(player_t* player, tuple_t* currentPosition); 

/**************** getGold ****************/
/*
* 
* 
*/
int getGold(player_t* player); 

/**************** setGold ****************/
/*
* 
* 
*/
void setGold(player_t* player, int gold); 

/**************** getSpectatorStatus ****************/
/*
* 
* 
*/
bool getSpectatorStatus(player_t* player); 

/**************** setSpectatorStatus ****************/
/*
* 
* 
*/
void setSpectatorStatus(player_t* player, bool spectator); 
