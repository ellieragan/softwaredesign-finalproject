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
static char** parseMsg(char* msg);
static int countPlayers(player_t** players);
static bool ifEmpty(char* str);
static char* processName(char* name);
static char* getID(int playerIndex);
static bool validKey(char key, bool spectator);
static int getPlayerin(player_t** players, addr_t* from);
static char* getDisplay(player_t* player);
static char* endMessage(player_t** players);

// global constants
int maxNamelength = 50;
int maxPlayers = 26;
int goldTotal = 250;
int goldMinnumpile = 10;
int goldMaxnumpile = 30;

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
  player_t** players = calloc(maxPlayers,sizeof(player_t*)); // include spectator

  // spectator stored in player struct
  player_t* spectator = malloc(sizeof(player_t*));

  // next open slot in players
  int index = 0;

  // helps pass 3 things into arg of message_loop
  hashtable_t* args = hashtable_new(2);
  hashtable_insert(args, "masterGrid", masterGrid);
  hashtable_insert(args, "players", players);
  hashtable_insert(args, "spectator", spectator);
  hashtable_insert(args,"index", &index);

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
  grid_t* masterGrid = hashtable_find(arg, "masterGrid");
  player_t** players = hashtable_find(arg, "players");
  player_t* spectator = hashtable_find(arg, "spectator");
  int* index = hashtble_find(arg,"index");

  // handles case when all gold has been collected
  if (getGoldLeft(masterGrid) <= 0){
    char* endGame = endMessage(players);
    // send endGame message to all players
    int pos;
    for (pos = 0; pos<index; pos++){
      if (players[pos] != NULL){
        message_send(getSocketAddr(players[pos]),endGame);
      }
    }
    return true; // terminates loop
  }

  char** msgParts = parseMsg(message);
  char* msgType = msgParts[0];
  char* msgRest = msgParts[1];

  if (strcmp(msgType, "PLAY")){ // PLAY type messages
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
      char* playerID = getID(*index);

      // initialize player and place into players array
      players[*index] = initPlayer(processName(msgRest),playerID,masterGrid,NULL,from);
      
      // construct and send OK message to client
      char okMsg[6]; // OK + space + ID + \0 + extra char
      sprintf(okMsg,"OK %s",playerID);
      message_send(from, &okMsg);

      // construct and send GRID message to client
      int nrows = getRows(getGrid(players[*index]));
      int ncols = getCols(getGrid(players[*index]));
      
      char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
      sprintf(gridMsg,"GRID %d %d",nrows,ncols);
      message_send(from, &gridMsg);

      // construct and send GOLD message to clients
      int remain = getGoldLeft(masterGrid);

      char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
      sprintf(goldMsg, "GOLD %d %d %d",0,0,remain);
      message_send(from, &goldMsg);

      // construct and send DISPLAY message to clients
      char* displayStr = getDisplay(players[*index]); // ask donia about this

      char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
      sprintf(displayMsg, "DISPLAY\n%s", displayStr);
      message_send(from, &displayMsg);

      *index++; // add to index count
    }
    return false; // continue loop
  }
  if (strcmp(msgType, "SPECTATE")){ // SPECTATE type messages
    if (spectator != NULL){ // replace existing spectator with new one
      message_send(getSocketAddr(spectator),"QUIT You have been replaced by a new spectator.");
      deletePlayer(spectator);
      spectator = initPlayer(".",".",masterGrid,players,from);
    }
    if (spectator == NULL){ // just create new player
      spectator = initPlayer(".",".",masterGrid,players,from);
    }
    
    // construct and send GRID message to client
    int nrows = getRows(getGrid(spectator));
    int ncols = getCols(getGrid(spectator));

    char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
    sprintf(gridMsg,"GRID %d %d",nrows,ncols);
    message_send(from, &gridMsg);

    // construct and send GOLD message to clients
    int remain = getGoldLeft(masterGrid);

    char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
    sprintf(goldMsg, "GOLD %d %d %d",0,0,remain);
    message_send(from, &goldMsg);

    // construct and send DISPLAY message to clients
    char* displayStr = getDisplay(spectator); // ask donia about this

    char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
    sprintf(displayMsg, "DISPLAY\n%s", displayStr);
    message_send(from, &displayMsg);

    return false; // continue loop
  }

  if (strcmp(msgType, "KEY")){ // KEY type messages
    // find player in question
    player_t* player;
    player = players[getPlayerin(players,from)];

    // if client is player
    if (from != getSocketAddr(spectator)){ // if not spectator
      // if key is valid
      if (validKey(&msgRest,false)){
        // if it is a movement key
        if (strcmp(msgRest,"Q")!=0){
          /***************************** Dealing with spectator *********************************/
          // update spectator grid and send new display to spectator
          updateSpectator(player,masterGrid,msgRest,spectator,players);
          addr_t* spectSocket = getSocketAddr(spectator);

          // construct and send GRID message to client
          int nrows = getRows(getGrid(spectator));
          int ncols = getCols(getGrid(spectator));
      
          char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
          sprintf(gridMsg,"GRID %d %d",nrows,ncols);
          message_send(spectSocket, &gridMsg);

          // construct and send GOLD message to clients
          int remain = getGoldLeft(masterGrid);

          char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
          sprintf(goldMsg, "GOLD %d %d %d",0,0,remain);
          message_send(spectSocket, &goldMsg);

          // construct and send DISPLAY message to clients
          char* displayStr = getDisplay(spectator);

          char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
          sprintf(displayMsg, "DISPLAY\n%s", displayStr);
          message_send(spectSocket, &displayMsg);

          /******************************* Dealing with player *************************************/
          // update player grid and send new display to player
          movePlayer(player,masterGrid,msgRest,players);
          int playerIndex = getPlayerin(players, from);
          
          // construct and send GRID message to client
          int nrows = getRows(getGrid(players[playerIndex]));
          int ncols = getCols(getGrid(players[playerIndex]));

          char gridMsg[20]; // GRID + space + 5 digit rows + space + 5 digit cols + \0 + 2 extra chars
          sprintf(gridMsg,"GRID %d %d",nrows,ncols);
          message_send(from, &gridMsg);

          // construct and send GOLD message to clients
          int remain = getGoldLeft(masterGrid);

          char goldMsg[15]; // GOLD + space + 0 + space + 0 + 4 digit remianing gold + \0 + 2 extra chars
          sprintf(goldMsg, "GOLD %d %d %d",0,0,remain); // how do I get the first two values???????
          message_send(from, &goldMsg);

          // construct and send DISPLAY message to clients
          char* displayStr = getDisplay(players[playerIndex]);

          char displayMsg[nrows*ncols+1000]; // num of chars in map display string + 1000 extra chars
          sprintf(displayMsg, "DISPLAY\n%s", displayStr);
          message_send(from, &displayMsg);
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
      if (validKey(&msgRest,true)){
        // only possible key is Q
        if (strcmp(msgRest,"Q") == 0){
          message_send(from,"QUIT Thanks for watching!");
          deletePlayer(spectator);
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
}

/* parseMsg
 * Helper function of handleMsg that returns array of strings
 * The string in index position 0 indicates msg type (PLAY, SPECTATE, KEY)
 * The string in index position 1 contains the rest of the message
 */
static char** parseMsg(char* msg){
  char** msgParts = calloc(2,sizeof(char*));
  
  msgParts[0] = msg;

  char* ptr = msg;
  while(!isspace(*ptr)){ // slide to first space
    ptr++;
  }
  if (isspace(*ptr)){ // break message into 2 parts
    ptr = "\0";
    ptr++;
    msgParts[1] = ptr;
  }

  return msgParts;
}
    
/* countPlayers
 * Helper function of handleMsg
 * Counts number of players in player_t**
 */
static int countPlayers(player_t** players){
  int count;

  for (count = -1; players[count] != NULL; count++){
  }

  return count;
}

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
    name[maxNamelength] = "\0";
  }

  int index;
  for (index = 0; index < strlen(name); index++){
    if (!isgraph(name[index]) && !isblank(name[index])){
      name[index] = "_";
    }
  }
  return name;
}

/* getID
 * Helper function for handleMsg
 * Gets player ID
 */
static char* getID(int playerIndex){
  char ID = "A";
  int pos;
  for (pos = 0; pos < playerIndex; pos++){
    ID++;
  }
  return &ID;
}

/* validKey
 * Helper function for handleMsg
 * Tests whether key is a valid key
 */
static bool validKey(char key, bool spectator){
  // if is spectator; only valid key is Q
  if (spectator){
    if (key != "Q"){
      return false;
    }
    else{
      return true;
    }
  }
  // if is player
  else{
    bool valid = false;
    char validKeys[17] = {"Q","h","H","l","L","j","J","k","K","y","Y","u","U","b","B","n","N"};
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
static int getPlayerin(player_t** players, addr_t* from){
  int index;
  for (index = 0; index < maxPlayers; index++){
    if (message_eqAddr(from,getSocketAddr(players[index]))){
        return index;
    }
  }
  return NULL;
}

/* getDisplay
 * Helper function for handleMsg
 * Gets display string of player object
 */
static char* getDisplay(player_t* player){
  return getFileMap(getGrid(player));
}

/* endMessage
 * Helper function for handleMsg
 * Constructs game over message when all gold has been collected
 */
static char* endMessage(player_t** players){
  char* msg = "QUIT GAME OVER:";

  int pos;
  for (pos = 0; pos<index; pos++){
    if (players[pos] != NULL){
      char* newLine = malloc(sizeof(char)*100); // 100 chars per line will be more than enough space
      sprintf(newLine,"\n%s %6d %s",getID(pos),getGold(players[pos]),getRealName(players[pos]));
      char* newMsg = malloc(sizeof(msg)+sizeof(newLine)+10);
      sprintf(newMsg,"%s%s",msg,newLine);
      free(msg);
      free(newLine);
      msg = newMsg;
    }
  }
  return msg;
}
