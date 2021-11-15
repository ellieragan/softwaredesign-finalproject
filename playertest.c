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

char* realname = "donia"; 
static const char ID = 'A'; 
int seed = 19283;
int* seedPtr = &seed; 
const addr_t address; // TODO -- real address here
grid_t* masterGrid; 
grid_t* spectatorGrid; 

/**************** testing functions ****************/
void testWorkingPlayerInit(grid_t* grid);  
void testInvalidNamePlayerInit(grid_t* grid); 
void testInvalidGridPlayerInit(); 
void testValidPlayerStep(); 
void testValidPlayerSprint(); 
void testPlayerCollectingGold();  
void testOutOfBoundsPlayerMove();
void testWallPlayerMove(); 
void testOtherPlayerMove(); 
void testDeletePlayer(player_t* player); 


int main(const int argc, char* argv[]){

    if (argc > 1) {
        fprintf(stderr, "No arguments to this testing file.\n"); 
        return 1; 
    }

    masterGrid = grid_new("./maps/visdemo.txt", seed);
    spectatorGrid = grid_new("./maps/visdemo.txt", seed);
    buildPiles(seed, spectatorGrid); 

    // testing playerInit
    printf("Testing playerInit with valid inputs: \n"); 
    testWorkingPlayerInit(masterGrid); 
    printf("playerInit with valid inputs test done\n\n"); 

    printf("Testing playerInit with invalid name input: \n"); 
    testInvalidNamePlayerInit(masterGrid); 
    printf("playerInit with invalid input name test done \n\n"); 

    printf("Testing playerInit with invalid grid: \n"); 
    testInvalidGridPlayerInit(); 
    printf("playerInit with invalid grid test done \n"); 
    printf("------------------------------------------\n\n");
    
    // testing handlePlayerMove
    player_t* player = initPlayer(realname, ID, masterGrid, address);

    printf("Testing handlePlayerMove with a valid step: \n"); 
    testValidPlayerStep(player, masterGrid, spectatorGrid); 
    printf("handlePlayerMove with a valid step test done \n\n"); 

    printf("Testing handlePlayerMove with a valid sprint: \n"); 
    testValidPlayerSprint(player, masterGrid, spectatorGrid); 
    printf("handlePlayerMove with a valid sprint test done \n\n"); 

    printf("Testing handlePlayerMove with a invalid move (out of bounds): \n"); 
    testOutOfBoundsPlayerMove(player, masterGrid, spectatorGrid); 
    printf("handlePlayerMove with a invalid move (out of bounds) test done\n\n");

    printf("Testing handlePlayerMove with a invalid move (into a wall): \n"); 
    testWallPlayerMove(player, masterGrid, spectatorGrid); 
    printf("handlePlayerMove with a invalid move (into a wall) test done\n"); 

    printf("------------------------------------------\n");
    
    // testing deletePlayer
    printf("Testing delete player\n"); 
    testDeletePlayer(player); 
    printf("delete player test done\n");

}

void testWorkingPlayerInit(grid_t* grid)
{
    printf("Initializing player... \n"); 
    player_t* player = initPlayer(realname, ID, masterGrid, address); 
    if (player == NULL) {
        fprintf(stderr, "Initializing working player failed\n"); 
        return; 
    }
}

void testInvalidNamePlayerInit(grid_t* grid) 
{
    printf("Initializing player... \n"); 
    player_t* player = initPlayer(NULL, ID, masterGrid, address); 
    if (player == NULL) {
        printf("Invalid name inputs test passed\n"); 
        return; 
    }
}

void testInvalidGridPlayerInit() 
{
    printf("Initializing player... \n"); 
    player_t* player = initPlayer(realname, ID, NULL, address); 
    if (player == NULL) {
        printf("Invalid grid inputs test passed\n"); 
        return; 
    }
}

void testValidPlayerStep(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid)
{
    tuple_t* playerPosition = initTuple(5, 3); 
    setCurrentPos(player, playerPosition); 
    tuple_t* pos = getCurrentPos(player); 
    printf("player original position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
    int gold = handlePlayerMove(player, masterGrid, spectatorGrid, 'k', NULL);
    printf("amount of gold collected by player movement (should be 0): %d\n", gold);
    pos = getCurrentPos(player); 
    printf("player new position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
}


void testValidPlayerSprint(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid)
{
    tuple_t* pos = getCurrentPos(player); 
    printf("player original position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
    int gold = handlePlayerMove(player, masterGrid, spectatorGrid, 'L', NULL);
    printf("amount of gold collected by player movement (should be 0): %d\n", gold);
    pos = getCurrentPos(player); 
    printf("player new position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
}

void testOutOfBoundsPlayerMove(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid)
{
    tuple_t* playerPosition = initTuple(1, 1); 
    setCurrentPos(player, playerPosition); 
    tuple_t* pos = getCurrentPos(player); 
    printf("player original position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
    int gold = handlePlayerMove(player, masterGrid, spectatorGrid, 'k', NULL);
    printf("amount of gold collected by player movement (should be 0): %d\n", gold);
    pos = getCurrentPos(player); 
    printf("player new position: %d, %d\n", tupleGetX(pos), tupleGetY(pos));
}

void testWallPlayerMove(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid)
{
    tuple_t* playerPosition = initTuple(1, 3); 
    setCurrentPos(player, playerPosition); 
    tuple_t* pos = getCurrentPos(player); 
    printf("player original position: %d, %d\n", tupleGetX(pos), tupleGetY(pos)); 
    int gold = handlePlayerMove(player, masterGrid, spectatorGrid, 'h', NULL);
    printf("amount of gold collected by player movement (should be 0): %d\n", gold);
    pos = getCurrentPos(player); 
    printf("player new position: %d, %d\n", tupleGetX(pos), tupleGetY(pos));
}

// void testOtherPlayerMove(player_t* player, grid_t* masterGrid, grid_t* spectatorGrid)
// {
//     return;
// }

void testDeletePlayer(player_t* player)
{
    deletePlayer(player); 
}