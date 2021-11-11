/*
* grid.c file 
*
* Jeffrey Liu, November 2021
*/


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "file.h"
#include "grid.h"

static const char room = '.';
static const char pile = '*';

static const int minGold = 10;
static const int maxGold = 30;
static const int totalGold = 250;
static const int maxPlayers = 26;

static const int vis = 1; 
static const int alrVis = 2;
static const int notVis = 3;



typdef struct grid grid_t
{
    char* filemap;
    int rows;
    int cols;
    int goldPiles;
    int goldLeft;
} grid_t;

/*
* Reads the input map from the server module and the seed to create the map. Sets
* gold values as asteriks through predetermined minGold and maxGold as per specs.
*
* Inputs: map filename, seed
* Outputs: if found, a grid initialized with gold, gold remaining, height, width 
*
*/

grid_t* grid_new(char* filename, int seed)
{
    FILE* fp;
    fp = fopen(filename, "r");
    int rowNum = 0;
    int colNum = 0;
    if(fp != NULL)
    {
        rowNum = file_numLines(fp);
        char* cols = file_readLine(fp);
        colNum = strlen(cols);
        free(cols);
        fclose(fp);
    }

    char start[colNum * rowNum];
    int totalCount = 0;
    int roomCount = 0;
    char temp;
    while((temp = fgetc(fp)) != EOF)
    {
        start[totalCount] = temp; 
        if(temp == room)
        {
            roomCount++;
        }
        totalCount++;

    }
    fclose(fp);
    start[totalCount] = '\0';

    char* fileMapHold = malloc((rowNum * colNum + 10) * sizeof(char));
    if(fileMapHold != NULL)
    {
        strcpy(fileMapHold, start);        
    }

    grid_t* grid = grid_newHelper(fileMapHold, rowNum, colNum);
    buildPiles(seed, grid);
}

/*
*/
grid_t* grid_newHelper(char* fileMap, int rows, int cols)
{
    grid_t* mapGrid = malloc(sizeof(grid_t));
    if(mapGrid != NULL)
    {
        mapGrid -> filemap = fileMapHold; 
        mapGrid -> rows = rowNum;
        mapGrid -> cols = colNum; 
    }
    return mapGrid;
}

/*
* Returns a string that represents the map
*
* Inputs: a player's grid
* Output: if found, a map's string 
*
*/
char* getFileMap(grid_t* playerGrid)
{
    if(playerGrid != NULL)
    {
        char* fileMap = playerGrid -> filemap;
        return fileMap;
    }
    return; 
}

/*
* Returns the number of rows in a grid
*
* Inputs: a player's grid
* Output: if found, a map's row count
*/
int getRows(grid_t* playerGrid)
{
    if(playerGrid != NULL)
    {
        int rows = playerGrid -> rows;
        return rows;
    }
    return; 
    
}

/*
* Returns the number of columns in a grid
*
* Inputs: a player's grid
* Output: if found, a map's column count
*/
int getCols(grid_t* playerGrid)
{
    if(playerGrid != NULL)
    {
        int cols = playerGrid -> cols;  
        return cols;
    }
    return; 
}

/*
* Returns the number of gold piles in a grid
*
* Inputs: a player's grid
* Output: if found, a map's gold pile count
*/
int getNumPiles(grid_t* playerGrid)
{
    if(playerGrid != NULL)
    {
        int numPiles = playerGrid -> goldPiles;
        return numPiles;
    }
    return;
    
}

/*
* Returns the amount of gold that is left within a grid
*
* Inputs: a player's grid
* Output: if found, the count of the gold left in a map 
*/
int getGoldLeft(grid_t* playerGrid)
{
    if(playerGrid != NULL)
    {
        int goldLeft = playerGrid -> goldLeft;
        return goldLeft;
    }
    return;   
}

/*
* Randomly generates a number of gold piles based on the min and max gold pile vlaues.
* Then puts the gold, represented by a '*' character, on the map
*
* Inputs: a seed, a player's grid 
* Output: N/A
*/
void buildPiles(int seed, grid_t* playerGrid)
{
    int piles = (int)(rand() % (maxGold - minGold + 1)) + minGold; 

    grid -> goldPiles = piles; 
    grid -> goldLeft = totalGold;

    int i = 0;
    while(i < piles)
    {
        int row = grid -> row;
        int col = grid -> col; 
        int randRow = (rand() % row) + 1;
        int randCol = (rand() % col) + 1;

        int index = (randRow - 1) * col + randCol - 1;
        if (grid -> filemap[index] == room)
        {
            grid -> filemap[index] = pile;
            i++; 
        }
    }
}

