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
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct player{
    char* realName; 
    char* ID; 
    grid_t* grid; 
    char* visibility; 
    tuple_t* currentPos; 
    int gold; 
    bool spectator; 
    player_t** players; 
    addr_t socket; 
} player_t; 

/**************** global functions ****************/
/* that is, visible outside this file */
/* see player.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
// TODO: init here as necessary 


/**************** initPlayer ****************/
player_t* initPlayer(char* realName, char* ID, grid_t* masterGrid, player_t** players, addr_t socket)
{
    // validate inputs aren't null
    if (realName == NULL || ID == NULL || grid == NULL || socket == NULL) {
        return NULL; 
    }

    // TODO write positionInit
    tuple_t* currentPos = positionInit(masterGrid); 

    // TODO write playerGridInit
    grid_t* grid = playerGridInit(masterGrid, currentPos); 

    bool spectator; 
    if (players == NULL) {
        spectator = false; 
    } else{
        spectator = true; 
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

/**************** positionInit ****************/
// TODO --> SHOULD GO IN SERVER
tuple_t* positionInit(grid_t* grid, int seed)
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

/**************** playerGridInit ****************/
grid_t* playerGridInit(grid_t* masterGrid, tuple_t* position)
{

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
void movePlayer(player_t* player, grid_t* masterGrid, char keyPressed, player_t** players)
{
    tuple_t* newPosition = determineNewPosition(player, keyPressed); 
    if (checkValidMove(grid, getCurrentPos(player), newPosition)) {
        updateGrid(getGrid(player), masterGrid, getCurrentPos(player), newPosition, players);  // TODO: write in grid.c
        setCurrentPos(player, newPosition); 
    } else {
        // TODO: what to return if the player inputs an invalid move?
        return NULL; 
    }
}

/**************** updateSpectator ****************/
grid_t* updateSpectator(player_t* player, grid_t* masterGrid, char keyPressed, player_t* spectator, player_t** players)
{
    tuple_t* newPosition = determineNewPosition(player, keyPressed); 
    if (checkValidMove(grid, newPosition, players)) {
        updateSpectatorGrid(getGrid(spectator), masterGrid, getID(player), newPosition, getCurrentPos(player)); // TODO: write in grid.c
        return getGrid(player); 
    } else {
        // TODO: what to return if the player inputs an invalid move?
        return NULL; 
    }  
} 

/**************** checkValidMove ****************/
bool checkValidMove(grid_t* grid, tuple_t* newPosition, player_t** players)
{
    int x = tupleGetX(newPosition); 
    int y = tupleGetY(newPosition); 

    // validate within bounds 
    if (x < 0 || x > getRows(grid) || y < 0 || y > getCols(grid)) { return false; }

    // validate not a wall
    if (! isValidSpot(grid, newPosition)) { return false; }

    // validate no other player is there
   for (int i = 0; i < 26; i++) { // TODO - may need if statement so that the current player isn't being checked here
        player_t* otherPlayer = players[i]
        if (tupleEquals(getCurrentPos(otherPlayer), newPosition)) {
            return false; 
        }
    }
    // would return an index
    return true; 
}

/**************** determineNewPosition ****************/
tuple_t* determineNewPosition(player_t* player, char keyPressed) 
{
    tuple_t* currentPosition = getCurrentPos(player); 
    int x = tupleGetX(currentPosition); 
    int y = tupleGetY(currentPosition); 

    switch(keyPressed) { // assumes 0,0 is the top left corner
        case ('k') : // up 
            y -= 1;
            break; 
        case('l') : // right 
            x += 1; 
            break; 
        case('j') : // down
            y += 1; 
            break; 
        case ('h') : // left 
            x -= 1; 
            break; 
        case ('y') : // diagonally up & left
            y -= 1; 
            x -= 1; 
            break; 
        case('u') : // diagonally up & right 
            y -= 1; 
            x += 1; 
            break; 
        case('b') :  // diagonally down & left
            y += 1; 
            x -= 1; 
            break; 
        case ('n') : // diagonally down & right  
            y += 1; 
            x += 1; 
            break; 
    }

    return initTuple(x, y); 
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

/**************** helper methods that will go in grid ****************/
bool isValidSpot(grid_t* grid, tuple_t* location) 
{
    int x = tupleGetX(location); 
    int y = tupleGetY(location); 

    return ((strcmp(grid[x][y], '#')) || (strcmp(grid[x][y], '.'))); 
}

void updateGrid(grid_t* playerGrid, grid_t* masterGrid, tuple_t* oldPosition, tuple_t* newPosition, player_t** players)
{
    // physically move the player in the grid 
    movePlayerInGrid(playerGrid, masterGrid, '@', newPosition, oldPosition); 
    
    // update player visibility
    updateVisibility(playerGrid, newPosition, players);  
}

updateSpectatorGrid(grid_t* spectatorGrid, grid_t* masterGrid, char playerID, tuple_t* newPosition, tuple_t* oldPosition)
{
    movePlayerInGrid(spectatorGrid, masterGrid, playerID, newPosition, oldPosition); 
} 

void movePlayerInGrid(grid_t* grid, grid_t* masterGrid, char playerID, tuple_t* newPosition, tuple_t* oldPosition)
{
    int newX = tupleGetX(newPosition); 
    int newY = tupleGetY(newPosition); 
    int oldX = tupleGetX(oldPosition); 
    int oldY = tupleGetY(oldPosition); 

    grid[oldX][oldY] = masterGrid[oldX][oldY]; 
    grid[newX][newY] = playerID; 
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

/**************** getPlayers ****************/
bool getPlayers(player_t* player) { return player->players; }

/**************** setPlayers ****************/
void setPlayers(player_t* player, player_t** players) { player->players = players; }

/**************** getSocketAddr ****************/
addr_t getSocketAddr(player_t* player) { return player->socket; }

/**************** setSocketAddr ****************/
void setSocketAddr(player_t* player, addr_t socketAddr) { player->socket = socketAddr; }
