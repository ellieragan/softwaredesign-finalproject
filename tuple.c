/*
* tuple.c
* Module for the tuple data structure 
*
* Team 7: Gitting an A
* (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
* CS 50, Fall 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libcs50/mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct tuple{
    int x; 
    int y; 
} tuple_t; 

/**************** global functions ****************/
/* that is, visible outside this file */
/* see tuple.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
// TODO: init here as necessary 

 /************ initTuple ************/
tuple_t* initTuple(int x, int y) 
{
  // validate inputs
  if (x < 0 || y < 0) {
    return NULL;
  }

  tuple_t *tuple = mem_malloc(sizeof(tuple_t));
  tuple->x = x;
  tuple->y = y;

  return tuple;
}

/*********** tuple_delete ************/
void deleteTuple(tuple_t *tuple)
{
  if (tuple != NULL) {
    mem_free(tuple);
  }
}

/*********** tupleGetX ***********/
int tupleGetX(tuple_t* tuple) 
{
  if (tuple != NULL) {
    return tuple->x;
  } else {
    return -1;
  }
}

/*********** tupleGetY ***********/
int tupleGetY(tuple_t* tuple) 
{
  if (tuple != NULL) {
    return tuple->y;
  } else {
    return -1;
  }
} 

/*********** tupleSetX ***********/
void tupleSetX(tuple_t* tuple, int x) 
{
  if (tuple != NULL) {
    tuple->x = x;
  }
}
/*********** tupleSetY ***********/
void tupleSetY(tuple_t* tuple, int y) 
{
  if (tuple != NULL) {
    tuple->y = y;
  }
}
