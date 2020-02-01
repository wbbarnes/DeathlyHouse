/*******************************************************************************
*   Program: The Deathly House
*      File: HouseHaunt.h
*
*	 Author: William Barnes
*	  Class: 11MAR-CS106-WCO-V2D3 Fundamentals of Programming
*Instructor: Panos Mastrogiannis
*   Created: 30 March 2011
*  Modified:
*
********************************************************************************
* Abstract:
*	Text adventure game "The Deathy House," a Westwood CS106 Final Project, in
*	which programming elements learned in the class are demonstrated.
*
* This is the main file of the program. It contains the meat of the game
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;			// MSVC++ preprocessor pragma to only use this file once

#ifndef DEATHLY_HOUSE_DEATHLYHOUSE__H	// For more traditional C++ compilers.
#define DEATHLY_HOUSE_DEATHLYHOUSE__H	//forces preprocessor to use this file only once

roomData rooms[maxRooms];		// amount of rooms in the game +1.
itemData items[maxItems];		// amount of items in the game +1.

int currentRoomNumber;	// the current room number of the room the player is in.
int health;				// the health of the player

//prototypes
void logo(void);						// displays game logo.
int runGame(void);						// starts the game
bool startAnew(void);					// initializes a new game - FILE-IO REQUIRED.

	// retrieves command from input, breaks it down, then ships it to execCmd for processing
int processCmdLine(roomData rooms[], itemData items[], int roomNumber);
	// process the verb and noun, acts on them, and does the work of the game.
int execCmd(string verb,string noun, roomData room[], itemData item[]);

void displayRoom(roomData room, itemData items[]);	// display the data for a room, name, desc, exits, etc.
void look(string noun, itemData item[]);			// examine items or current room
int cure(string noun);								// cure an item or person
int isDirection(string str);						// returns a direction index if the string is a direction., -1 if not.
void usePortal(itemData items[],int itemNum);		// use an item as a portal to another room
int youDied(void);									// verify user wants to end the game.
int questionRestart(void);							// verify user wants to restart game.
int questionLoad(void);								// verify user wants to load a saved game.
int questionQuit(void);								// verify user wants to quit the game
void hide(string noun);								// hides an unequipped item with another item
void unlock(string noun);							// unlocks doors and paths in the game
int endGame(void);									// the game ending.
int attack(string noun);							// attack an object with an object


#endif
