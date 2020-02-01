/*******************************************************************************
*   Program: The Deathly House
*      File: TerminalIO.h
*
*	 Author: William Barnes
*	  Class: 11MAR-CS106-WCO-V2D3 Fundamentals of Programming
*Instructor: Panos Mastrogiannis
*   Created: 17 April 2011
*  Modified:
*
********************************************************************************
* Abstract:
*	Text adventure game "The Deathy House," a Westwood CS106 Final Project, in
*	which programming elements learned in the class are demonstrated.
*
* This file is for Console/Dumb Terminal/CLI IO functions & special string
* handling functions
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;			// MSVC++ preprocessor pragma to only use this file once

#ifndef DEATHLY_HOUSE_TERMINALIO__H	// For more traditional C++ compilers.
#define DEATHLY_HOUSE_TERMINALIO__H	//forces preprocessor to use this file only once

#include <conio.h>		// Needed for some Windows/DOS specific functions
#include "common.h"

const unsigned int screenLen = 24;			// default screen length
const unsigned int screenWidth = 80;		// default screen width
const int tabStop = 4;			// tab stops are every "tabStop" characters
extern int currentRoomNumber;	// the current room number of the room the player is in.

void pause(void);			// pauses the screen - uses one console function.
void display(string str, bool newLine);		// displays a string to the screen, with proper word-wrap.
void display(char *str, bool newLine);		// wrapper for display to use char array for output.	
void display(string str);					// wrapper for display to include an endline automatically
void display(char *str);					// wrapper for display to use char array and include an enline automatically
bool displayFile(string fname);				// opens a file and displays it to the screen
string getInput(string prompt);				// reads in a command line string and returns it via a string stream.

string lowerString(string str);				// converts a string to all lower case
string stripLeading(string str);			// strip leading whitespace & numbers off a string
string stripTrailing(string str);			// strip trailing whitespace & numbers off a string
string leftStr(string str);							// gets the left-most word of str and returns it
string rightStr(string str);						// gets the right-most word of str and returns it

#endif

