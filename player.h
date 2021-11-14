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
#include <unistd.h>
#include "grid.h"
#include "tuple.h"
#include "libcs50/mem.h"


/**************** global types ****************/
typedef struct player player_t; // opaque to users of the module 

/**************** global functions, called by server ****************/

/**************** initPlayer ****************/
/* Initializes a new player data structure 
* 
* input: initial values for player's internal instance variables 
* output: a pointer to the initialized player data struct or NULL if unable to initialize
*/
player_t* initPlayer(char* realName, const char ID, grid_t* masterGrid, const addr_t socket);

/**************** handlePlayerMove ****************/
/* Handles movement of a player, processes between sprints and steps 
* 
* input: player to move, master grid, spectator grid, the key pressed, and a list of other players
* output: an int representing the amount of gold collected (if any) by the movement
*/
int handlePlayerMove(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid, char keyPressed, player_t** players);

/**************** deletePlayer ****************/
/* Deletes the player object and clears associated memory 
* 
* input: the player to delete 
* output: n/a
*/
void deletePlayer(player_t* player); 

/**************** getters and setters ****************/

/**************** getRealName ****************/
/* get a player's inputted real name 
* 
* input: player 
* output: player's real name 
*/
char* getRealName(player_t* player); 

/**************** setRealName ****************/
/* set a player's real name 
* 
* input: player and name to set 
* output: n/a 
*/
void setRealName(player_t* player, char* realName); 

/**************** getID ****************/
/* get a player's localized ID 
* 
* input: player 
* output: player's local ID 
*/
char getID(player_t* player); 

/**************** setID ****************/
/* set a player's localized ID 
* 
* input: player and ID to set 
* output: n/a
*/
void setID(player_t* player, const char ID); 

/**************** getVisibility ****************/
/* get a player's visibility
* 
* input: player
* output: player's viewed grid 
*/
char* getVisibility(player_t* player); 

/**************** setVisibility ****************/
/* set a player's grid of viewed locations
* 
* input: player and grid to set 
* output: n/a
*/
void setVisibility(player_t* player, char* visibility); 

/**************** getCurrentPos ****************/
/* get a player's current position 
* 
* input: player
* output: player's position, represented as a tuple
*/
tuple_t* getCurrentPos(player_t* player); 

/**************** setCurrentPos ****************/
/* set a player's current position 
* 
* input: player and position 
* output: n/a 
*/
void setCurrentPos(player_t* player, tuple_t* currentPosition); 

/**************** getGold ****************/
/* get a player's gold count 
* 
* input: player
* output: player's gold collected
*/
int getGold(player_t* player); 

/**************** setGold ****************/
/* set a player's gold count 
* 
* input: player and gold collected
* output: n/a 
*/
void setGold(player_t* player, int gold); 

/**************** getSocketAddr ****************/
/* get a player's socket address
* 
* input: player
* output: pointer to the player's socket address
*/
addr_t* getSocketAddr(player_t* player); 

/**************** setSocketAddr ****************/
/* set a player's socket address
* 
* input: player and address of socket
* output: n/a
*/
void setSocketAddr(player_t* player, addr_t* socketAddr); 

/**************** helper functions ****************/

/**************** getRandomPosition ****************/
/*  helper method for generating a random position in the grid
* used in initPlayer
* 
* input: the grid
* output: a random position in the grid
*/
tuple_t* getRandomPosition(grid_t* grid); 

/**************** checkValidMove ****************/
/*  checks whether or not a move to a new position is valid
* 
* input: the spectator grid with gold piles, the new position to test, and a list of other players
* output: boolean value representing whether or not the new position is valid 
*/
bool checkValidMove(grid_t* grid, tuple_t* newPosition, player_t** players); 

/**************** addPlayerGold ****************/
/*  function for adding to a player's gold count 
* 
* input: player and the amount of gold collected
* output: n/a
*/
void addPlayerGold(player_t* player, int goldCollected); 

/**************** getNextPosition ****************/
/*  function for generating the next position of a player based on deltaX and deltaY
* 
* input: starting position, deltaX, deltaY
* output: the resulting position from that move
*/
tuple_t* getNextPosition(tuple_t* position, int deltaX, int deltaY);

/**************** playerStep ****************/
/*  driver for player step, determines new position, checks the position is valid, 
* if valid, updates gold (if necessary), updates the player's visibility, and updates spectatorGrid
* 
* input: player to move, deltaX, deltaY, master and spectator grids, and a list of other players
* output: int representing the amount of gold collected
*/
int playerStep(player_t* player, int deltaX, int deltaY, grid_t* spectatorGrid, grid_t* masterGrid, player_t** players);

/**************** playerSprint ****************/
/*  driver for sprint. calls playerStep until the next position of the player is invalid 
* 
* input: player to move, deltaX, deltaY, master and spectator grids, and a list of other players
* output: int representing the amount of gold collected
*/
int playerSprint(player_t* player, int deltaX, int deltaY, grid_t* spectatorGrid, grid_t* masterGrid, player_t** players);
