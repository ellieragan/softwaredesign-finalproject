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
//const int nrows;
//const int ncols;
int ry, rx;


int main(const int argc, char* argv[])
{
  //parse arguments and check to see if player or spectator
  isPlayer = parseArgs(argc, argv);

  //assign variables
  host = argv[1];
  printf("%s\n", host);
  port = argv[2];
  printf("%s\n", port);

  if (isPlayer) {
    playerName = argv[3];
    printf("%s\n", playerName);
  }
  //if (!message_setAddr(host, port, server)) {
    //fprintf
  //server = malloc(sizeof(addr_t));

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
    //}
    //else {
      //strncpy(playerName, argv[3], nameLength);
      //sprintf(line, "
  }

  else {
    sprintf(line, "SPECTATE");
  }
    
  message_send(server, line);
  //free(line);

  //initialize ncurses
  initscr(); //screen
  printf("initted\n");
  cbreak();
  noecho();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  //attroff(COLOR_PAIR(1));
  //int ly, lx; //upper left corner
  //getbegyx(stdscr, ly, lx);
  //int ry, rx; //lower right corner
  getmaxyx(stdscr, ry, rx);
  refresh();

  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
  message_done();
  //free(*server);
  return ok? 0 : 1;

}


bool parseArgs(const int argc, char* argv[])
{

  //check if message module can be initialized
  if (message_init(stderr) == 0) {
    fprintf(stderr, "failure to initialize message module\n");
    //free(server);
    exit(1);
  }

  //check for correct number of arguments
  if (argc < 3 || argc > 4) {
    //free(server);
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
    message_send(server, message);

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
    free(message);
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
    sscanf(content, "%d %d", &nrows, &ncols);

    bool tooSmall = true;
    getmaxyx(stdscr, ry, rx);

    if (ry >= (nrows+1) && rx >= (ncols+1)) {
      tooSmall = false;
    }

    else {
      printw("screen size too small for grid! expand screen and push 'enter'\n");
      refresh();

      while (tooSmall) {
        while (getch() != '\n') {
        ;
        }
        //refresh();
        getmaxyx(stdscr, ry, rx);
        fprintf(stderr, "%d %d\n", ry, rx);

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

  else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
    const char* content = message + strlen("GOLD ");
    printw("%s\n", content);
    refresh();
  }

  else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    const char* content = message + strlen("DISPLAY\n");
    //char* gridMap = NULL;
    //sscanf(content, "%s", gridMap);
    printf("\nhandled correctly\n");
    printw("%s", content);
    refresh();
  }

  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    const char* content = message + strlen("QUIT ");

    endwin();
    free(host);
    free(port);
    //free(server);
    //message_done();

    char* whyQuit = NULL;
    sscanf(content, "%s", whyQuit);
    printf("%s\n", whyQuit);
    attroff(COLOR_PAIR(1));

    free(whyQuit);
  }

  else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
    const char* content = message + strlen("ERROR ");

    char* error = NULL;
    sscanf(content, "%s", error);
    printw("%s\n", error);
    refresh();
  }

  return false;
}
