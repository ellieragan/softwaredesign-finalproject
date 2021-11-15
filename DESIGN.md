# CS50 Nuggets Final Project
## Design Spec
### Team 7 (Ellie Boyd, Jeffrey Liu, Brian Chun Yin Ng, Donia Tung), Fall, 2021

 According to the [Requirements Spec](https://github.com/CS50Dartmouth21FS1/nuggets-info/blob/main/REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server. Our design also includes grid and player modules. We describe each program and module separately. We do not describe the support library nor the modules that enable features that go beyond the spec. We avoid repeating information that is provided in the requirements spec.

Here we focus on the core subset for both the client and the server:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## Client 
The client acts in one of two modes:

1. spectator, the passive spectator mode described in the requirements spec.
2. player, the interactive game-playing mode described in the requirements spec.

### User Interface 
See the requirements spec for both the command-line and interactive UI.

The client takes three command line arguments; first argument is the hostname or IP address where the server is running, the second argument is the port number on which the server expects messages, and the third (optional) argument determines whether to join as a player or spectator.

```bash=
./client hostname port [playername]
```
* [playername] is optional
* However, if no name is provided, the player is assumed to be a spectator. 

If the playername argument is provided, the client joins as a player and can interactively play the game. Otherwise the client joins as a view-only spectator.

### Inputs and outputs 
#### Input:
There are 2 types of inputs for client: 

1. Inputs from command-line (specified above) 
2. Keystroke inputs after initialization 
* q quit the game.
* h move left, if possible
* l move right, if possible
* j move down, if possible
* k move up , if possible
* y move diagonally up and left, if possible
* u move diagonally up and right, if possible
* b move diagonally down and left, if possible
* n move diagonally down and right, if possible

Where possible means the adjacent gridpoint in the given direction is an empty spot, a pile of gold, or another player.

For each move key, the corresponding Capitalized character will move automatically and repeatedly in that direction, until it is no longer possible.

####  Output: 
Client's current map, the location of the players, the score, and the status header. 
The client is responsible for the UI for gameplay and it's "output" will be the game interface. 

### Functional decomposition into modules
#### Client Module
We anticipate the following functions and modules:
* *main* to drive the client module 
* *parseArgs* to determine validity of arguments
* *handleMessage* to receive messages from sever
* *handleInput* to send messages to server


### Pseudo code for logic/algorithmic flow
```
parse arguments from user, validate arguments
start game with message to server asking to enter game as player or spectator
start message loop to wait for responses from server 

listen for keyboard input from user: 
    send movement and quit messages to server
listen for responses from server
    update game screen accordingly 
```

### Major data structures
The client provides the user interface for the playing particpant. 
It contains no local inherent data structures.  

## Server 

### User Interface 
See the requirements spec for the command-line interface. There is no interaction with the user.

In order to initialize a server, you will need to provide it a map 
```bash=
./server mapPath
```

### Inputs and outputs 
Inputs: As per the requirements spec, other than the inputted map on which the game instance will be run, the server receives no other inputs directly from the user. All server inputs will be received as messages from the client.

Outputs: As per the requirements spec, the server has no output directly visible by the player. Rather, the server outputs are messages to the client. 
 
### Functional decomposition into modules
We anticipate the following functions or modules in our server: 
* *main* will drive the module 
* *parseArgs* which will parse and validate the inputted file
* *initGrid* which will initialize the grid and set gold randomly throughout the maze
* *receiveMessages* which will be resonsible for handling the messages from the client & acting accordingly
* *sendMessages* which will send messages to the client

We will rely on two helper modules, *grid* and *player*, which define distict data structures that hold the grid structure or the player structure. The grid will maintain a 2D array of the loaded grid. The player structure will maintain its real name, local name, gold collected, current location, and a grid object (containing the "visited" or visible spaces on the grid that the player has seen). 


### Pseudo code for logic/algorithmic flow

The server will run as follows:
```
Parse and validate input from command line
Load map onto display
Randomly insert gold onto map in random amounts
If the server gets a new client message:
	make either new spectator or player
While there is gold on the map:
	update clients about player locations and game status as necessary
Exit game, display scores
```

## Other data structures
### Grid
Maintains an array of the loaded grid. Contains the following:
1. grid string
2. num rows
3. num cols
4. gold remaining
5. number of gold piles


### Player
Contains the following data about players:
1. real name
2. local name
3. gold collected
4. current location
5. visibility object
6. socket address

There will be getters and setters for each of these fields. Visibility will be implemented as a component of the player data structure, as it is necessary to update the grid object upon a player's movement. 

See the implementation spec for more details about each modules.

## Division of labor 
Brian --> Server
    
Donia --> Player

Ellie --> Client

Jeff --> Grid
    
#### All team members: 
* major design and implementation decisions 
* reviewing PRs on github 
* testing for their own code 
* communication ! 
