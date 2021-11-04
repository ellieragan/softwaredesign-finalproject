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

typedef struct player{
    char* realName; 
    char* ID; 
    grid_t* grid; 
    tuple_t* currentPos; 
    int gold; 
    bool spectator; 
} player_t; 