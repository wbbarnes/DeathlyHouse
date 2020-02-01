/*******************************************************************************
*   Program: The Deathly House
*      File: Room.h
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
* This contains the data & functions specific to a room & it's navigation.
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;

#ifndef DEATHLY_HOUSE_ROOM__H
#define DEATHLY_HOUSE_ROOM__H

#include "TerminalIO.h"
//#include "common.h"
#include "Items.h"

struct roomData				// data structure for room data
{
	int roomNumber;			// Room ID number
	string roomName;		// Name of the Room
	string roomDescription;	// description of the room
	int exits[10];			// normal exits:  N,S,E,W,NE,NW,SE,SW,Up,Dn
	int blockedExits[10];	// blocked exits that can be unblocked
	bool permBlocked[10];	// permanently blocked exits
	int requiredItems[5];	// item numbers of items needed in inventory to access room, 5 MAX.
	bool hasVisited;		// have we been here before?
};

extern int currentRoomNumber;	// the current room number of the room the player is in.
extern struct itemData room[];			// the datastructure for item data

void roomDefault(roomData room[]);												// setup default room conditions
int gotoRoom(roomData room[],int direction);									// navigate to a room
int unblockRoom(roomData rooms[], itemData items[],int itemNum, int roomNum);	// unblock/unlock a room
void viewExits(roomData room);													// lists exits for a room

#endif
