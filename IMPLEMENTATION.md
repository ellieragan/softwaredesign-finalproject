# CS50 Nuggets
## Implementation Spec
### Team 7 (Ellie Boyd, Jeffrey Liu, Brian Chun Yin Ng, Donia Tung), 21F

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes grid and player modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

Brian --> Server, receiving & sending messages

Donia --> Player data structure, handles participating players and spectator

Ellie --> Client

Jeff --> Grid, visibility

All team members:
major design and implementation decisions
reviewing PRs on github
testing for their own code
communication !

## Client

### Data structures

None

### Definition of function prototypes

*parseArgs* is a function to parse the command-line arguments, initialize the game struct, initialize the message module, and initialize analytics module:
```c
static int parseArgs(const int argc, char* argv[]);
```

*receiveMessages* is a function to receieve messages sent by the server:
```c
static void receiveMessages(int socket);
```

*sendMessages* is a function that sends messages to the server:
```c
static void sendMessages(int socket);
```

*outputMap* is a function that iterates through the map string sent by the server and prints it onto the screen for the player:
```c
static void outputMap(char* map);
```

### Detailed pseudo code

#### `main`:
    
    call parseArgs
    initialize address to given hostname and port
    send message to server to join game as player or spectator
    call recieveMessages to wait for response from server
    message_done

#### `parseArgs`:

	validate commandline
	initialize message module
	print assigned port number
	decide whether spectator or player

#### `receiveMessages`:
    calls message_loop with handleMessage
    
#### `sendMessages`:
    calls message_send
    
#### `handleMessage`:
    update screen as indicated by message from server
    
    
---

## Server

### Data structures

> For each new data structure, describe it briefly and provide a code block listing the `struct` definition(s).
> No need to provide `struct` for existing CS50 data structures like `hashtable`.

#### Player (detailed implementation under Player Module)
```c=
typedef struct player{
    char* realName; 
    char* ID; 
    grid_t* grid; 
    tuple_t* currentPos; 
    int gold; 
    bool spectator; 
    addr_t socket; 
} player_t; 
```
#### Grid (detailed implementation under Grid Module)
```c=
typedef struct grid{
    char* map;
    int rows;
    int cols;
    int gold;
    int goldLeft;
}
```
### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.
#### main
Parses command-line arguments with `parseArgs`, processes incoming messages, timeouts and inputs from stdin by calling `message_loop` from the message support module.

#### parseArgs
A function that parses command-line arguments.
```c
static int parseArgs(const int argc, char* argv[]);
```
### Helper functions
#### handleMsg
A function that is provided the address from which the message arrived, and a string containing the contents of the message. The handler should realize the string's memory will be reused upon return from the handler.
```c
bool handleMsg(void* arg, const addr_t from, const char* message));
```
#### handleTO
A function that is called when time passes without input or message.
```c
bool handleTO(void* arg);
```

#### handleStdin
A function that should read once from stdin and process it.
```c
bool handleStdin(void* arg);
```

### Detailed pseudo code

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

#### `main`

    Initializes array containing player objects
    Calls parseArgs
    Calls message_loop, passing the array of player objects as the argument

#### `parseArgs`

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()

#### `handleMsg`
    Grab player object corresponding to address
    Parse message and determine which function to call:
        Call methods in player data structure, sending messages back to client accordingly
        
#### `handleTO`
    Sends message to client telling it to terminate
    Terminates connection with client
    Deletes player object from array
    
#### `handleStdin`
    Doesn't do anything as Server doesn't take stdin commands
---

## Player module 

The player module will be responsible for holding key information about each player. We've constructed this data type as a mechanism for maintaining an easily accessible set of information about each player, rather than holding it all in maps within the server object.  
The player will contain, as instance variables: 
* String: Real name 
* String: Alphabet ID (local name)
* Grid: Grid of "viewed" locations
* Int tuple: Current location 
* Int: Gold collected 
* Boolean: representing whether or not the player is a spectator or participating client 
* Address type: socket # for the player

