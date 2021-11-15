/* 
 * gridtest.c serves as a testing program for the grid module
 *
 * Jeffrey Liu, November 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "tuple.h"
#include "grid.h"

/* 
*
* Main method that drives the tester for the grid module
*/
int main() 
{
  char* filemap;
  int rows;                    
  int cols;                
  int goldDecremented;
  char* visible;

  grid_t* grid = NULL;
  printf("Testing grid_new...\n\n");

  printf("Creating grid1... \n");
  grid_t* grid1 = grid_new("maps/main.txt", getpid());
  if (grid1 == NULL)
  {
    fprintf(stderr, "The initialization failed!\n");
    return 1;
  }
  buildPiles(1, grid1);

  printf("rows: %d\n", getRows(grid1));
  printf("cols: %d\n", getCols(grid1));
  printf("goldLeft: %d\n", getGoldLeft(grid1));
  printf("goldPiles: %d\n", getNumPiles(grid1));
  printf("filemap:\n%s", getFileMap(grid1));
  printf("----------------------\n\n\n");


  printf("Testing getFileMap...\n\n");

  printf("Test with grid that is NULL...\n");
  filemap = getFileMap(grid);
  if (filemap != NULL)
  {
    printf("getFileMap map returns...\n%s", filemap);
  } 
  else 
  {
    printf("getFileMap map returns NULL n\n");
  }

  printf("Test with grid1...\n");
  filemap = getFileMap(grid1);
  if (filemap != NULL)
  {
    printf("getFileMap map returmns...\n%s", filemap);
  } else
  {
    printf("getFileMap map returns NULL\n\n");
  }

  printf("----------------------\n\n\n");

  printf("Testing getCols...\n\n");

  printf("Testing grid1...\n");
  cols = getCols(grid1);
  printf("getCols returns %d\n", cols);

  printf("----------------------\n\n\n");

  printf("Testing getRows...\n\n");

  printf("Testing grid1...\n");
  rows = getRows(grid1);
  printf("getRows returns %d\n", rows);

  printf("----------------------\n\n\n");

  printf("Testing gold collection methods...\n\n");

  tuple_t* one = initTuple(1, 1); //Initializes a tuple that represents a testing point

  printf("Testing with the grid that is NULL...\n");
  goldDecremented = updateGoldCount(grid, one);
  printf("grid_updateGold returned with value %d\n\n", goldDecremented);

  printf("Testing with grid1...\n");
  printf("Intends to iterate through map and collect all pieces of gold.\n");
  int point;
  for(point = 0; filemap[point] != '*'; point++);
  while (point != strlen(filemap) && filemap[point] == '*') //Iterates through the filemap and looks for gold 
  {
    int colCord = (point % cols) +1; 
    int rowCord = (point / (cols - 1)) + 1;
    tuple_t* cords = initTuple(colCord, rowCord); 

    if (point == charConvertIndexNum(grid1, colCord, rowCord)) //Checks if hte convert index function works
    {
      printf("The one and two-dimensional indexes correspond!\n");
    }
    else
    {
      printf("The point index is %d which isn't equal to the one-dimensional index which is %d!\n", point, charConvertIndexNum(grid1, colCord, rowCord));
    }
    
    printf("Remove gold from (%d,%d)\n", colCord, rowCord);
    goldDecremented = updateGoldCount(grid1, cords);
    printf("Removed %d gold from (%d,%d)\n", goldDecremented, colCord, rowCord);
    printf("%s\n", filemap);

    for(point++; filemap[point] != '*' && point != strlen(filemap); point++);
  }
  printf("----------------------\n\n\n");
 

  printf("Testing updateVisibility...\n\n"); 
  
  visible = malloc((strlen(filemap) * sizeof(char) + 1));
  strcpy(visible, filemap);
  for (int i = 0; i < strlen(filemap); i++) 
  {
    if (filemap[i] == '\n')
    {
      visible[i] = '\n';
    }
    else 
    {
      visible[i] = '3'; //Initializes a visibility array by setting all to 3, reepresenting not visible
    }
  }
  visible[strlen(filemap)] = '\0';

  char* updated = malloc(sizeof(char) * (strlen(filemap) + 1));
  printf("Testing the grid that is NULL...\n");
  updated = updateVisibility(grid, 1, 1, visible);
  printf("The visibility array represents:\n%s", updated);

  printf("Testing with grid1...\n");
  printf("Updating the visibility array given the position (6, 2)\n");
  updated = updateVisibility(grid1, 6, 2, visible);
  printf("The visibility array represents:\n%s", updated);

  printf("----------------------\n\n\n");

  printf("Testing gridDelete\n\n"); 

  printf("Deleting grid1...\n");
  delete(grid1);

  return 0;
}
