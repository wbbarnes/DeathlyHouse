/*******************************************************************************
*   Program: The Deathly House
*      File: FileIO.h
*
*	 Author: William Barnes
*	  Class: 11MAR-CS106-WCO-V2D3 Fundamentals of Programming
*Instructor: Panos Mastrogiannis
*   Created: 31 March 2011
*  Modified:
*
********************************************************************************
* Abstract:
*	Text adventure game "The Deathy House," a Westwood CS106 Final Project, in
*	which programming elements learned in the class are demonstrated.
*
* This file is for File IO functions
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;			// MSVC++ preprocessor pragma to only use this file once

#ifndef DEATHLY_HOUSE_FILEIO__H	// For more traditional C++ compilers.
#define DEATHLY_HOUSE_FILEIO__H	//forces preprocessor to use this file only once
// project includes
#include "Items.h"
#include "Room.h"
#include "common.h"

// Data file names used with the game
const string saveFile = "Deathly.sav";			// game save filename
const string mapFile ="Deathly.dat";			// room map filename
const string objectFile = "Deathly.itm";		// item data filename
const string gameTitleFile = "GameLogo.tx";		// game's title screen filename

extern int currentRoomNumber;	// the current room number of the room the player is in.



//prototypes
int fGetRoomCount(string fname);					// retrieve The number of rooms in the game.
bool fGetRoomData(string fname,roomData room[]);	// load the rooms into an array.
int fGetItemCount(string fname);					// retrieve the number of items in the game.
bool fGetItemData(string fname, itemData item[]);	// load the items into an array
bool loadSaveGame(string fname, roomData room[], itemData item[]);	// loads a game state. Returns false on success, true on fail
bool saveSaveGame(string fname, roomData room[], itemData item[]);	// saves a game state. Returns false on success, true on fail

#endif							//end of preprocessor directive for "single use"
