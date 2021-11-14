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
player_t* initPlayer(char* realName, const char ID, grid_t* masterGrid, addr_t* socket, int* seed);

/**************** addPlayerGold ****************/
/* Add gold to a player's gold count 
* 
* input: the player who collected gold and the amount of gold collected
* output: n/a
*/
void addPlayerGold(player_t* player, int goldCollected);

/**************** movePlayer ****************/
/* Moves player and updates their grid of viewed spaces
* 
* input: player who moved, the key they pressed, and a list of the other players in the game
* output: the updated grid of the player's viewed maze 
*/
int movePlayer(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid, char keyPressed, player_t** players);

/**************** deletePlayer ****************/
/* Deletes the player object and clears associated memory 
* 
* input: the player to delete 
* output: n/a
*/
void deletePlayer(player_t* player); 

/**************** helper functions ****************/

/****************  ****************/
/*  
* 
* input: 
* output: 
*/
bool checkValidMove(grid_t* grid, tuple_t* newPosition, player_t** players); 

/****************  ****************/
/*  
* 
* input: 
* output: 
*/
tuple_t* determineNewPosition(player_t* player, char keyPressed);

/****************  ****************/
/*  
* 
* input: 
* output: 
*/
tuple_t* getRandomPosition(grid_t* grid, int seed); 

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
