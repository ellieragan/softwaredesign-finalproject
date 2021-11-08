/*
* tuple.h
* declaration & header file for tuple struct
*
* Team 7: Gitting an A
* (Ellie Boyd, Jeffrey Liu, Brian Ng, Donia Tung)
* CS 50, Fall 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

/**************** global types ****************/
typedef struct tuple tuple_t; // TODO: this could also be defined in grid, which might make more sense 

/**************** functions ****************/

 /************ initTuple ************/
tuple_t* initTuple(int x, int y); 

/*********** tuple_delete ************/
void tuple_delete(tuple_t *tuple); 

/*********** tupleGetX ***********/
int tupleGetX(tuple_t* tuple); 

/*********** tupleGetY ***********/
int tupleGetY(tuple_t* tuple); 

/*********** tupleSetx ***********/
void tupleSetx(tuple_t* tuple, int x); 

/*********** tupleSetY ***********/
void tupleSetY(tuple_t* tuple, int y); 