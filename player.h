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
/* Initializes a new player data structure 
* 
* input: initial values for player's internal instance variables 
* output: a pointer to the initialized player data struct or NULL if unable to initialize
*/
player_t* initPlayer(char* realName, char* ID, grid_t* grid, tuple_t* currentPos, bool spectator, addr_t socket); 

/**************** addGold ****************/
/* Add gold to a player's gold count 
* 
* input: the player who collected gold, the amount of gold collected, and a pointer to the remaining gold 
* output: a pointer to the int representing the remaining gold 
*/
int addGold(player_t* player, int goldCollected, int* remainingGold);

/**************** movePlayer ****************/
/* Moves player and updates their grid of viewed spaces
* 
* input: player who moved, the key they pressed, and a list of the other players in the game
* output: the updated grid of the player's viewed maze 
*/
grid_t* movePlayer(player_t* player, char* keyPressed, player_t** otherPlayers); 

/**************** updateSpectator ****************/
/* Update the grid of the spectator, with each player movement 
* 
* input: information about the player that moved and the spectator player object
* output: the updated spectator grid 
*/
grid_t* updateSpectator(player_t* player, char* keyPressed, player_t* spectator); 

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
char* getID(player_t* player); 

/**************** setID ****************/
/* set a player's localized ID 
* 
* input: player and ID to set 
* output: n/a
*/
void setID(player_t* player, char* ID); 

/**************** getGrid ****************/
/* get a player's grid of viewed locations
* 
* input: player
* output: player's viewed grid 
*/
grid_t* getGrid(player_t* player); 

/**************** getGrid ****************/
/* set a player's grid of viewed locations
* 
* input: player and grid to set 
* output: n/a
*/
void getGrid(player_t* player, grid_t* grid); 

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

/**************** getSpectatorStatus ****************/
/* get a player's status as a spectator 
* 
* input: player
* output: boolean representing whether or not the player is a spectator, true if player is a spectator, false otherwise 
*/
bool getSpectatorStatus(player_t* player); 

/**************** setSpectatorStatus ****************/
/* set a player's status as a spectator 
* 
* input: player and spectator status boolean 
* output: n/a
*/
void setSpectatorStatus(player_t* player, bool spectator); 


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
