/*
* grid.h file which serves as a header for the grid module
*
* Jeffrey Liu, November 2021
*/

typedef struct grid grid_t; 

/*
* Reads the input map from the server module and the seed to create the map. Sets
* gold values as asteriks through predetermined minGold and maxGold as per specs.
*
* Inputs: map filename, seed
* Outputs: if found, a grid initialized with gold, gold remaining, height, width 
*
*/
grid_t* grid_new(char* filename, int seed);
grid_t* grid_newHelper(char* fileMap, int rows, int cols);
char* getFileMap(grid_t* masterGrid);
int getRows(grid_t* masterGrid);
int getCols(grid_t* masterGrid);
int getNumPiles(grid_t* masterGrid);
int getGoldLeft(grid_t* masterGrid);
void buildPiles(int seed, grid_t* masterGrid);
int updateGoldCount(grid_t* masterGrid, tuple_t* position);
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
char* gridFromVisibility(grid_t* masterGrid, char* spectatorGrid, char* playerVisibility, tuple_t* position); 


/*
* Returns a string that represents the map
*
* Inputs: a player's grid
* Output: if found, a map's string 
*
*/
char* getFileMap(grid_t* playerGrid);

/*
* Returns the number of rows in a grid
*
* Inputs: a player's grid
* Output: if found, a map's row count
*/
int getRows(grid_t* playerGrid);

/*
* Returns the number of columns in a grid
*
* Inputs: a player's grid
* Output: if found, a map's column count
*/
int getCols(grid_t* playerGrid);

/*
* Returns the number of gold piles in a grid
*
* Inputs: a player's grid
* Output: if found, a map's gold pile count
*/
int getNumPiles(grid_t* playerGrid);

/*
* Returns the amount of gold that is left within a grid
*
* Inputs: a player's grid
* Output: if found, the count of the gold left in a map 
*/
int getGoldLeft(grid_t* playerGrid);

/*
* Randomly generates a number of gold piles based on the min and max gold pile vlaues.
* Then puts the gold, represented by a '*' character, on the map
*
* Inputs: a seed, a player's grid 
* Output: N/A
*/
void buildPiles(int seed, grid_t* playerGrid);

/*
* Takes gold out of the map's string and decreases the count of the gold by
* a random amount so long as each remaining pile of gold maintains at least 
* one nugget.
* 
* Inputs: a player's grid, the count 
* Output: the amount of gold removed. 
*/
int updateGoldCount(grid_t* masterGrid, tuple_t* position);

/*
* Updates the grid visibility for a new player position via a visibility array
* 
* Inputs: a player's grid, a player's location
* Output: N/A
*
*/
char* updateVisibility(grid_t* masterGrid, int rowCord, int colCord, char* visibility); 

char* initializeVisibility(grid_t* masterGrid, int row, int col);

bool validSpot(grid_t* masterGrid, int row, int col);

bool isGold(grid_t* grid, tuple_t* location);

int charConvertIndexNum(grid_t* masterGrid, int row, int col);


/*
* Deletes the player's grid by freeing its memory 
*   
* Inputs: a player's grid
* Output: N/A
*/
void delete(grid_t* playerGrid);

/*
* Updates the spectator grid by revealing all items 
*
* Inputs: a spectator's grid
* Output: a spectator grid that contains complete visibility of the map
*/
void updateSpectatorGrid(grid_t* spectatorGrid, grid_t* masterGrid, char playerID, tuple_t* newPosition, tuple_t* oldPosition);
