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
#include <ctype.h>
#include "message.h"
#include "tuple.h"
#include "grid.h"
#include "player.h"
#include "./libcs50/hashtable.h"


// function declarations
static void parseArgs(const int argc, char* argv[]);
bool handleMsg(void* arg, const addr_t from, const char* message);
//static char** parseMsg(char* msg);
//static int countPlayers(player_t** players);
static bool ifEmpty(char* str);
static char* processName(char* name);
static char playerID(int playerIndex);
static bool validKey(char key, bool spectator);
static int getPlayerin(player_t** players, const addr_t from);
//static char* getDisplay(player_t* player);
static char* endMessage(player_t** players, int index);
void hashDel(void* item);
static bool isPlayer(const addr_t from, player_t** players, int *index);

typedef struct spect{
  addr_t spectator;
} spect_t;

static spect_t* spectCast_new(const addr_t address){
  spect_t* spectCast = mem_malloc(sizeof(addr_t));

  if (spectCast == NULL){
    return NULL;
  }
  else{
    spectCast->spectator = address;
    return spectCast;
  }
}

// global constants
int maxNamelength = 50;
int maxPlayers = 26;

int main(const int argc, char* argv[]){
  parseArgs(argc, argv);

  char* map;
  int seed = -1;

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
  player_t** players = calloc(maxPlayers,sizeof(player_t*));

  // spectator stored in player struct
  grid_t* spectator = grid_new(map, seed);
  buildPiles(seed,spectator);
  const addr_t noAddr = message_noAddr();
  spect_t* spectAddr = spectCast_new(noAddr);

  // next open slot in players
  int index = 0;

  // helps pass 3 things into arg of message_loop
  hashtable_t* args = hashtable_new(6);
  hashtable_insert(args, "masterGrid", masterGrid);
  hashtable_insert(args, "players", players);
  hashtable_insert(args, "spectator", spectator);
  hashtable_insert(args, "spectAddr", spectAddr);
  hashtable_insert(args,"index", &index);
  hashtable_insert(args,"seed", &seed);

  message_init(stderr);
  message_loop(args, 0, NULL, NULL, handleMsg);
  message_done();
  delete(masterGrid);
  free(players);
  delete(spectator);
  hashtable_delete(args,hashDel);
  return 0;
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
  grid_t* masterGrid = hashtable_find(arg, "masterGrid");
  player_t** players = hashtable_find(arg, "players");
  grid_t* spectator = hashtable_find(arg, "spectator");
  spect_t* spectAddr = hashtable_find(arg, "spectAddr");
  int* index = hashtable_find(arg,"index");
  //int* seed = hashtable_find(arg,"seed");

  // handles case when all gold has been collected
  if (getGoldLeft(spectator) <= 0){
    char* endGame = endMessage(players,*index);
    // send endGame message to all players
    int pos;
    for (pos = 0; pos<*index; pos++){
      if (players[pos] != NULL){
        deletePlayer(players[pos]);
        message_send(getSocketAddr(players[pos]),endGame);
      }
    }
    free(endGame);
    return true; // terminates loop
  }

  //char msgCpy[strlen(message)+1];
  //strcpy(msgCpy,message);
  //char** msgParts = parseMsg(msgCpy);
  //char* msgType = msgParts[0];
  //char* msgRest = msgParts[1];
  //printf("Type: %s\n",msgType);
  //printf("Msg: %s\n",msgRest);

  if (strncmp(message, "PLAY ",strlen("PLAY ")) == 0){ // PLAY type messages
    const char* content = message + strlen("PLAY ");
    char msgR[strlen(content)+1];
    strcpy(msgR,content);
    char* msgRest = msgR;
    // if there are already maxPlayer players
    if (players[maxPlayers-1] != NULL){
      message_send(from, "QUIT Game is full: no more players can join.");
    }
    // if player's real name is empty
    else if (ifEmpty(msgRest)){
      message_send(from, "QUIT Sorry - you must provide player's name.");
    }
    // otherwise, add player to game
    else{
      char ID = playerID(*index);

      // initialize player and place into players array
      players[*index] = initPlayer(processName(msgRest),ID,masterGrid,from);
      
      // construct and send OK message to client
      char okMsg[6]; // OK + space + ID + \0 + extra char
      sprintf(okMsg,"OK %c",ID);
      const char* okMessage = okMsg;
      message_send(from, okMessage);

      // construct and send GRID message to client
      int nrows = getRows(spectator);
      int ncols = getCols(spectator);
      
      char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
      sprintf(gridMsg,"GRID %d %d",nrows,ncols);
      const char* gridMessage = gridMsg;
      message_send(from, gridMessage);

      // construct and send GOLD message to clients
      int remain = getGoldLeft(spectator);

      char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
      sprintf(goldMsg, "GOLD %d %d %d",0,0,remain);
      const char* goldMessage = goldMsg;
      message_send(from, goldMessage);

      // construct and send DISPLAY message to clients
      char* displayStr = getVisibility(players[*index]);

      char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
      sprintf(displayMsg, "DISPLAY\n%s", displayStr);
      const char* displayMessage = displayMsg;
      message_send(from, displayMessage);
      
      free(displayStr);

      (*index)++; // add to index count
    }
    return false; // continue loop
  }
  if (strncmp(message, "SPECTATE",strlen("SPECTATE")) == 0){ // SPECTATE type messages
    //const char* content = message + strlen("SPECTATE ");
    //char msgR[strlen(content)+1];
    //strcpy(msgR,content);
    //char* msgRest = m
    if (message_isAddr(spectAddr->spectator)){ // replace existing spectator address with new one
      message_send(spectAddr->spectator,"QUIT You have been replaced by a new spectator.");
      spectAddr = spectCast_new(from);
    }
    if (!message_isAddr(spectAddr->spectator)){ // just allocate spectator address
      spectAddr = spectCast_new(from);
    }
    
    // construct and send GRID message to client
    int nrows = getRows(spectator);
    int ncols = getCols(spectator);

    char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
    sprintf(gridMsg,"GRID %d %d",nrows,ncols);
    const char* gridMessage = gridMsg;
    message_send(from, gridMessage);

    // construct and send GOLD message to clients
    int remain = getGoldLeft(spectator);

    char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
    sprintf(goldMsg, "GOLD %d %d %d",0,0,remain);
    const char* goldMessage = goldMsg;
    message_send(from, goldMessage);

    // construct and send DISPLAY message to clients
    char* displayStr = getFileMap(spectator);

    char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
    sprintf(displayMsg, "DISPLAY\n%s", displayStr);
    const char* displayMessage = displayMsg;
    message_send(from, displayMessage);

    free(displayStr);

    return false; // continue loop
  }

  if (strncmp(message, "KEY ", strlen("KEY ")) == 0){ // KEY type messages
    const char* msgRest = message + strlen("KEY ");
    // if client is player
    if (isPlayer(from,players,index)){ // if not spectator
      // find player in question
      player_t* player = players[getPlayerin(players,from)];
      // if key is valid
      if (validKey(msgRest[0],false)){
        // if it is a movement key
        if (strcmp(msgRest,"Q")!=0){
          /******************************* Dealing with player *************************************/
          // update player grid and send new display to player
          char key = msgRest[0]; // cast as char
          int goldCol = handlePlayerMove(player,masterGrid,spectator,key,players);
          int playerIndex = getPlayerin(players, from);
          
          // construct and send GRID message to client
          int nrows = getRows(spectator);
          int ncols = getCols(spectator);

          char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
          sprintf(gridMsg,"GRID %d %d",nrows,ncols);
          const char* gridMessage = gridMsg;
          message_send(from, gridMessage);

          // construct and send GOLD message to clients
          int remain = getGoldLeft(spectator);

          char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
          sprintf(goldMsg, "GOLD %d %d %d",goldCol,getGold(players[playerIndex]),remain); 
          const char* goldMessage = goldMsg;
          message_send(from, goldMessage);

          // construct and send DISPLAY message to clients
          char* visibilityStr = getVisibility(players[playerIndex]);
          char* displayStr = gridFromVisibility(masterGrid,getFileMap(spectator),visibilityStr);

          char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
          sprintf(displayMsg, "DISPLAY\n%s", displayStr);
          const char* displayMessage = displayMsg;
          message_send(from, displayMessage);


          /***************************** Dealing with spectator *********************************/
          if (spectAddr != NULL){
            // construct and send GRID message to client
            int nrows = getRows(spectator);
            int ncols = getCols(spectator);

            char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
            sprintf(gridMsg,"GRID %d %d",nrows,ncols);
            const char* gridMessage = gridMsg;
            message_send(spectAddr->spectator, gridMessage);

            // construct and send GOLD message to clients
            int remain = getGoldLeft(spectator);

            char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
            sprintf(goldMsg, "GOLD %d %d %d",0,0,remain);
            const char* goldMessage = goldMsg;
            message_send(spectAddr->spectator, goldMessage);

            // construct and send DISPLAY message to clients
            char* displayStr = getFileMap(spectator);

            char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
            sprintf(displayMsg, "DISPLAY\n%s", displayStr);
            const char* displayMessage = displayMsg;
            message_send(spectAddr->spectator, displayMessage);
          }
        }
        // if not movement key, must be Q
        else{
          message_send(from,"QUIT Thanks for playing!");
          deletePlayer(player);
        }
      }
      // if not valid key
      else{
        message_send(from,"ERROR Invalid key for player client.");
      }
    }
    // if client is spectator
    else{
      // if key is valid
      if (validKey(msgRest[0],true)){
        // only possible key is Q
        if (strcmp(msgRest,"Q") == 0){
          message_send(from,"QUIT Thanks for watching!");
          spectAddr = NULL;
          }
      }
      // if not valid key
      else{
        message_send(from,"ERROR Invalid key for spectator client.");
      }
    }
    return false; // continue loop
  }
  else{ // if invalid msgType
    message_send(from, "ERROR Invalid message type.");
    return false; // continue loop
  }
  //free(msgType);
  //free(msgRest);
  //free(msgParts);
}

