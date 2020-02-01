/*******************************************************************************
*   Program: The Deathly House
*      File: specialRoom.cpp
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
#include "common.h"
#include "TerminalIO.h"
#include "specialCmds.h"

/*****************************************************************************
* specialCmds()																 *
******************************************************************************
* handles special commands and conditions									 *
******************************************************************************
* Params:																	 *
*	string noun - the "items" section of a command							 *
*	string verb - the action section of a command							 *
*	roomData rooms[] - array of rooms in the game							 *
*	itemData items[] - array of items in the game							 *
* Returns:																	 *
*	int	status - the commands for the main loop to execute, or in the case	 *
*				of the right conditions, movement blocking					 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int specialCmds(string noun,string verb, roomData rooms[], itemData items[])
{
	int status = doNothing;		// our system command - default nothing done
	static int turns = 0;		// turns taken within the cab (room 2)
	static int attackMoves =0;	// turns taken under isPriority conditions

	//if the room is the room with the beast and its alive
	if(items[19].itemLocation == currentRoomNumber && !items[19].isDead)
	{
		status = cantMove;		// disable player movement
		if( attackMoves%3 == 2)	// how often our beast attacks - NOT RANDOM
		{
			display("The Beast attacks and hits You!");	// let the player know they were attacked
			health -= 25;								// take damage
			if(health <= 0)								// have we died?
			{
				// yup, we died, inform the player
				display("The beast lands a final devestating blow.");
				status = youDied();		// see what the player wants to do now
			}
		}
		else
			display("The beast attacks you and misses!");	// the beast missed
		attackMoves++;	// increment out attacks counter/timer
	}
	else
		attackMoves = 0;	// reset the counter, we are not under the right conditions

	// call a cab from room 3 or 1
	if(!verb.compare("call") && (!noun.compare("cab") || !noun.compare("taxi") || !noun.compare("taxicab") || !noun.compare("taxi cab") ) && (currentRoomNumber == 1 || currentRoomNumber ==3))
	{
		currentRoomNumber = 2;		// cab called from proper room. move player into room 2
		display("As you climb in the cab, the driver says: \"Where to?\"");
		status = handledElsewhere;	// let the system know the command was handled here
	}
	else if(!verb.compare("call"))
	{
		display("Call who or what?");
		status = handledElsewhere;	// let the system know the command was handled here
	}
	// special exit from hospital room
	else if((!verb.compare("leave") || !verb.compare("exit")) && (!noun.compare("hospital") ||!noun.compare("room") ||!noun.compare("hospital room")) && currentRoomNumber == 1)
	{
		currentRoomNumber = 2;		//cab called from the hospital room, move into the cab
		display("You hail a cab as you leave the hospital. As you climb in, the driver says: \"Where to?\"");
		status = handledElsewhere;	// let the system know the command was handled here
	}
	else if(!verb.compare("leave") || !verb.compare("exit"))
	{
		// can't leave or exit anything else but the hospital room
		display("I don't understand what you want to leave");
		status = handledElsewhere;	// let the system know the command was handled here
	}
	
	else if(currentRoomNumber ==2)	// if the player is in the taxi cab, these commands take effect
	{
		turns++;		// increment the amount of turns we can be in the cab before something bad happens
		if(!verb.compare("driver,") && !noun.compare("home"))	// tell the driver you want to go home (you'll never arrive though)
		{
			display ("The Driver says: \"Okay\"");	// driver acknowledges
			status = handledElsewhere;	// let the system know the command was handled here
		}
		else if(!verb.compare("driver,") && !noun.compare("hospital")) // tell the driver you want to go to the hospital
		{
			display ("The Driver says: \"Okay\"");	// driver acknowledges
			currentRoomNumber = 1;					// move into the hospital room
			turns = 0;								// reset our turns counter
			status = handledElsewhere;	// let the system know the command was handled here
		}
		// tell driver we want to go to joes house (or home)
		else if(!verb.compare("driver,") && (!noun.compare("joes house") || !noun.compare("joe's house") || !noun.compare("joes home") || !noun.compare("joe's home")))
		{
			display ("The Driver says: \"Okay\"");	// driver acknowledges
			currentRoomNumber = 3;					// move to just outside Joes place
			turns = 0;								// reset our turns counter
			status = handledElsewhere;	// let the system know the command was handled here
		}
		else if (!verb.compare("driver,"))	// the driver wasn't told any of the above actions
		{
			display("The Driver just looks as if you've spoken some ancient dialect instead of English");
			status = handledElsewhere;	// let the system know the command was handled here
		}
		else
			display("Time passes.");		// let the player know time is important here

		switch (turns)	// explain the seriousness of the time that has passed
		{
			case 1:	display("\nYou have feelings of doubt about going home at this moment.\n");		// first warning
					break;
			case 2:	display("\nYou have a bad feeling about going home.\n");						// second warning
					break;
			case 3:	display("\nYou now have a very bad feeling about going home\n");				// last warning
					break;
					// final results for not heeding the warnings
			case 4:	display("\nYikes! You just recieved a message from the hospital that your friend has died. In you own grief of not moving quick enough...");
		}
		if(turns > 3)	// have we hit the final results point?
			status = youDied();			// player has died if so
	}
	return(status);		// let the rest of the game know how it went
}
