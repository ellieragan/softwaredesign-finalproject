/* client.c
 * client for nuggets
 * 
 * team 7: Gitting an A
 * (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
 * CS50 21F
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <ncurses.h>
#include <ctype.h>
#include "message.h"


/* functions */
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
bool parseArgs(const int argc, char* argv[]);

/* struct definitions */
typedef struct game 
{
  char* host;
  char* port;
  char* playerName;
  char letter;
  addr_t server;
  bool isPlayer;
  int ry, rx;
  int has, received, unclaimed;
} game_t;

/* global variables */

game_t game;


int main(const int argc, char* argv[])
{
  //parse arguments and check to see if player or spectator
  game.isPlayer = parseArgs(argc, argv);

  //assign variables
  game.host = argv[1];
  game.port = argv[2];

  if (game.isPlayer) {
    game.playerName = argv[3];
  }

  //validate server address
  if (!message_setAddr(game.host, game.port, &game.server)) {
    fprintf(stderr, "bad hostname/port\n");
    exit(1);
  }

  int stringLength;

  //ask server to join game as player/spectator
  if (game.playerName != NULL) {
    stringLength = (5 + strlen(game.playerName));
  }
  else {
    stringLength = 9;
  }

  char line[stringLength];

  if (game.isPlayer) {
    sprintf(line, "PLAY %s", game.playerName);
  }
  else {
    sprintf(line, "SPECTATE");
  }
    
  message_send(game.server, line);

  //start message loop to wait for response
  bool ok = message_loop(&game.server, 0, NULL, handleInput, handleMessage);
  message_done();
  
  return ok? 0 : 1;
}


bool parseArgs(const int argc, char* argv[])
{

  //check if message module can be initialized
  if (message_init(stderr) == 0) {
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
  //gets key input
  char* message = malloc(7);
  char c = getch();

    //player can (attempt to) send any letter
    if (game.isPlayer) {
      if (c != EOF) {
        sprintf(message, "KEY %c", c);
      }
      else {
        message = "KEY Q";
      }
    }

    //spectator may only send Q
    else {
      if (c == 'Q') {
        message = "KEY Q";
      }
    }
    message_send(game.server, message);
    free(message);

    return false;
}

static bool handleMessage(void* arg, const addr_t from, const char* message) {
  
  //get letter representing each player
  if (strncmp(message, "OK ", strlen("OK ")) == 0) {
    char* content = (char*)message + strlen("OK ");
    sscanf(content, "%c", &game.letter);
  } 

  //check to see if screen is big enough to fit grid
  else if (strncmp(message, "GRID ", strlen("GRID ")) == 0) {
    const char* content = message + strlen("GRID ");
    int nrows, ncols;
    sscanf(content, "%d %d", &nrows, &ncols);

    //initialize ncurses
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    getmaxyx(stdscr, game.ry, game.rx);
    refresh();

    bool tooSmall = true;
    getmaxyx(stdscr, game.ry, game.rx); //get screen size

    //if the screen is bigger than the grid:
    if (game.ry >= (nrows+1) && game.rx >= (ncols+1)) {
      tooSmall = false;
    }

    else {
      printw("screen size too small for grid! expand screen and push 'enter'\n");
      refresh();

      while (tooSmall) {
        while (getch() != '\n') { //wait for player to push enter
        ;
        }
  
        getmaxyx(stdscr, game.ry, game.rx); //get new screen size
        fprintf(stderr, "%d %d\n", game.ry, game.rx);

        //if the screen is bigger than the grid:
        if (game.ry >= (nrows + 1) && game.rx >= (ncols + 1)) {
          tooSmall = false;
        }
        else {
          printw("screen size too small for grid! expand screen and push 'enter'\n");
          refresh();
        }
      }
    }
  }

  //keep track of the gold that has just been found, the total found gold, and the undiscovered gold
  else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
    game.received = 0;
    game.has = 0;
    game.unclaimed = 0;

    const char* content = message + strlen("GOLD ");
    sscanf(content, "%d %d %d", &game.received, &game.has, &game.unclaimed);
    refresh();

  }

  //reprint map (and any new messages at the top)
  else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    const char* content = message + strlen("DISPLAY\n");
    clear();

    //if it is a player:
    if (game.isPlayer) {

      //if gold has been found on the most recent move:
      if (game.received > 0) {
        printw("Player %c has %d nuggets (%d nuggets unclaimed). GOLD received: %d\n", game.letter, game.has, game.unclaimed, game.received);
        game.received = 0;
      }

      //if gold has not just been found:
      else {
        printw("Player %c has %d nuggets (%d nuggets unclaimed).\n", game.letter, game.has, game.unclaimed);
      }
    }

    //if it is a spectator:
    else {
      printw("Spectator: %d nuggets unclaimed).\n", game.unclaimed);
    }
    
    //print map
    printw("%s", content);
    refresh();
  }

  //exit game
  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    const char* content = message + strlen("QUIT ");
    
    //shut down ncurses
    attroff(COLOR_PAIR(1));
    endwin();

    //print quit message
    printf("%s\n", content);
    return true;
  
  }

  else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
    ;
    //logs error and continues
  }

  return false;
}