/* parseMsg
 * Helper function of handleMsg that returns array of strings
 * The string in index position 0 indicates msg type (PLAY, SPECTATE, KEY)
 * The string in index position 1 contains the rest of the message
 */
//static char** parseMsg(char* msg){
//  char** msgParts = calloc(2,sizeof(char*));
  
//  msgParts[0] = msg;

//  char* ptr = msg;
//  while(!isspace(*ptr) && (*ptr != '\0')){ // slide to first space or end of msg
//    ptr++;
//  }
//  if (isspace(*ptr)){ // break message into 2 parts
//    ptr = "\0";
//    ptr++;
//    msgParts[1] = ptr;
//  }
//  else{ // if there is only 1 part
//    msgParts[2] = NULL;
//  }
//
// return msgParts;
//}
    
/* countPlayers
 * Helper function of handleMsg
 * Counts number of players in player_t**
 */
//static int countPlayers(player_t** players){
//  int count;

//  for (count = -1; players[count] != NULL; count++){
//  }

//  return count;
//}

/* ifEmpty
 * Helper function of handleMsg
 * Finds if a string is empty (no non-space characters)
 */
static bool ifEmpty(char* str){
  bool empty = true;
  int index;

  for (index = 0; index < strlen(str); index++){
    if (!isspace(str[index])){
      empty = false;
      return empty;
    }
  }
  return empty;
}

