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
    addr_t socket; 
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


/**************** initPlayer ****************/
player_t* initPlayer(char* realName, char* ID, grid_t* grid, tuple_t* currentPos, bool spectator, addr_t socket)
{
    // validate inputs aren't null
    if (realName == NULL || ID == NULL || grid == NULL || currentPos == NULL || spectator == NULL || socket == NULL) {
        return NULL; 
    }
    
    // allocate space and set instance variables
    player_t* player = mem_malloc(sizeof(player_t));
    setRealName(player, realName); 
    setID(player, ID); 
    setGrid(player, grid); 
    setCurrentPos(player, currentPos); 
    setSpectatorStatus(spectator); 
    setSocketAddr(socket); 

    // check everything was initialized correctly 
    if (getRealName(player) == NULL || getID(player) == NULL || getGrid(player) == NULL || getCurrentPos(player) == NULL || getSpectatorStatus(player) == NULL || getSocketAddr(player) == NULL ) {
        return NULL; 
    } 

    return player; 
}

/**************** addGold ****************/
int addGold(player_t* player, int goldCollected, int* remainingGold)
{
    int currGold = getGold(player); 
    setGold(player, currGold + goldCollected); 
    *remainingGold -= goldCollected; 
    return *remainingGold; 
}

/**************** movePlayer ****************/
grid_t* movePlayer(player_t* player, char keyPressed, player_t** otherPlayers)
{

}

/**************** updateSpectator ****************/
grid_t* updateSpectator(player_t* player, char keyPressed, player_t* spectator)
{
    
} 

/**************** parseKeyPressed ****************/
/* output key: 
* 0 --> up
* 1 --> right 
* 2 --> down
* 3 --> left

* 4 --> 
* 5 --> 
* 6 --> 
* 7 --> 
* 
* 
*/
int determineDirection(char keyPressed)
{
    switch(keyPressed) {
        case ('k') // up 

        case('l') // right 

        case('j') // down

        case ('h') // left 


        case ('y') // diagonally up & left

        case('u') // diagonally up & right 

        case('b') // diagonally down & left

        case ('n') // diagonally down & right  
    }
}

/**************** checkValidMove ****************/
bool checkValidMove(grid_t* grid, tuple_t* position, int direction)
{

}


/**************** deletePlayer ****************/
void deletePlayer(player_t* player)
{
    if (player != NULL) {
        if (getRealName(player) != NULL) {
            mem_free(player->realName); 
        }

        if (getID(player) != NULL) {
            mem_free(player->ID); 
        }

        if (getGrid(player) != NULL) {
            delete(player->grid); 
        }

        // TODO: only question is whether or not we need to delete/free the tuple

    }
}

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

/**************** setGrid ****************/
void setGrid(player_t* player, grid_t* grid) { player->grid = grid; }

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

/**************** getSocketAddr ****************/
addr_t getSocketAddr(player_t* player) { return player->socket; }

/**************** setSocketAddr ****************/
void setSocketAddr(player_t* player, addr_t socketAddr) { player->socket = socketAddr; }