### Data structures
The module is itself defining a new data structure that will contain the requisite getters and setters. In addition to the basic data types identified above, the Player module will also hold a grid (which will be defined in the section below). 
```c=
typedef struct player{
    char* realName; 
    char* ID; 
    grid_t* grid; 
    tuple_t* currentPos; 
    int gold; 
    bool spectator; 
    addr_t socket; 
} player_t; 
```

We define a tuple struct by the following: 
```c=
typedef struct tuple{
    int x; 
    int y; 
} tuple_t; 
```

### Definition of function prototypes
For the purposes of cleanliness and simplicity, we won't list the getters and setters for the data module here. To find more information on them, reference `playerStruct.h`. Assume that getters and setters exist for each of the 5 instance variables contained within the player module. 

The other functions within the data structure are as follows: 
```c=
void initPlayer(char* realName, char* ID, grid_t* grid, tuple_t* currentPos, bool spectator); 
int addGold(player_t* player, int goldCollected, int*) remainingGold);
grid_t* movePlayer(player_t* player, char* keyPressed, player_t** otherPlayers); 
grid_t* updateSpectator(player_t* player, char* keyPressed, player_t* spectator)
void deletePlayer(player_t* player); 
```

### Detailed pseudo code
#### `initPlayer`
The `initPlayer` function is responsible for initializing a new player data structure. It's primary function is allocating space for the components of the player and validating the initial values for the player variables. 
```c=
validate inputs
if valid: 
    allocate memory for a new player struct 
    set instance variables based on input 
    return player object
else: 
    print to stderr requesting the missing/invalid input 
    return; 
```

#### `addGold`
The `addGold` function is responsible for updating a player's gold variable and decrementing the total amount of remaining gold in the game. 
```c=
get player's current gold count 
increment by goldCollected
subtract goldCollected from remainingGold 
return player's goldCollected 
```

#### `movePlayer`
The `movePlayer` function handles a player's movement, updating their internal grid of viewed/visited spots. First, we validate the new location. It calls `updateGrid`, defined in the grid module, which handles visibility. It returns the updated grid, which is passed back to the server, for communication to the client. 
```c=
parse keyPressed to determine new location of player
validate location would be "inside" the maze/not a wall 
if valid: 
    call setCurrentPos with new location
    call updateGrid, passing the player's grid, new location, and list of other players 
    set player's grid to output of updateGrid
    return output of updateGrid 
else: 
    print to err that the player tried to move off the grid/out of bounds
    return;  
```

#### `updateSpectator`
The `updateSpectator` method updates the grid of the spectator client. First, we validate the new location. It calls `updateSpectatorGrid` to update the grid of the spectator. Then we return the updated grid, which is passed back to the server, for communication to the client.
```c=
parse keyPressed to determine new location of player
validate location would be "inside" the maze/not a wall
if valid: 
    call updateSpectatorGrid, passing the spectator's grid, the local ID of the player that moved, and the new location of that player.
    set spectators's grid to output of updateSpectatorGrid
    return output of updateSpectatorGrid
else: 
    print to err that the player tried to move off the grid/out of bounds
    return;
```

#### `deletePlayer`
`deletePlayer` is responsible for clearing the memory associated with a given player. It will be called after a game is quit. 
```c=
delete player's grid 
delete player object
```

---

## Grid module 

The grid module handles and defines the grid data structure which includes the initialization of the map. The grid places gold in random locations across the map and additionally updates the quantity of gold in each point. 

### Data structures

The module defines a new data structure grid that establishes the basis of the map and stores its locations: 
```c=
typedef struct grid{
    char* filemap;
    int rows;
    int cols;
    int gold;
    int goldLeft;
}
```
Additionally, defines a visibility array that holds a value to represent whether a specific point is visible or not. 
### Definition of function prototypes
```c=
grid_t* grid_new(char* filemap, int seed);
char* getFileMap(grid_t* playerGrid);
int getRows(grid_t* playerGrid);
int getCols(grid_t* playerGrid);
int getNumPiles(grid_t* playerGrid);
int getGoldLeft(grid_t* playerGrid);
void buildPiles(int seed, grid_t* playerGrid);
int updateGoldCount(grid_t* playerGrid, int goldDecrease, tuple_t* location);
void updateVisibility(grid_t* playerGrid, tuple_t* location);
void delete(grid_t* playerGrid);
grid_t* updateSpectatorGrid(grid_t* spectatorGrid, char* playerID, tuple_t* newLocation); 
```
### Detailed pseudo code
#### `grid_new`
The `grid_new` method draws from the map.txt file from the server. It initializes the grid data structure for a specific player based upon inputs. Additionally, the method updates the grid to account for a random number of gold. 