/*
* Takes gold out of the map's string and decreases the count of the gold by
* a random amount so long as each remaining pile of gold maintains at least 
* one nugget.
* 
* Inputs: a player's grid, the count 
* Output: the amount of gold removed. 
*/
int updateGoldCount(grid_t* playerGrid, int goldDecrease, int rowCord, int colCord)
{
    if(playerGrid != NULL)
    {
        int removeGold;
        if(playerGrid -> goldPiles == 1)
        {
            removeGold = playerGrid -> goldLeft; 
        }
        else
        {
            removeGold = (int) (rand() % (2* (playerGrid -> goldLeft/playerGrid -> goldPiles) - playerGrid-> goldPiles + 1)); 
        }

        int index = (rowCord - 1) * playerGrid -> col + colCord - 1;
        playerGrid -> filemap[index] = room; 
        if(playerGrid -> goldPiles >= 1 && playerGrid -> goldLeft >= removeGold)
        {
            playerGrid -> goldLeft = playerGrid -> goldLeft - removeGold;
            playerGrid -> goldPiles --; 
            return removeGold; 
        }
    }
    return 0;
}

/*
*
*/
void buildUpdatedVisibility(char* updatedVis, char* currentVis)
{
    strpy(updatedVis, currentVis);
    for(int i = 0; i < strlen(updatedVis); i++)
    {
        if(updatedVis[i] = vis)
        {
            updatedVis[i] = alrVis;
        }
    }
}


/*
* Updates the grid visibility for a new player position via a visibility array
* 
* Inputs: a player's grid, a player's location
* Output: N/A
*
*/
char* updateVisibility(grid_t* playerGrid, int rowCord, int colCord, char* visibility)
{
    char* updatedVis;
    buildUpdatedVisibility(updatedVis, visibility);

    if(playerGrid != NULL)
    {
        for(int r = 1; r < playerGrid -> rows; r++)
        {
            for(int c = 1; c < playerGrid -> cols; c++)
            {
                float slope = slopeCalc(r, c, rows, cols);
                float intercept = interceptCalc(r, c, slope);
                int index = (r - 1) * playerGrid -> col + c - 1;
                bool visible;
                visible = rowCheck(r, c, row, col, slope, intercept, playerGrid); 
                visible = colCheck(r, c, row, col, slope, intercept, playerGrid);

                if(visible)
                {
                    updatedVis[index] = vis;
                }
                else if (updatedVis[index] != alrVis)
                {
                    updatedVis[index] = notVis;
                }
            }
        }
    }
    return updatedVis;
}
 

/*
*/
bool colCheck(int r, int c, int row, int col, float slope, float intercept, grid_t* playerGrid)
{
    int ctemp = c; 
    while(ctemp != col) //Iterates until reaches grid's column 
    {
        if(ctemp != c) //As long as not observing current col spot
        {
            float curRow;
            if(r == row)
            {
                curRow = (ctemp - intercept)/slope;
            }
            else
            {
                curRow = (float)r;
            }
            int low = (int) curRow;
            int high = (int) ceil(curRow);
            if(!validSpot(grid, ctemp, high) && !validSpot(grid, ctemp, low))
            {
                return false; 
            }
        }
        if(ctemp <= col)
        {
            rtemp++; 
        }
        else
        {
            rtemp--; 
        }
    }
    return false;
}

/*
*/
bool rowCheck(int r, int c, int row, int col, float slope, float intercept, grid_t* playerGrid)
{
    int rtemp = r; 
    while(rtemp != row)
    {
        if(rtemp != r)
        {
            float curCol;
            if(c == col)
            {
                curCol = (rtemp - intercept)/slope;
            }
            else
            {
                curCol = (float)c;
            }
            int low = (int) curCol;
            int high = (int) ceil(curCol);
            if(!validSpot(grid, high, rtemp) && !validSpot(grid, low, rtemp))
            {
                return false; 
            }
        }
        if(rtemp <= row)
        {
            rtemp++; 
        }
        else
        {
            rtemp--; 
        }
    }
    return false;
}


/*
*/
float slopeCalc(int r, int c, int row, int col)
{
    float slope = ((float) (r - row)) / ((float) (c - col));
    return slope;
}

/*
*/
float interceptCalc(int r, int c, float slope)
{
    float intercept = (float)r - (slope * (float)c);
    return intercept;
}


bool validSpot(grid_t* playerGrid, int row, int col)
{
    int index = (row - 1) * playerGrid -> cols + col - 1;
    return (playerGrid -> filemap[index] == room || playerGrid -> filemap[index] == pile);
}


/*
* Deletes the player's grid by freeing its memory 
*   
* Inputs: a player's grid
* Output: N/A
*/
void delete(grid_t* playerGrid)
{
    if(playerGrid != NULL && playerGrid -> filemap != NULL)
    {
        free(playerGrid -> filemap);
        free(playerGrid);
    }
    else if (playerGrid != NULL)
    {
        free(playerGrid);
    }
}

