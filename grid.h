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