``` c=
opens the map.txt file from server
store row as number of lines
store columns as line length
iterate through file
    store each character into char array
initialize grid structure with row, columns, array, gold constants
call buildPiles to display gold on grid
returns grid
```
#### `getFileMap`
The `getFileMap` method returns the char pointer to the filename. 

#### `getRows`
The `getRows` method returns the number of rows on a grid. 

#### `getCols`
The `getCols` method returns the number of columns on a grid. 

#### `getNumPiles`
The `getNumPiles` method returns the number of piles on a grid.

#### `getGoldLeft`
The `getGoldLeft` method returns a number that represents the amount of gold left in the grid.

#### `getTotalGold`
The `getTotalGold` method returns the total amount of gold.

#### `updateGoldCount`
The `updateGoldCount` method removes gold from the map when necessary and appropriately decreases the count. 
``` C=
change the grid into an one-dimensional array
change the pile to normal spot in the array
```

#### `updateVisibility`
The `updateVisibility` method updates the visibility of a grid in terms of a player's location. 
``` C=
iterate through all points of a grid
    calculate the slope of each point based on the location of a player
    calculate the intercept of each point based on the location of a player
    iterate through all rows between the provided grid's row and the player's row 
        compute the column value according to the linear equation
        if the column's highest and lowest points are not in a room
            set not visible at that specific grid point
    iterate through all columns between the provided grid's column and the player's column
     compute the row value according to the linear equation
        if the row's highest and lowest points are not in a room
            set not visible at that specific grid point   
set all others to visible
```
#### `updateSpectatorGrid`
```C=
iterates through all indexes in a visibility array
    sets all elements to visible
```

---

## Testing plan

### Unit testing

We intend to write unit tests for each individual module that ensure baseline functionality. These unit tests will test and check that each individual function within the module is working correctly. 
#### Client 
1. Test with invalid inputs from user 
2. Test receive message function with valid methods and invalid methods 
3. Test send message function with various messages that get sent to the server. 

#### Server
1. Test with invalid inputs (invalid map)
2. Test receive message function with valid and invalid messages
3. Test send message function with various messages 
4. Test termination

#### Player
1. Test initialization method 
2. Test getters and setters
3. Test movePlayer with mocked inputs 
4. Test updateSpectator with mocked inputs
5. Test addGold with mocked inputs 

#### Grid 
1. Test grid_new with maps of different sizes, increasing complexity  
3. Test getters and setters
4. Test updateVisibility with mocked inputs
5. Test updateSpectatorGrid with mocked inputs


### Integration testing
For integration testing, we intend to test interactions between the various modules. We will check
* Map initialization across players
* Gold display across players
* Gold updating across players
* Movement of players (and updates to their visibility)
* Players moving into the view of another player 
* Spectator view updates 
* Winners display 

### System testing
For system testing, we intend to manually test a spectator thread, end to end, and a player thread, end to end. Ensuring that the system is working will require manual confirmation of information updating for all players and the spectator. We will incrementally test the following game scenarios, increasing in complexity: 
1. Spectator
2. Single player  
3. 2 players
4. 3+ players

We will also the following test edge cases: 
* terminal size too small
* exceeding max number of players 
* multiple spectators (should bump the excess)

This is not an extensive description of the tests we intend to include and will be updated once we've implemented our code for this project. 

---

## Limitations
We intend to add to this section once we've implemented our nuggets game. 
> Bulleted list of any limitations of your implementation.
> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation.