/* processName
 * Helper function for handleMsg
 * (1) truncating an over-length real name to MaxNameLength characters
 * (2) replacing with an underscore _ any character for which both isgraph() and isblank() are false
 */
static char* processName(char* name){
  if (strlen(name) > maxNamelength){
    name[maxNamelength] = '\0';
  }

  int index;
  for (index = 0; index < strlen(name); index++){
    if (!isgraph(name[index]) && !isblank(name[index])){
      name[index] = '_';
    }
  }
  return name;
}

/* playerID
 * Helper function for handleMsg
 * Gets player ID
 */
static char playerID(int playerIndex){
  char ID = 'A';
  int pos = 0;
  while (pos < playerIndex){
    ID++;
    pos++;
  }
  return ID;
}

/* validKey
 * Helper function for handleMsg
 * Tests whether key is a valid key
 */
static bool validKey(char key, bool spectator){
  // if is spectator; only valid key is Q
  if (spectator){
    if (key != 'Q'){
      return false;
    }
    else{
      return true;
    }
  }
  // if is player
  else{
    bool valid = false;
    char validKeys[17] = {'Q','h','H','l','L','j','J','k','K','y','Y','u','U','b','B','n','N'};
    int index;
    for (index = 0; index < 17; index++){
      if (key == validKeys[index]){
        valid = true;
      }
    }
    return valid;
  }
}

/* getPlayerin
 * Gets player's index in players array
 */
static int getPlayerin(player_t** players, const addr_t from){
  int index;
  for (index = 0; index < maxPlayers; index++){
    if (message_eqAddr(from,getSocketAddr(players[index]))){
        return index;
    }
  }
  return -1;
}

/* getDisplay
 * Helper function for handleMsg
 * Gets display string of player object
 */
//static char* getDisplay(player_t* player){
//  return getFileMap(getGrid(player));
//}

/* endMessage
 * Helper function for handleMsg
 * Constructs game over message when all gold has been collected
 */
static char* endMessage(player_t** players, int index){
  char* msg = "QUIT GAME OVER:";

  int pos;
  for (pos = 0; pos<index; pos++){
    if (players[pos] != NULL){
      char* newLine = malloc(sizeof(char)*100); // 100 chars per line will be more than enough space
      sprintf(newLine,"\n%c %6d %s",playerID(pos),getGold(players[pos]),getRealName(players[pos]));
      char* newMsg = malloc(sizeof(msg)+sizeof(newLine)+10);
      sprintf(newMsg,"%s%s",msg,newLine);
      free(msg);
      free(newLine);
      msg = newMsg;
    }
  }
  return msg;
}

/* hashDel
 * Delete function for hashtable_delete
 * Deletes item if not NULL
 */
void hashDel(void* item){
  if (item != NULL){
    free(item);
  }
}

/* isPlayer
 * Helper for handleMsg
 * Tests if it is an address of a player
 */
static bool isPlayer(const addr_t from, player_t** players, int* index){
  int pos;
  for (pos = 0; pos<*index; pos++){
    if (message_eqAddr(from,getSocketAddr(players[pos]))){
      return true;
    }
  }
  return false;
}
