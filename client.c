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
addr_t* server;
bool isPlayer;
//const int nrows;
//const int ncols;
int ry, rx;


int main(const int argc, char* argv[])
{
  //parse arguments and check to see if player or spectator
  isPlayer = parseArgs(argc, argv);

  //assign variables
  host = argv[1];
  port = argv[2];
  server = malloc(sizeof(addr_t));

  //initialize message_init
  //if (message_init(stderr) == 0) {
    //fprintf(stderr, "failure to initialize message module\n");
    //exit(1);
  //}

  //set variables from command line input
  //const char* host = argv[1];
  //const char* port = argv[2];

  //validate server address
  //addr_t* server;
  if (!message_setAddr(host, port, server)) {
    fprintf(stderr, "bad hostname/pot\n");
    exit(1);
  }

  //ask server to join game as player/spectator
  char* line = NULL;
  if (isPlayer) {
      sprintf(line, "PLAY %s", playerName);
    //}
    //else {
      //strncpy(playerName, argv[3], nameLength);
      //sprintf(line, "
  }

  else {
    sprintf(line, "SPECTATE");
  }
    
  message_send(*server, line);

  //initialize ncurses
  initscr(); //screen
  cbreak();
  noecho();
  //start_color();
  //init_pair(1, COLOR_RED, COLOR_BLACK);
  //attron(COLOR_PAIR(1));
  //attroff(COLOR_PAIR(1));
  //int ly, lx; //upper left corner
  //getbegyx(stdscr, ly, lx);
  //int ry, rx; //lower right corner
  getmaxyx(stdscr, ry, rx);


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
  char* message = NULL;
  char c = getch();
    if (isPlayer) {
      if (c != EOF) {
        sprintf(message, "KEY %c", c);
      }
      else {
        message = "KEY Q";
      }
    }
    else {
      if (c == 'Q') {
        message = "KEY Q";
      }
    }
    message_send(*server, message);

  //while ((c
    //switch(c) {
      //case (Q): message_send(*server, "KEY Q");
      //case h: message_send(*server, "KEY h");
      //case H: message_send(*server, "KEY H");
      //case l: message_send(*server, "KEY l");
      //case L: message_send(*server, "KEY L");
      //case j: message_send(*server, "KEY j");
      //case J: message_send(*server, "KEY J");
      //case k: message_send(*server, "KEY k");
      //case K: message_send(*server, "KEY K");
      //case y: message_send(*server, "KEY y");
      //case Y: message_send(*server, "KEY Y");
      //case u: message_send(*server, "KEY u");
      //case U: message_send(*server, "KEY U");
      //case b: message_send(*server, "KEY b");
      //case B: message_send(*server, "KEY B");
      //case n: message_send(*server, "KEY n");
      //case N: message_send(*server, "KEY N");
    //}
    return false;
}

static bool handleMessage(void* arg, const addr_t from, const char* message) {
  if (strncmp(message, "OK ", strlen("OK ")) == 0) {
    ;
    //const char* content = message + strlen("OK ");
    //do something with content???
  } 
  else if (strncmp(message, "GRID ", strlen("GRID ")) == 0) {
    const char* content = message + strlen("GRID ");
    int nrows, ncols;
    sscanf(content, "GRID %d %d", &nrows, &ncols);

    bool tooSmall = true;

    if (nrows >= (ry+1) && ncols >= (rx+1)) {
      tooSmall = false;
    }

    else {
      printw("screen size too small for grid!\n");

      while (tooSmall) {

        endwin();
        initscr();
        printw("screen size too small for grid!\n");
        refresh();
        getmaxyx(stdscr, ry, rx);

        if (nrows >= (ry + 1) && ncols >= (rx + 1)) {
          tooSmall = false;
        }
      }
    }
  }

  else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
    const char* content = message + strlen("GOLD ");
    printw("%s\n", content);
  }

  else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    const char* content = message + strlen("DISPLAY\n");
    char* gridMap = NULL;
    sscanf(content, "DISPLAY\n%s", gridMap);
    printw("%s", gridMap);
  }

  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    const char* content = message + strlen("QUIT ");

    endwin();
    free(host);
    free(port);
    free(server);
    message_done();

    char* whyQuit = NULL;
    sscanf(content, "QUIT %s", whyQuit);
    printf("%s\n", whyQuit);

    free(whyQuit);
  }

  else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
    const char* content = message + strlen("ERROR ");

    char* error = NULL;
    sscanf(content, "ERROR %s", error);
    printw("%s\n", error);
  }

  return false;
}
