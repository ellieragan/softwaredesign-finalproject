/* client.c
 * client for nuggets
 * Ellie Boyd
 * Nov 6 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <ncurses.h>
#include <ctype.h>
#include "support/message.h"


/* functions */
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
bool parseArgs(const int argc, char* argv[]);

/* global variables */
char* host;
char* port;
char* playerName;
addr_t server;
bool isPlayer;
int ry, rx;
char letter;
int has, received, unclaimed;


int main(const int argc, char* argv[])
{
  //parse arguments and check to see if player or spectator
  isPlayer = parseArgs(argc, argv);

  //assign variables
  host = argv[1];
  port = argv[2];

  if (isPlayer) {
    playerName = argv[3];
  }

  //validate server address
  if (!message_setAddr(host, port, &server)) {
    fprintf(stderr, "bad hostname/port\n");
    exit(1);
  }

  int stringLength;

  //ask server to join game as player/spectator
  if (playerName != NULL) {
    stringLength = (5 + strlen(playerName));
  }
  else {
    stringLength = 9;
  }

  char line[stringLength];

  if (isPlayer) {
    sprintf(line, "PLAY %s", playerName);
  }
  else {
    sprintf(line, "SPECTATE");
  }
    
  message_send(server, line);

  //start message loop to wait for response
  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
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
  //use ncurses to make key input into something the server can deal with
  char* message = malloc(7);
  char c = getch();

    //player can (attempt to) send any letter
    if (isPlayer) {
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
    message_send(server, message);
    free(message);

    return false;
}

static bool handleMessage(void* arg, const addr_t from, const char* message) {
  
  //get letter representing each player
  if (strncmp(message, "OK ", strlen("OK ")) == 0) {
    char* content = (char*)message + strlen("OK ");
    sscanf(content, "%c", &letter);
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
    getmaxyx(stdscr, ry, rx);
    refresh();

    bool tooSmall = true;
    getmaxyx(stdscr, ry, rx); //get screen size

    //if the screen is bigger than the grid:
    if (ry >= (nrows+1) && rx >= (ncols+1)) {
      tooSmall = false;
    }

    else {
      printw("screen size too small for grid! expand screen and push 'enter'\n");
      refresh();

      while (tooSmall) {
        while (getch() != '\n') { //wait for player to push enter
        ;
        }
  
        getmaxyx(stdscr, ry, rx); //get new screen size
        fprintf(stderr, "%d %d\n", ry, rx);

        //if the screen is bigger than the grid:
        if (ry >= (nrows + 1) && rx >= (ncols + 1)) {
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
    received = 0;
    has = 0;
    unclaimed = 0;

    const char* content = message + strlen("GOLD ");
    sscanf(content, "%d %d %d", &received, &has, &unclaimed);
    refresh();

  }

  //reprint map (and any new messages at the top)
  else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    const char* content = message + strlen("DISPLAY\n");

    clear();
    //printf("%d %d %d\n", has, unclaimed, received);

    //if it is a player:
    if (isPlayer) {

      //if gold has been found on the most recent move:
      if (received > 0) {
        printw("Player %c has %d nuggets (%d nuggets unclaimed). GOLD received: %d\n", letter, has, unclaimed, received);
        received = 0;
      }

      //if gold has not just been found:
      else {
        printw("Player %c has %d nuggets (%d nuggets unclaimed).\n", letter, has, unclaimed);
      }
    }

    //if it is a spectator:
    else {
      printw("Spectator: %d nuggets unclaimed).\n", unclaimed);
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


