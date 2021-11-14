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



typedef struct grid
{
    char* filemap;
    int rows;
    int cols;
    int goldPiles;
    int goldLeft;
} grid_t;

static const char room = '.';
static const char pile = '*';

static const int minGold = 10;
static const int maxGold = 30;
static const int totalGold = 250;
//static const int maxPlayers = 26;

static const char vis = '1'; 
static const char alrVis = '2';
static const char notVis = '3';


grid_t* grid_new(char* filename, int seed);
grid_t* grid_newHelper(char* fileMap, int rows, int cols);
char* getFileMap(grid_t* masterGrid);
int getRows(grid_t* masterGrid);
int getCols(grid_t* masterGrid);
int getNumPiles(grid_t* masterGrid);
int getGoldLeft(grid_t* masterGrid);
void buildPiles(int seed, grid_t* masterGrid);
int updateGoldCount(grid_t* masterGrid, int seed, int colCord, int rowCord);
void buildUpdatedVisibility(char* updatedVis);
char* updateVisibility(grid_t* masterGrid, int colCord, int owCord, char* visibility); 
bool colCheck(int row, int col, int rowCord, int colCord, float slope, float intercept, grid_t* masterGrid);
bool rowCheck(int row, int col, int rowCord, int colCord, float slope, float intercept, grid_t* masterGrid);
float slopeCalc(int r, int c, int row, int col);
float interceptCalc(int r, int c, float slope);
bool validSpot(grid_t* masterGrid, int row, int col);
void delete(grid_t* masterGrid);
char* charConvertIndex(grid_t* masterGrid, char** gridMap);
int charConvertIndexNum(grid_t* masterGrid, int row, int col);
char* initializeVisibility(grid_t* masterGrid, int row, int col);




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
        colNum = strlen(cols) + 1;
        free(cols);
        fclose(fp);
    }
    else
    {
        return NULL;
    }

    char start[colNum*rowNum + 10];
    int totalCount = 0;
    int roomCount = 0;
    char temp;
    fp = fopen(filename, "r");
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

    grid_t* masterGrid = grid_newHelper(start, rowNum, colNum);
    buildPiles(seed, masterGrid);

    return masterGrid;
}

