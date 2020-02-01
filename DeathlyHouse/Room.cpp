/*******************************************************************************
*   Program: The Deathly House
*      File: Room.cpp
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
#include "common.h"
#include "Room.h"


/*****************************************************************************
* roomDefault()																 *
******************************************************************************
* sets each room encountered to a default value								 *
******************************************************************************
* Params:																	 *
*	roomData room[] - array of rooms in the game							 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void roomDefault(roomData room[])
{
	int roomNum;
	int counter;

	for(roomNum = 0; roomNum < maxRooms; roomNum++)		// loop through each room
	{
		room[roomNum].roomNumber = 0;					// make them invalid room # 0
		room[roomNum].roomName = "<EMPTY>";				// room name defailt
		room[roomNum].roomDescription = "<EMPTY>";		// room description defailt

		for(counter = 0; counter < 10; counter++)		// set all exits to none
		{
			room[roomNum].exits[counter] = 0;				// regular exits are mpme
			room[roomNum].blockedExits[counter] = 0;		// blocked exits are none
			room[roomNum].permBlocked[counter] = false;		// no permanently blocked directions
		}
		for(counter = 0; counter < 5;counter++)		// set required items to none
		{
			room[roomNum].requiredItems[counter] = 0;		// no required items
		}
		room[roomNum].hasVisited = false;		// obviously we haven't visited a room yet
	}
	return;
}


/*****************************************************************************
* viewExits()																 *
******************************************************************************
* finds and displays exits in a room										 *
******************************************************************************
* Params:																	 *
*	roomData room[] - array of rooms in the game							 *
*	int roomNum - the room number we are viewing details on.				 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void viewExits(roomData room)
{
	int index;			// index counter for room array
	string exitStr;		// a string to build a list of exits
	bool exits = false;	// room exits were found: none at this time

	exitStr = "Exits are ";			// start building that string
	for (index =0; index < 10;index++)	// loop through possible exits list)
	{
		//  any rooms listed for a given exit direction?
		if(room.exits[index]!=0 || room.blockedExits[index] !=0 || room.permBlocked[index])
		{
			exits = true;		// we got exits
			exitStr.append(compass[0][index]);	// add that direction to our list of exits
			exitStr.append(", ");
		}
	}
	if(!exits)
		exitStr.append("non existant.");	// no exits? say so.
	else
		exitStr.append("\b \b\b \b.");	// clean up that list of exits.
	display(exitStr);					// tell the player of the exits
	return;
}


/*****************************************************************************
* gotoRoom()																 *
******************************************************************************
* moves player to a new room, if possible.									 *
******************************************************************************
* Params:																	 *
*	roomData room[] - array of rooms in the game							 *
*	int direction - index number of the direction the user wishes to move	 *
* Returns:																	 *
*	int currentRoomNumber - the room the player has moved to				 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int gotoRoom(roomData room[],int direction)
{
	if(direction <0 || direction > 9)	//if an invalid direction, say so
		display("Huh? which way?");
	else if(room[currentRoomNumber].permBlocked[direction])		// if the direction can't ever be accessed, say so
		display("That way is inaccessable!");
	else if (room[currentRoomNumber].blockedExits[direction] != 0)	// if the direction is currently blocked, say so
		display("The way is blocked at the moment.");
	else if (room[currentRoomNumber].exits[direction] == 0)	// if no room exists for that direction, say so
		display("You can't go that direction!");
	else		// since we can go that direction
	{
		currentRoomNumber = room[currentRoomNumber].exits[direction];	// make current room the one for that exit
		room[currentRoomNumber].hasVisited= true;						// let the game know we have been in the room
	}
	return(currentRoomNumber);			// report back the current room number
}



/*****************************************************************************
* unblockRoom()																 *
******************************************************************************
* makes a blocked exit passable (ONE SIDED, return direction may be locked & *
* therefore blocked still)													 *
******************************************************************************
* Params:																	 *
*	roomData room[] - array of rooms in the game							 *
*	int direction - index number of the direction the user wishes to move	 *
*	itemData items[] - array of items in the room							 *
* Returns:																	 *
*	int currentRoomNumber - the room the player has moved to				 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int unblockRoom(roomData rooms[], itemData items[],int itemNum, int roomNum)
{
	int unlockedRoom = 0;		// set to default - no room unlocked
	int reqItem =0;				// item number of a required number
	int otherRoom =0;			// room to access to move to regular exit
	bool allItems = true;		// do we got all required items to unlock a room?
	bool alreadyDone = true;	// lets us know if the room unlock was successful
	int index;					// our index for a loop
	string unlockStr;			// our unlocked room status string

	if(items[itemNum].itemLocation != roomNum)	// item that is unlocked to unblock a room not in present room
		display("I can't unlock what isn't here");
	else	// Okay, so it is in the same room
	{
		for(index=0; index <5; index++)	// loop through the required item list
		{
			reqItem = items[itemNum].requiredItems[index];		// get the item number
			if (reqItem !=0  && items[reqItem].itemLocation != inventory)	// if item isn't item 0, AND not in inventory
				allItems = false;					// we don't have all the items we need.
		}
		if(allItems)		// if we got everything we need
		{
			for(index = 0 ;index < 10; index++)	// loop through all the directions
			{
				if(rooms[roomNum].blockedExits[index] !=0) //if the blocked exit points to a valid room
				{
					otherRoom =rooms[roomNum].blockedExits[index];		// get the unlocked destination room
					rooms[roomNum].blockedExits[index] = 0;				// set that position to zero
					rooms[roomNum].exits[index] = otherRoom;			// set a valid exit to that unlocked room
					alreadyDone = false;								// no blocked rooms before we started
					unlockedRoom = otherRoom;							// report back the unlocked room number
				}
			}
			if(!alreadyDone)	// if there was blocked rooms, thus not already unlocked (if it ever was locked)
			{
				unlockStr = "You successfully unlocked the ";		// tell the player we unlocked it
				unlockStr.append(items[itemNum].itemName);
				display(unlockStr);
			}
			else			// otherwise tell the player it isn't locked
				display("It isn't locked.");
		}
		else		// if we didn't have everything, tell the player so,
			display("I don't have what I need to unlock that");
	}
	return(unlockedRoom);		// report back the room we unlocked
}
