/*
 * server.c
 * Server for nuggets; All game logic is handled by server
 *
 * Team 7: Gitting an A
 * (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
 * CS50, Fall 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
// #include "grid.h"

// function declarations
static void parseArgs(const int argc, char* argv[]);
bool handleMsg(void* arg, const addr_t from, const char* message);
//bool handleTO(void* arg);
//bool handleStdin(void* arg);

int main(const int argc, char* argv[]){
  parseArgs(argc, argv);

  char* map = NULL;
  int seed = NULL;

  if (argc == 2){
    map = argv[1];
  }
  else if (argc == 3){
    map = argv[1];
    seed = atoi(argv[2]);
  }
  
  // masterGrid holds total amount of gold
  grid_t* masterGrid = grid_new(map, seed);

  // players stores all player structs
  int maxPlayers = 26;
  player_t** players = calloc(maxPlayers+1,sizeof(player_t*)); // include spectator

  // helps pass 2 things into arg of message_loop
  hashtable_t* args = hashtable_new(2);
  hashtable_insert(args, "masterGrid", masterGrid);
  hashtable_insert(args, "players", players);

  message_init(stderr);
  message_loop(args, 0, NULL, NULL, handleMsg);
  message_done();
}

/* parseArgs
 * Parses command-line arguments and verifies validity
 *
 * Exit codes:
 * 1: wrong nummber of args
 * 2: invalid map.txt file
 * 3: optional seed is not a positive int
 */
static void parseArgs(const int argc, char* argv[]){
  // check arg count
  if (argc != 2 && argc != 3){
    fprintf(stderr, "Error: Incorrect number of arguments passed.\n");
    exit(1);
  }
  
  // test validity of map.txt regardless of whether optional seed was passed
  if (argc == 2 || argc == 3){
    // verify map txt file can be opened for reading
    FILE* fp = fopen(argv[1],"r"); // open file
    if (fp == NULL){ // exit if cannot open
      fprintf(stderr, "Error: Unable to read the provided map.txt.\n");
      exit(2);
    }
    fclose(fp); // close file
  }

  // test if optional seed is positive int only if passed
  if (argc == 3){
    int seed = atoi(argv[2]); // if provided input isn't an int, string will return 0
    if (seed < 1){ // if non positive int
      fprintf(stderr, "Error: Provided seed is not a positive integer.\n");
      exit(3);
    }
  }
}

/* handleMsg
 * Runs when server receives message
 */
bool handleMsg(void* arg, const addr_t from, const char* message){
  
