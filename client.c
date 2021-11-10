/* client.c
 * client for nuggets
 * Ellie Boyd
 * Nov 6 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "support/message.h"

/* functions */
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
bool parseArgs(const int argc, char* argv[]);


int main(const int argc, char* argv[])
{
  //parse arguments and check to see if player or spectator
  bool player;
  player = parseArgs(argc, argv);

  //set variables from command line input
  const char* host = argv[1];
  const char* port = argv[2];

  //initialize display using ncurses

  //validate server address
  addr_t server;
  if (!message_setAddr(host, port, &server)) {
    fprintf(stderr, "bad hostname/pot\n");
    return 1;
  }

  //ask server to join game as player/spectator
  char* line;
  if (player) {
    sprintf(line, "PLAY %s", argv[3]);
  }
    
  message_send(server, line);

  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);

  message_done();

  return ok? 0 : 1;

}


bool parseArgs(const int argc, char* argv[])
{

  //check if message module can be initialized
  if (message_init(NULL) == 0) {
    fprintf(stderr, "failure to initialize message module\n");
    exit(1);
  }

  //check for correct number of arguments
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "client takes two (or three) arguments\n");
    exit(1);
  }

  if (argc == 3) {
    return false;
  }

  else {
    return true;
  }
}


static bool handleInput(void* arg) 
{
  //use ncurses to make key input into something the server can deal with
  ;
}

static bool handleMessage(void* arg, const addr_t from, const char* message) {
  if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
    const char* content = message + strlen("PLAY ");
    ... work with content...
  } else...



 



