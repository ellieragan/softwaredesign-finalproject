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
static int* seedPtr = &seed; 
addr_t* address; // TODO -- real address here
grid_t* masterGrid; 


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

    message_setAddr("localhost", "2000", address); 
    masterGrid = grid_new("./maps/small.txt", seed);

    // testing playerInit
    printf("Testing playerInit with valid inputs: \n"); 
    testWorkingPlayerInit(); 
    printf("playerInit with valid inputs test done\n"); 

    printf("Testing playerInit with invalid inputs: \n"); 
    testInvalidPlayerInit(); 
    printf("playerInit with invalid inputs test done \n"); 
    printf("------------------------------------------");
    
    // testing handlePlayerMove
    printf("Testing handlePlayerMove with a valid move: \n"); 
    testValidPlayerMove(); 
    printf("handlePlayerMove with a valid move test done \n"); 

    printf("Testing valid handlePlayerMove with collecting gold: \n"); 
    testPlayerCollectingGold(); 
    printf("valid handlePlayerMove with collecting gold test done \n"); 

    printf("Testing handlePlayerMove with a invalid move (out of bounds): \n"); 
    testOutOfBoundsPlayerMove(); 
    printf("handlePlayerMove with a invalid move (out of bounds) test done\n");

    printf("Testing handlePlayerMove with a invalid move (into a wall): \n"); 
    testWallPlayerMove(); 
    printf("handlePlayerMove with a invalid move (into a wall) test done\n"); 

    printf("Testing handlePlayerMove with a invalid move (into another player): \n"); 
    testOtherPlayerMove(); 
    printf("handlePlayerMove with a invalid move (into another player) test done\n");

    printf("------------------------------------------");
    
    // testing deletePlayer
    printf("Testing delete player\n"); 
    testDeletePlayer(); 
    printf("delete player test done\n");

}

void testWorkingPlayerInit()
{

    printf("Initializing player... \n"); 
    player_t* player = initPlayer(realname, ID, masterGrid, address, seedPtr); 
    if (player == NULL) {
        fprintf(stderr, "Initializing working player failed\n"); 
        return; 
    }
}

void testInvalidPlayerInit() 
{
    printf("Initializing player... \n"); 
    player_t* player = initPlayer(realname, ID, masterGrid, NULL, seedPtr); 
    if (player == NULL) {
        fprintf(stderr, "Initializing working player failed\n"); 
        return; 
    }
}

void testValidPlayerMove()
{
    return; 
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