/*
* Method serves as a helper for the grid 
*
*
*/
grid_t* grid_newHelper(char* fileMap, int rows, int cols)
{
    char* fileMapHold = malloc((rows * cols) + (10 * sizeof(char)));
    if(fileMapHold != NULL)
    {
        strcpy(fileMapHold, fileMap);        
    }

    grid_t* mapGrid = malloc(sizeof(grid_t));
    if(mapGrid != NULL)
    {
        mapGrid -> filemap = fileMapHold; 
        mapGrid -> rows = rows;
        mapGrid -> cols = cols; 
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
char* getFileMap(grid_t* masterGrid)
{
    if(masterGrid != NULL && masterGrid -> filemap != NULL)
    {
        return masterGrid -> filemap;
    }
    return NULL; 
}

/*
* Returns the number of rows in a grid
*
* Inputs: a player's grid
* Output: if found, a map's row count
*/
int getRows(grid_t* masterGrid)
{
    if(masterGrid != NULL)
    {
        int rows = masterGrid -> rows;
        return rows;
    }
    return -1; 
    
}

/*
* Returns the number of columns in a grid
*
* Inputs: a player's grid
* Output: if found, a map's column count
*/
int getCols(grid_t* masterGrid)
{
    if(masterGrid != NULL)
    {
        int cols = masterGrid -> cols;  
        return cols;
    }
    return -1; 
}

/*
* Returns the number of gold piles in a grid
*
* Inputs: a player's grid
* Output: if found, a map's gold pile count
*/
int getNumPiles(grid_t* masterGrid)
{
    if(masterGrid != NULL)
    {
        int numPiles = masterGrid -> goldPiles;
        return numPiles;
    }
    return -1; 
    
}

/*
* Returns the amount of gold that is left within a grid
*
* Inputs: a player's grid
* Output: if found, the count of the gold left in a map 
*/
int getGoldLeft(grid_t* masterGrid)
{
    if(masterGrid != NULL)
    {
        int goldLeft = masterGrid -> goldLeft;
        return goldLeft;
    }
    return -1; 
}

/*
* Randomly generates a number of gold piles based on the min and max gold pile vlaues.
* Then puts the gold, represented by a '*' character, on the map
*
* Inputs: a seed, a player's grid 
* Output: N/A
*/
void buildPiles(int seed, grid_t* masterGrid)
{
    int piles = (int)(rand() % (maxGold - minGold + 1)) + minGold; 

    masterGrid -> goldPiles = piles; 
    masterGrid -> goldLeft = totalGold;

    int i = 0;
    while(i < piles)
    {
        int row = masterGrid -> rows;
        int col = masterGrid -> cols; 
        int randRow = (rand() % row) + 1;
        int randCol = (rand() % col) + 1;

        int index = (randRow - 1) * col + randCol - 1;
        if (masterGrid -> filemap[index] == room)
        {
            masterGrid -> filemap[index] = pile;
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
int updateGoldCount(grid_t* masterGrid, int seed, int colCord, int rowCord)
{
    if(masterGrid != NULL)
    {
        int removeGold;
        if(masterGrid -> goldPiles == 1)
        {
            removeGold = masterGrid -> goldLeft; 
        }
        else
        {
            removeGold = (int) (rand() % (2* (masterGrid -> goldLeft/masterGrid -> goldPiles) - masterGrid-> goldPiles + 1)); 
        }

        int index = (rowCord - 1) * masterGrid -> cols + colCord - 1;
        masterGrid -> filemap[index] = room; 
        if(masterGrid -> goldPiles >= 1 && masterGrid -> goldLeft >= removeGold)
        {
            masterGrid -> goldLeft = masterGrid -> goldLeft - removeGold;
            masterGrid -> goldPiles --; 
            return removeGold; 
        }
    }
    return 0;
}

/*
* Constructs the updated visibility array that converts visible items into
* memory for reference.
*
* Inputs: an empty updatedVis array, the current visibility array
* Output: N/a
*/
void buildUpdatedVisibility(char* updatedVis)
{
    for(int i = 0; i < strlen(updatedVis); i++)
    {
        if(updatedVis[i] == vis)
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
char* updateVisibility(grid_t* masterGrid, int colCord, int rowCord, char* visibility)
{
    char* updatedVis = malloc(strlen(visibility) + 1);
    strcpy(updatedVis, visibility);

    buildUpdatedVisibility(updatedVis);
    
    if(masterGrid != NULL)
    {
        for(int col = 1; col < masterGrid -> cols; col++)
        {
            for(int row = 1; row < masterGrid -> rows + 1; row++)
            {
                float slope = slopeCalc(row, col, rowCord, colCord);

                float intercept = interceptCalc(row, col, slope);

                int index = (row - 1) * masterGrid -> cols + (col - 1);
                bool visibleRow;
                bool visibleCol;
                bool visibleBoth = false;

                visibleRow = rowCheck(row, col, rowCord, colCord, slope, intercept, masterGrid); 
                visibleCol = colCheck(row, col, rowCord, colCord, slope, intercept, masterGrid);
               
                if(visibleCol && visibleRow)
                {
                    visibleBoth = true; 
                }

                if(visibleBoth)
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
* Checks the points in between the column of the iteration and the column coordinate
* of the player to determine visibility. Helper function for update visibility
* Inputs: Two sets of row and column values, the slope, the intercept, and the grid.
* Outputs: true if valid spots; otherwise, false
*
*/
bool colCheck(int row, int col, int rowCord, int colCord, float slope, float intercept, grid_t* masterGrid)
{
    int ctemp = col; 
    while(ctemp != colCord) //Iterates until reaches grid's column 
    {
        if(ctemp != col) //As long as not observing current col spot
        {
            float curRow = (ctemp * slope) + intercept;
            int low = (int) curRow;
            int high = (int) ceil(curRow);
            if(!validSpot(masterGrid, high, ctemp) && !validSpot(masterGrid, low, ctemp))
            {
                return false; 
            }
        }
        if(ctemp < colCord)
        {
            ctemp++; 
        }
        else
        {
            ctemp--; 
        }
    }
    return true;
}

/*
* Checks the points in between the row of the iteration and the row coordinate
* of the player to determine visibility. Helper function for update visibility
* Inputs: Two sets of row and column values, the slope, the intercept, and the grid.
* Outputs: true if valid spots; otherwise, false
*/
bool rowCheck(int row, int col, int rowCord, int colCord, float slope, float intercept, grid_t* masterGrid)
{
    int rtemp = row; 
    while(rtemp != rowCord)
    {
        if(rtemp != row)
        {
            float curCol = (float)col;
            if(col != colCord)
            {
                curCol = (rtemp - intercept)/slope;
            }
            int low = (int) curCol;
            int high = (int) ceil(curCol);
            if(!validSpot(masterGrid, rtemp, high) && !validSpot(masterGrid, rtemp, low))
            {
                return false; 
            }
        }
        if(rtemp < rowCord)
        {
            rtemp++; 
        }
        else
        {
            rtemp--; 
        }
    }
    return true;
}


/*
* Calculates the slope.
* Input: Two sets of row and column values
* Output: A float that represents the slope
*/
float slopeCalc(int r, int c, int row, int col)
{
    float slope = ((float) (r - row)) / ((float) (c - col));
    return slope;
}

/*
* Calculates the intercept.
* Input: A row and column value, the slope.
* Output: the intercept
*/
float interceptCalc(int r, int c, float slope)
{
    float intercept = (float)r - (slope * (float)c);
    return intercept;
}

/*
* Checks whether a spot is a valid.
* Inputs: The master a grid, a row and column value
* Output: true if the spot is valid; otherwise, false.
*/
bool validSpot(grid_t* masterGrid, int row, int col)
{
    int index = (row - 1) * masterGrid -> cols + col - 1;
    return (masterGrid -> filemap[index] == room || masterGrid -> filemap[index] == pile);
}


/*
* Deletes the player's grid by freeing its memory 
*   
* Inputs: a player's grid
* Output: N/A
*/
void delete(grid_t* masterGrid)
{
    if(masterGrid != NULL && masterGrid -> filemap != NULL)
    {
        free(masterGrid -> filemap);
        free(masterGrid);
    }
    else if (masterGrid != NULL)
    {
        free(masterGrid);
    }
}


/*
* Initializes a visibility array that keeps track of a player's visibiility.
*
* Input: the mastergrid, the row, and column
* Output: a visibility array
*
*/
char* initializeVisibility(grid_t* masterGrid, int row, int col)
{
    char* temp = masterGrid -> filemap;
    char* visibility;
    visibility = malloc(strlen(temp) * sizeof(char) + 1);
    if(visibility != NULL)
    {
        for(int i = 0; i < strlen(temp) + 1; i++)
        {
            if(i == strlen(temp))
            {
                visibility[i] = '\0';
            }
            else if(temp[i] != '\n')
            {
                visibility[i] = '\n';
            }
            else
            {
                visibility[i] = vis;
            }
        }
        int index = (row - 1) * (masterGrid -> cols) + col -1;
        visibility[index] = vis;
        return visibility;
    }
    return NULL;
}


/*
* Method that converts a row and column coordinate into an one-dimensional index for 
* the input purposes of a filemap. 
*
* Inputs: a master grid, the row coordinate of a player, the column coordinate of a player
* Output: an integer that represents an index 
*/
int charConvertIndexNum(grid_t* masterGrid, int col, int row)
{
    if(masterGrid != NULL)
    {
        return (row - 1) * masterGrid -> cols + (col -1);
    }
    return -1; 
}
