/*
* playertest.c
* Module for testing the player data structure 
*
* Team 7: Gitting an A
* (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
* CS 50, Fall 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "./support/message.h"
#include "libcs50/mem.h"
#include "./tuple.h"
#include "./grid.h"
#include "./player.h"

static char* realname = "donia"; 
static const char ID = 'A'; 
int seed = 19283;
int* seedPtr = &seed; 
addr_t* address; // TODO -- real address here
grid_t* masterGrid; 
grid_t* spectatorGrid; 

/**************** testing functions ****************/
void testWorkingPlayerInit();  
void testInvalidPlayerInit(); 
void testValidPlayerMove(); 
void testPlayerCollectingGold();  
void testOutOfBoundsPlayerMove();
void testWallPlayerMove(); 
void testOtherPlayerMove(); 
void testDeletePlayer(); 


int main(const int argc, char* argv[]){

    if (argc > 1) {
        fprintf(stderr, "No arguments to this testing file.\n"); 
        return 1; 
    }

    masterGrid = grid_new("./maps/small.txt", seed);
    spectatorGrid = grid_new("./maps/small.txt", seed);
    buildPiles(seed, spectatorGrid); 

    // printf("test random position: \n"); 
    // randomPos(masterGrid, seed);

    // testing playerInit
    // TODO: playerInit works with a null seed, not with a nonnull seed
    printf("Testing playerInit with valid inputs: \n"); 
    testWorkingPlayerInit(masterGrid); 
    printf("playerInit with valid inputs test done\n"); 

    // printf("Testing playerInit with invalid inputs: \n"); 
    // testInvalidPlayerInit(); 
    // printf("playerInit with invalid inputs test done \n"); 
    printf("------------------------------------------\n\n");
    
    // testing handlePlayerMove
    player_t* player = initPlayer(realname, ID, masterGrid, address, NULL);
    tuple_t* playerPosition = initTuple(5, 5); 
    setCurrentPos(player, playerPosition); 

    printf("Testing handlePlayerMove with a valid move: \n"); 
    testValidPlayerMove(player, masterGrid), spectatorGrid; 
    printf("handlePlayerMove with a valid move test done \n\n"); 

    // printf("Testing valid handlePlayerMove with collecting gold: \n"); 
    // testPlayerCollectingGold(); 
    // printf("valid handlePlayerMove with collecting gold test done \n"); 

    // printf("Testing handlePlayerMove with a invalid move (out of bounds): \n"); 
    // testOutOfBoundsPlayerMove(); 
    // printf("handlePlayerMove with a invalid move (out of bounds) test done\n");

    // printf("Testing handlePlayerMove with a invalid move (into a wall): \n"); 
    // testWallPlayerMove(); 
    // printf("handlePlayerMove with a invalid move (into a wall) test done\n"); 

    // printf("Testing handlePlayerMove with a invalid move (into another player): \n"); 
    // testOtherPlayerMove(); 
    // printf("handlePlayerMove with a invalid move (into another player) test done\n");

    // printf("------------------------------------------");
    
    // // testing deletePlayer
    // printf("Testing delete player\n"); 
    // testDeletePlayer(); 
    // printf("delete player test done\n");

}

void randomPos(grid_t* grid, int seed)
{
    getRandomPosition(grid, seed);
}

void testWorkingPlayerInit(grid_t* grid)
{

    printf("Initializing player... \n"); 
    player_t* player = initPlayer(realname, ID, masterGrid, address, -1); 
    if (player == NULL) {
        fprintf(stderr, "Initializing working player failed\n"); 
        return; 
    }
}

void testInvalidPlayerInit(grid_t* grid) 
{
    printf("Initializing player... \n"); 
    player_t* player = initPlayer(realname, ID, masterGrid, NULL, seedPtr); 
    if (player == NULL) {
        fprintf(stderr, "Initializing working player failed\n"); 
        return; 
    }
}

void testValidPlayerMove(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid)
{
    tuple_t* pos = getCurrentPos(player); 
    printf("player original position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
    int gold = handlePlayerMove(player, masterGrid, spectatorGrid, 'k', NULL);
    printf("amount of gold collected by player movement (should be 0): %d\n", gold);
    pos = getCurrentPos(player); 
    printf("player new position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
 
}

void testPlayerCollectingGold()
{
    return; 
}

void testOutOfBoundsPlayerMove()
{
    return;
}

void testWallPlayerMove()
{
    return;
}

void testOtherPlayerMove()
{
    return;
}

void testDeletePlayer()
{
    return;
}