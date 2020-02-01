/*******************************************************************************
*   Program: The Deathly House
*      File: common.h
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
* This file is for Common universal defs for all the project.
* There is no associated .c or .cpp file.
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;			// MSVC++ preprocessor pragma to only use this file once

#ifndef DEATHLY_HOUSE_COMMON__H	// For more traditional C++ compilers.
#define DEATHLY_HOUSE_COMMON__H	//forces preprocessor to use this file only once

// library includes
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>

using namespace std;

// global variables here

enum		// command sub-structure for game loop
{
	handledElsewhere = -1,		// command was handled by specialCmds()
	doNothing = 0,				// command processed, no special task to be done
	quit = 1,					// command processed, quit the game
	restart,					// command processed, restart the game
	loadGame,					// command processed, load the game
	saveGame,					// command processed, save the game
	cantMove					// special condition, disable movement
};

const int inventory = -1;		// special room number: player's invnentory

const string compass[2][10] = {{"north","south","east","west","northeast","northwest","southeast","southwest","up","down"},{"n","s","e","w","ne","nw","se","sw","u","d"}};


const int maxRooms = 35;		// maximum amount of rooms in the game, + 1.

const string version = "00.00.01";		// version number of software
const string copyright = "2011";		// (C) year

#endif


