/*******************************************************************************
*   Program: The Deathly House
*      File: specialRoom.h
*
*	 Author: William Barnes
*	  Class: 11MAR-CS106-WCO-V2D3 Fundamentals of Programming
*Instructor: Panos Mastrogiannis
*   Created: 13 May 2011
*  Modified:
*
********************************************************************************
* Abstract:
*	Text adventure game "The Deathy House," a Westwood CS106 Final Project, in
*	which programming elements learned in the class are demonstrated.
*
* This is the file that handles special rooms & conditions
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;			// MSVC++ preprocessor pragma to only use this file once

#ifndef DEATHLY_HOUSE_SPECIALROOM__H	// For more traditional C++ compilers.
#define DEATHLY_HOUSE_SPECIALROOM__H	//forces preprocessor to use this file only once

#include "Items.h"
#include "Room.h"

extern int health;

// execute commands specific to this game, and don't have to be in the main file
int specialCmds(string noun,string verb, roomData rooms[], itemData items[]);

extern int youDied(void);				// verify user wants to end the game.

#endif