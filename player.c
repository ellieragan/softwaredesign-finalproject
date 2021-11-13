/*
* player.c
* Module for the player data structure 
*
* Team 7: Gitting an A
* (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
* CS 50, Fall 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tuple.h"
#include "libcs50/mem.h"
// #include "grid.h"

/**************** file-local global variables ****************/
static const int left = -1; 
static const int right = -1; 
static const int up = -1; 
static const int down = 1; 

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct player{
    char* realName; 
    char* ID; 
    char* visibility; 
    tuple_t* currentPos; 
    int gold; 
    addr_t socket; 
} player_t; 

/**************** global functions ****************/
/* that is, visible outside this file */
/* see player.h for comments about exported functions */
player_t* initPlayer(char* realName, char* ID, grid_t* masterGrid, addr_t socket); 
int movePlayer(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid, char keyPressed, player_t** players); 
void deletePlayer(player_t* player); 

/**************** local functions ****************/
/* not visible outside this file */
tuple_t* getRandomPosition(grid_t* grid, int seed); 
bool checkValidMove(grid_t* grid, tuple_t* newPosition, player_t** players); 
int addPlayerGold(player_t* player, int goldCollected); 
tuple_t* determineNewPosition(player_t* player, char keyPressed); 

/**************** initPlayer ****************/
player_t* initPlayer(char* realName, char* ID, grid_t* masterGrid, addr_t socket, int seed)
{
    // validate inputs aren't null
    if (realName == NULL || ID == NULL || grid == NULL || socket == NULL) {
        return NULL; 
    }

    // randomly select a position for the player
    tuple_t* currentPos = getRandomPosition(masterGrid, seed); 

    // calculate player's visibility at that position 
    char* visibility = initializeVisibility(masterGrid, tupleGetX(currentPos), tupleGetY(currentPos)); 

    // TODO - if we use local grids --> initialize player's local grid 

    // allocate space and set instance variables
    player_t* player = mem_malloc(sizeof(player_t));
    setRealName(player, realName); 
    setID(player, ID); 
    // setGrid(player, grid); 
    setVisibility(player, visibility);
    setCurrentPos(player, currentPos); 
    // setSpectatorStatus(player, spectator); 
    setSocketAddr(socket); 

    // check everything was initialized correctly 
    if (getRealName(player) == NULL || getID(player) == NULL ||  getVisibility(player) == NULL || 
        getCurrentPos(player) == NULL || getSocketAddr(player) == NULL ) {
        return NULL; 
    } 

    return player; 
}

/**************** positionInit ****************/
tuple_t* getRandomPosition(grid_t* grid, int seed)
{
    int numRows = getRows(grid); 
    int numCols = getCols(grid); 

    if (seed != NULL) {
        srand(seed); 
    } else {
        srand(getpid()); 
    }

    int x = rand() % (numRows + 1); 
    int y = rand() % (numCols + 1); 

    tuple_t* tuple = initTuple(x, y); 
    while (! isValidSpot(tuple)) {
        int x = rand() % (numRows + 1); 
        int y = rand() % (numCols + 1);
        tuple = initTuple(x, y); 
    }
    return tuple; 
}

/**************** checkValidMove ****************/
bool checkValidMove(grid_t* grid, tuple_t* newPosition, player_t** players)
{
    int x = tupleGetX(newPosition); 
    int y = tupleGetY(newPosition); 

    // validate within bounds 
    if (x < 0 || x > getRows(grid) || y < 0 || y > getCols(grid)) { return false; }

    // validate not a wall
    if (! validSpot(grid, x, y)) { return false; }

    // validate no other player is there
   for (int i = 0; i < 26; i++) { // TODO - may need if statement so that the current player isn't being checked here
        player_t* otherPlayer = players[i]
        if (otherPlayer != NULL && tupleEquals(getCurrentPos(otherPlayer), newPosition)) {
            return false; 
        }
    }
    // would return an index
    return true; 
}

/**************** handlePlayerMove ****************/
int handlePlayerMove(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid, char keyPressed, player_t** players)
{

    switch(keyPressed) { // assumes 0,0 is the top left corner
        case ('k') : // up 
            return playerStep(player, 0, up); 
        case('l') : // right 
            return playerStep(player, right, 0);
        case('j') : // down
            return playerStep(player, 0, down);
        case ('h') : // left 
            return playerStep(player, left, 0);
        case ('y') : // diagonally up & left
            return playerStep(player, left, up);
        case('u') : // diagonally up & right 
            return playerStep(player, right, up);
        case('b') :  // diagonally down & left
            return playerStep(player, left, down);
        case ('n') : // diagonally down & right  
            return playerStep(player, right, down);

        case ('K') : // up 
            return playerSprint(player, 0, up); 
        case('L') : // right 
            return playerSprint(player, right, 0); 
        case('J') : // down
            return playerSprint(player, 0, down); 
        case ('H') : // left 
            return playerSprint(player, left, 0); 
        case ('Y') : // diagonally up & left
            return playerSprint(player, left, up); 
        case('U') : // diagonally up & right 
            return playerSprint(player, right, up); 
        case('B') :  // diagonally down & left
            return playerSprint(player, left, down); 
        case ('N') : // diagonally down & right  
            return playerSprint(player, right, down);   
    }
}

/**************** getNextPosition ****************/
tuple_t* getNextPosition(tuple_t* position, int deltaX, int deltaY)
{
    return initTuple(tupleGetX(position) + deltaX, tupleGetY(position) + deltaY);
}

/**************** playerStep ****************/
int playerStep(player_t* player, int deltaX, int deltaY)
{
    tuple_t* currentPosition = getCurrentPos(player);
    tuple_t* newPosition = getNextPosition(currentPosition, deltaX, deltaY); 

    // determine the new position of the player
    int x = tupleGetX(newPosition); 
    int y = tupleGetY(newPosition); 
    
    int goldCollected = 0; 

    // if new position is valid 
    if (checkValidMove(masterGrid, newPosition, players)) {
        // check if gold and update accordingly
        if (isGold(spectatorGrid, newPosition)) {
            goldCollected = updateGoldCount(masterGrid, newPosition); // get amount of gold collected
            addPlayerGold(player, goldCollected); // update player's count of their gold 
        }
        
        // update player's visibility 
        char* updatedVisibility = updateVisibility(masterGrid, posX, posY, getVisibility(player));
        mem_free(getVisibility(player)); // TODO - not sure if this is necessary 
        int index = charConvertIndexNum(masterGrid, posX, posY); 
        updatedVisibility[index] = '@'; 
        setVisibility(updatedVisibility);  

        // update spectator grid
        updateSpectatorGrid(spectatorGrid, masterGrid, getID(player), newPosition, getCurrentPos(player)); 

        // update player current position
        setCurrentPos(player, newPosition); 
        
    } 

    return goldCollected; 

}


/**************** playerSprint ****************/
int playerSprint(player_t* player, int deltaX, int deltaY)
{
    int goldCollected = 0; 

    tuple_t* nextPosition = getNextPosition(getCurrentPos(player), deltaX, deltaY); 
    // while next position is valid 
    while (checkValidMove(nextPosition)) {
        // call playerStep 
        goldCollected += playerStep(player, deltaX, deltaY);
    }

    return goldCollected; 
}


/**************** addPlayerGold ****************/
void addPlayerGold(player_t* player, int goldCollected)
{
    int currGold = getGold(player); 
    setGold(player, currGold + goldCollected); 
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

        if (getVisibility(player) != NULL) {
            mem_free(player->visibility); 
        }
        // TODO: only question is whether or not we need to delete/free the tuple
    }
}


/**************** getters & setters ****************/

/**************** getRealName ****************/
char* getRealName(player_t* player) { return player->realName; }

/**************** setRealName ****************/
void setRealName(player_t* player, char* realName) { player->realName = realName; }

/**************** getID ****************/
char* getID(player_t* player) { return player->ID; }

/**************** setID ****************/
void setID(player_t* player, char* ID) { player->ID = ID; }

/**************** getVisibility ****************/
char* getVisibility(player_t* player) { return player->visibility; }

/**************** setVisibility ****************/
void setVisibility(player_t* player, char* visibility) { player->visibility = visibility; }

/**************** getCurrentPos ****************/
tuple_t* getCurrentPos(player_t* player) { return player->currentPosition; }

/**************** setCurrentPos ****************/
void setCurrentPos(player_t* player, tuple_t* currentPosition) { player->currentPosition = currentPosition; } 

/**************** getGold ****************/
int getGold(player_t* player) { return player->gold; }

/**************** setGold ****************/
void setGold(player_t* player, int gold) { player->gold = gold; }

/**************** getSocketAddr ****************/
addr_t getSocketAddr(player_t* player) { return player->socket; }

/**************** setSocketAddr ****************/
void setSocketAddr(player_t* player, addr_t socketAddr) { player->socket = socketAddr; }
