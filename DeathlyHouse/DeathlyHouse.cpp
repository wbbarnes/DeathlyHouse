/*******************************************************************************
*   Program: The Deathly House
*      File: HouseHaunt.cpp
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
// project includes
#include "TerminalIO.h"
#include "Items.h"
#include "Room.h"
#include "FileIO.h"
#include "DeathlyHouse.h"
#include "specialCmds.h"
#include "common.h"


/*****************************************************************************
* main()																	 *
******************************************************************************
* Our main function, delegates & calls other functions to do our work.		 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int - exit status of program, 0 is a normal termination and exit.		 *
* Throws/Catches:															 *
*	None - Exception handling hasn't been covered at this time AND main		 *
*		should NEVER EVER throw an exception.								 *
*****************************************************************************/
int main()
{
	bool error;				// error flag
	int errorNum = 0;		// return error number
	logo();					// display game logo and wait for user to press a key
	error = startAnew();	// prepare the game as if we were starting fresh.
	if(!error)				// run the game if no error
		runGame();
	else					// else report our critical errors
	{
		cerr << "Critical Error(s) encountered! Game aborted!" << endl;
		errorNum = 1;
	}
	pause();
	return(errorNum);
}


/*****************************************************************************
* logo()																	 *
******************************************************************************
* Display game logo. Will clear the screen by scrolling (24 line screen) and *
* displaying the game logo.													 *
*																			 *
* NOTE: Uses global variable: version.										 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void logo(void)
{
	string vercpy;

	cout << endl << endl;		// display a vew blank lines
	cout << endl << endl;
	if (displayFile(gameTitleFile))		// display our game title file
		display("The Deathly House\n");	// if an error displaying the file, display a string instead
	vercpy = "  Version ";				// display version info
	vercpy.append(version);
	vercpy.append("\t\t\tWritten By & (C) ");
	vercpy.append(copyright);
	vercpy.append(", William Barnes.\n");
	display(vercpy,true);				// display version info
	return;
}


/*****************************************************************************
* startAnew()																 *
******************************************************************************
* initialize the game by getting all the external data						 *
* (other than save data)													 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	bool failure - the success of loading and initializing the data			 *
*				true if it failed to load properly, false if it loaded right *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
bool startAnew(void)		// initializes a new game - FILE-IO REQUIRED.
{
	bool roomFail = false;	// room load failure flag
	bool itmFail = false;	// item load failure flag
	bool failure;			// failure flage

	health = 100;

	//set up and initialize rooms
	roomDefault(rooms);							// set default values
	roomFail = fGetRoomData(mapFile, rooms);	// get room data

	//setup and initialize item data
	itemDefault(items);							// set default values
	itmFail = fGetItemData(objectFile,items);	// get item data

	currentRoomNumber = 1;		// set up our current room.
	rooms[currentRoomNumber].hasVisited = true;		// mark the room we start in as visited

	failure = roomFail | itmFail;	// OR our error flags into a single one.
	return(failure);
}


/*****************************************************************************
* runGame()																	 *
******************************************************************************
* Primary Game loop - displays details of area and reports game end status	 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int runtimeStat - the Game's error status while running the game.		 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int runGame(void)	// starts the game
{
	int runtimeStat =doNothing;	// game command status
	bool run = true;
	int room;

	pause();					// give the player a second
	display ("\n\n\n\n");		// add some blank lines
	room = -100;				// set this to a room we are guaranteed not in

	while(run)					// our game loop
	{
		if(room != currentRoomNumber)		// display the room info if coming in from another room
		{
			displayRoom(rooms[currentRoomNumber], items);	// show off room contents
			room = currentRoomNumber;						// set the room to the current room
		}
		display(" ");										// display a blank line
		runtimeStat = processCmdLine(rooms,items,currentRoomNumber);	// go get commands and process them

		if(runtimeStat == quit)		// end the game if we're told to
			run = false;
		else if(runtimeStat == restart)	//restart the game if we're told to
		{
			startAnew();
			room = 0;
		}
		else if(runtimeStat == loadGame)	//load the game if we're told to
		{
			startAnew();
			if(loadSaveGame(saveFile,rooms,items))	// report any errors during load
				display("\nAn ERROR occurred loading your save.");
			else
				room = 0;
		}
		else if(runtimeStat == saveGame)	//save the game
		{
			if(saveSaveGame(saveFile,rooms,items))	// report any errors during save
				display("\nAn ERROR occurred while saving your game.");
			else
				room = 0;
		}
	}
	return (runtimeStat);
}

/*****************************************************************************
* displayRoom()																 *
******************************************************************************
* Primary Game loop - displays details of area and reports game end status	 *
******************************************************************************
* Params:																	 *
*	roomData room    - room to display info on in the game					 *
*	itemData items[] - array of items in the game							 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void displayRoom(roomData room, itemData items[])
{
	display(" ");							// show blank line
	display(room.roomName);					// display room name
	display(room.roomDescription);			// display room description
	viewExits(room);						// show exits
	visibleItems(items,currentRoomNumber);	// show items present
	return;
}


/*****************************************************************************
* processCommandLine()														 *
******************************************************************************
* retrieves the user's command line and breaks it into two pieces. (noun and *
* verb), then converts it into lower case text for the parser to digest.	 *
******************************************************************************
* Params:																	 *
*	roomData rooms[] - array of rooms in the game							 *
*	itemData items[] - array of items in the game							 *
*	int roomNumber - the current room the player is in.						 *
* Returns:																	 *
*	int cmdCode - the Game's special function status while running the game. *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int processCmdLine(roomData rooms[],itemData items[],int roomNumber)
{
	unsigned int index;		// for-loop index counter
	string command;			// the cin (stdin) command line from the player
	int cmdCode;			// the comand code, or status of turn executions
	string verb;			// the action word from the user
	string noun;			// the target word(s) from the user, if present

	command = getInput("What now?");	// get user's command line
	command = stripLeading(command);	// strip non-alpha chars from beginning of string
	command = stripTrailing(command);	// strip non-alpha chars from the end of the string

	for(index =0;index < command.length();index++)	//get first word, verb
	{
		if(command.c_str()[index] ==' ')
		{
			verb = command.substr(0,index);		// retrieve the verb
			if((index+1) <command.length())
				noun = command.substr(index+1);	// any remaining text is considered the noun
			break;
		}
	}
	if(index == command.length())	// user typed in one word.
	{
		verb = command;				// that word is our verb
		noun = "";					// without a noun.
	}
	cmdCode = execCmd(verb,noun,rooms,items);	// process our commands
	return (cmdCode);
}


/*****************************************************************************
* execCmd()																	 *
******************************************************************************
* executes the given verb, with a given noun (if noun applicable) and		 *
* performs the desired task.												 *
******************************************************************************
* Params:																	 *
*	string verb - the action to do											 *
*	string noun - the object to act upon, if applicable						 *
*	roomData rooms[] - array of rooms in the game							 *
*	itemData items[] - array of items in the game							 *
* Returns:																	 *
*	int cmdCode - the Game's special function status while running the game. *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int execCmd(string verb,string noun, roomData room[], itemData item[])
{
	int cmdCode = doNothing;
	string compStr;


	// SPECIAL MODS THAT CANNOT GO IN specialROOM
	//		Hospital Bed aliases
	if(!noun.compare("bed") && currentRoomNumber == 1)
		noun = "hospital bed";
	//		House Key & door mat alias
	if(currentRoomNumber ==3)
	{
		if(!noun.compare("key"))
			noun = "house key";
		else if (!noun.compare("mat") || !noun.compare("doormat"))
			noun = "door mat";
	}
	//		Joe's house alias
	if(!noun.compare("joes house") || !noun.compare("house") && currentRoomNumber == 3)
		noun = "joe's house";


	// PROCESS special cases
	cmdCode= specialCmds(noun,verb,rooms,items);
	if(cmdCode == handledElsewhere)
		cmdCode = doNothing;
	// RESTART the game
	if(!verb.compare("restart"))
		cmdCode = questionRestart();

	// end the game
	//		VERBS: die, quit, q, "leave game"
	else if(!verb.compare("die") || !verb.compare("quit") || !verb.compare("q") || (!verb.compare("leave") && !noun.compare("game")))
		cmdCode = questionQuit();
	 //		PHRASES: commit suicide
	else if(!verb.compare("commit") && !noun.compare("suicide"))
		cmdCode = youDied();

	// WAIT a turn
	else if(!verb.compare("wait") || !verb.compare("z"))
		display("Time Passes.");

	// SAVE or LOAD
	else if(!verb.compare("save"))		// save game
	{
		display("Saving game...",false);
		cmdCode = saveGame;
		display("");
	}
	else if(!verb.compare("load"))	// load game
	{
		cmdCode = questionLoad();
		if(cmdCode == loadGame)
			display("Loading game...",false);
		display("");
	}

	// EAT something
	else if(!verb.compare("eat"))
	{
		if(!noun.compare("joe") && currentRoomNumber == 1)
		{
			display("EWWWW! Ya Canabal! Keep Away, FAR away! <ZZZZZZZAP!!!>...");
			cmdCode = youDied();
		}
		else
			display("Real Funny, I don't see anything edible.");
	}

	// MOVE an object
	else if(!verb.compare("move") && isDirection(noun)== -1)
		moveItem(items,findItem(noun,items),currentRoomNumber);

	// HIDE an item
	else if(!verb.compare("hide"))
		hide(noun);

	// PICK UP an item. VERBS: pickup, get, take, grab
	else if(!verb.compare("pickup") ||!verb.compare("get") || !verb.compare("take") || !verb.compare("grab") || !verb.compare("collect"))
		pickupItem(items,findItem(noun,items),currentRoomNumber);

	// DROP an item. VERBS: drop
	else if(!verb.compare("drop") || !verb.compare("leave"))
		dropItem(items,findItem(noun,items),currentRoomNumber);

	// EXAMINE an item or person. VERBS: look, examine
	else if (!verb.compare("look") || !verb.compare("examine"))
		look(noun,item);

	// HEAL an item. VERBS: cure, heal, fix
	else if (!verb.compare("cure") || !verb.compare("heal") || !verb.compare("fix"))
		cmdCode = cure(noun);

	// EQUIP and UNEQUIP an item
	else if(!verb.compare("equip") || !verb.compare("wear") || !verb.compare("wield"))
		equipItem(items,findItem(noun,items));
	else if(!verb.compare("unequip"))
		unequipItem(items,findItem(noun,items));

	// MOVE about
	//		VERBS: go, move, travel
	else if (!verb.compare("go") || !verb.compare("move")|| !verb.compare("travel"))
	{
		if(cmdCode == cantMove)		// if we can't move, say so
			display("Your efforts to escape the room are thwarted!");
		else
		{		// move the player
			// COMPASS direction
			if(isDirection(noun) != -1)	// if a valid direction, move that direction
			{
				currentRoomNumber = gotoRoom(rooms,isDirection(noun));
				// SPECIAL IF THAT CANT BE IN specialCmds
				if(currentRoomNumber ==2)
					display("You hail a cab as you exit the hospital. As you climb in, the driver says: \"Where to?\"");
			}
			// else "direction" must be via using an ITEM as a portal.
			else if(findItem(noun,items)>=0)
				usePortal(items,findItem(noun,items));
		}
	}

	//		VERBS are the compas directions themselves, long or short form
	else if(isDirection(verb) >= 0)
	{
		if(cmdCode == cantMove)		// if we can't move, say so
			display("Your efforts to escape the room are thwarted!");
		else
		{		// move the player
			currentRoomNumber = gotoRoom(rooms,isDirection(verb));
			if(currentRoomNumber ==2)
				display("You call a cab as you exit the hospital. As you climb in, the driver says: \"Where to?\"");
		}
	}

	// INVENTORY
	else if(!verb.compare("i") || !verb.compare("inv") || !verb.compare("inventory"))
		getInventory(items);

	// UNLOCK and LOCK
	else if(!verb.compare("unlock"))	// unlock
		unlock(noun);
	else if(!verb.compare("lock"))		// lock
	{
		if(noun.length() >0)		// didn't give anything to lock
			display("I cannot lock that.");
		else
			display("Huh? What am I supposed to lock?");
	}

	// CREATE an item
	else if(!verb.compare("create") || !verb.compare("make") || !verb.compare("build"))
		makeItem(items,findItem(noun,items),currentRoomNumber);

	// KILL
	else if(!verb.compare("kill") || !verb.compare("attack") || !verb.compare("fight"))
		cmdCode = attack(noun);

	// DEFAULT: didn't recognize the verb
	else
		display("I didn't understand that.");

	return (cmdCode);
}

/*****************************************************************************
* look()																	 *
******************************************************************************
* looks or examines an item or the current room								 *
******************************************************************************
* Params:																	 *
*	string noun - the object to act upon, if applicable						 *
*	itemData item[] - array of items in the game							 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void look(string noun, itemData item[])
{
	int index;			// the item number to look at

	//TAXI ROOM SPECIFIC: room: 2 (taxi) VERB: examine, look	NOUN: taxi, cab
	if (currentRoomNumber == 2 && (!noun.compare("taxi") || !noun.compare("cab")))
		display(rooms[currentRoomNumber].roomDescription);
	else if(!noun.compare("taxi") || !noun.compare("cab"))
		display("You are not in a taxi right now");
	// noun: <blank>, room 
	else if(noun.length() ==0 || !noun.compare("room"))
	{
		display("[Examine Room]\n");
		displayRoom(rooms[currentRoomNumber],items);
	}
	else	// we are looking at an item
	{
		index = findItem(noun,items);
		// if the item is in the list, AND is here or in inventory
		if(index !=0  && (item[index].itemLocation == currentRoomNumber || item[index].itemLocation == inventory ))
			display(item[index].itemDescription);	// describe the item
		else
			display("I Don't See that here");	// the item isn't here or doesn't exist
	}
	return;
}

/*****************************************************************************
* cure()																	 *
******************************************************************************
* cures a person or item													 *
******************************************************************************
* Params:																	 *
*	string noun - the object to act upon, if applicable						 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int cure(string noun)
{
	int status = doNothing;		// lets game loop know what to do

	// IF we want to cure joe AND are in the same room AND don't have the meds
	if(currentRoomNumber == 1 && !noun.compare("joe") && items[22].itemLocation != inventory)
		display("As much as you try, you're no doctor. Perhaps there is another way to help him.");

	// IF we want to cure joe AND are in the same room AND have the meds, cure him and end game
	else if (currentRoomNumber == 1 && !noun.compare("joe") && items[22].itemLocation == inventory)
		status = endGame();

	// if we want to cure joe (and isn't in the same room)
	else if (!noun.compare("joe"))
		display("Joe isn't here.");

	// IF we want to cure the player
	else if(!noun.compare("me"))
		display("You are already healthy as you can possibly be");

	// IF we want to cure the beast
	else if(currentRoomNumber == 17 && !noun.compare("beast"))
		display("As you have pity on the poor beast, you realize it MUST die!");

	// IF we want to cure an item (other than the beast)
	else if(items[findItem(noun,items)].itemLocation == currentRoomNumber)
		display("That doesn't need to be cured.");

	// we can't see the item we want to "cure"
	else
		display("I don't see that here.");
	return(status);
}


/*****************************************************************************
* isDirection()																 *
******************************************************************************
* Determines if a given string contains a direction							 *
******************************************************************************
* Params:																	 *
*	string str - the string containing the direction to move				 *
* Returns:																	 *
*	int compassDirection - 0-9 are valid directions, as defined by the		 *
*						struct Direction, and forms found in				 *
*						string compass[][]									 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int isDirection(string str)
{
	int index;	// internal loop index, for directions
	int loop;	// external loop index, for abbreviated or full direction name choice
	int compassDirection = -1;	// the direction we found, -1 is an invalid direction

	for (loop =0; loop < 2 && compassDirection < 0; loop++)	// outer loop: abbr or full name
	{
		for(index=0;index <10 && compassDirection < 0; index++)	// inner loop: direction name
		{
			if(!str.compare(compass[loop][index]))	// if we got a match
				compassDirection = index;			// take the inner index.
		}
	}
	return(compassDirection); //return the direction
}


/*****************************************************************************
* usePortal()																 *
******************************************************************************
* uses an item for a direction if the item is considered a portal			 *
******************************************************************************
* Params:																	 *
*	itemData item[] - the item to use as a portal							 *
*	int itemNum - the item number to try to use as portal					 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void usePortal(itemData items[],int itemNum)
{
	bool allReqItems = true;		// flag, do we have all required items?
	int index;						// index for getting required item number
	int altItem;					// item number for required item

	if(items[itemNum].mapPassage != 0)	// if we have a room to go to
	{
		for(index =0; index < 5;index++)	// find out if we have everything
		{
			altItem = items[itemNum].requiredItems[index];	//get item number to see if we have it
			if(altItem > 0 && items[altItem].itemLocation != inventory)	// if we don't
				allReqItems = false;									// we don't have allReqItems
		}
		if(allReqItems)		// if we have all required items
			currentRoomNumber = items[itemNum].mapPassage;	// goto the room stated in mapPassage
		else
			display("You can't pass through that right now.");	// else let player know we don't have everything
	}
	else	// if we don't have a room to go to, let player know
		display("You get a bloody nose walking into it.");
	return;
}


/*****************************************************************************
* youDied()																	 *
******************************************************************************
* Gives user options upon death or quitting									 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int - status of not quitting, quitting, or restarting					 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int youDied(void)
{
	int option = doNothing;	// the option chosen
	string cmd;				// the answer to our questions

	display("\nYou have Died.\n");

	do
	{
		cmd = getInput("Do you wish to <L>oad, <R>estart, or <Q>uit the game?");

		if(cmd.c_str()[0] == 'q')		// first char is a 'Q', so we quit
			option = quit;
		else if(cmd.c_str()[0] == 'r')	// first char is a 'R', so we restart
			option = restart;
		else if(cmd.c_str()[0] == 'l')	// first char is a "L" so we load a saved game
			option = loadGame;
	}
	while (option == 0);	// we didn't get a Q,R, or L, so repeat question.
	return(option);
}


/*****************************************************************************
* questionQuit()															 *
******************************************************************************
* makes sure the user wants to end the game									 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int - status of not quitting, quitting, or restarting					 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int questionQuit(void)
{
	int option = doNothing;		// the option chosen
	string cmd;					// the answer to our questions

	cmd = getInput("Do you Really want to quit the game? (y/N)");

	if(cmd.c_str()[0] == 'y')	// if first char is 'Y', yes we want to quit
		option = quit;
	return(option);
}


/*****************************************************************************
* questionRestart()															 *
******************************************************************************
* makes sure the user wants to end the game									 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int status  - restart or not to restart									 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int questionRestart(void)
{
	int option = doNothing;		// the option chosen
	string cmd;					// the answer to our questions

	cmd = getInput("Do you Really want to restart the game? (y/N)");

	if(cmd.c_str()[0] == 'y')	// if first char is 'Y', yes we want to restart
		option = restart;
	return(option);
}


/*****************************************************************************
* questionLoad()															 *
******************************************************************************
* makes sure the user wants to end the game									 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int status  - load a saved game  or not to to load						 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int questionLoad(void)
{
	int option = doNothing;	// the option chosen
	string cmd;				// the answer to our questions

	display("WARNING! Loading a saved game will lose any progress since the game was saved!");
	cmd = getInput("Do you Really want to load a saved game? (y/N)");

	if(cmd.c_str()[0] == 'y')	// if first char is 'Y', yes we want to load a saved game
		option = loadGame;
	return(option);
}


/*****************************************************************************
* hide()																	 *
******************************************************************************
* hides an item with another item											 *
******************************************************************************
* Params:																	 *
*	string noun - the hiding item and target								 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void hide(string noun)
{
	int toHideNum=0;		// the item number to hide
	int targetNum=0;		// the item number to hide with
	string toHide="";		// the name of the item to hide
	string target="";		// the name of the item to hide with

	if(noun.length() >0)	// make sure we got something to work with
	{
		// break noun down into parts
		toHide = leftStr(noun);			// get our item to hide
		target = rightStr(noun);		// get our item to hide with

		//adjust for shortened word phrase or alias.
		if(!toHide.compare("key") || toHide.compare("house"))
			toHide = "house key";
		if (!target.compare("mat") || !target.compare("doormat"))
			target = "door mat";

		// if the item to hide is equipped, we can't hide it.
		if(items[toHideNum].isEquipped)
			display("You need to unequip it before you can hide it.");

		// see if the item to hide is the same as the one to hide it with
		else if (target.compare(toHide))
		{											// if not, we hide it
			targetNum = findItem(target,items);		// get the item number of the hiding item
			toHideNum = findItem(toHide,items);		// get the item number of the item to hide

			if(targetNum > 0 && toHideNum > 0)								// make sure they both exist, then
				hideItem(items,toHideNum,targetNum, currentRoomNumber);		//	hide an item
			else if(targetNum == 0 && toHideNum >0)							// if only the item to hide
				display("I don't know where you want to hide that.");		//	let them know you don't know where to hide
			else if(targetNum > 0 && toHideNum == 0)						// if only the hiding item
				display("You want to hide what?");							//	let them know you don't know what to hide
			else
				display ("Huh? Hide what, and where?");						// if they both don't exist, say so
		}
		else																// if it's trying to hide itself with itself
			display ("Huh? I can't hide something with itself.");			//	say something about it.
	}
	else
		display("Huh? What am I supposed to hide, and Where?");				// nothing was given to hide or hide with 
	return;
}


/*****************************************************************************
* unlock()																	 *
******************************************************************************
* unlocks an item and opens the blocked path associated with the direction.  *
******************************************************************************
* Params:																	 *
*	string noun - the item to unlock										 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void unlock(string noun)
{
	int target;
	if(noun.length() >0)
	{
		target = findItem(noun,items);		// find target to unlock
		unblockRoom(rooms, items, target, currentRoomNumber);	// go unlock the room
	}
	else
		display("Huh? What am I supposed to unlock?");	// no target to unlock recognized
	return;
}


/*****************************************************************************
* attack()																	 *
******************************************************************************
* attack an item with another item											 *
******************************************************************************
* Params:																	 *
*	string noun - the weapon and target										 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int attack(string noun)
{
	int targetNum = 0;		// item number of target
	int weaponNum = 0;		// item number of weapon
	int status = doNothing;	// command status
	string target = "";		// target name for search
	string weapon = "";		// weapon name for search

	if(noun.length() >0)
	{
		// break noun down for multi parts
		target = leftStr(noun);		// get target name
		weapon = rightStr(noun);	// get weapon name

		// if our target is joe, and we are in the room with him, we be bad... kill us
		if(!target.compare("joe") && currentRoomNumber ==1)
		{
			display("You got some issues to work out Bud! <ZZZZZZZAP!!!>...");
			status = youDied();
		}
		// if our target is the player, kill him
		else if(!target.compare("me"))
		{
			display("You have been attacked...");
			status = youDied();
		}
		// else make sure target isnt the weapon
		else if (target.compare(weapon))
		{											// if it isnt
			targetNum = findItem(target,items);		// get target number
			weaponNum = findItem(weapon,items);		// get weapon number
			if(targetNum > 0 && weaponNum > 0)		// make sure we got both
				attackItem(items,targetNum, weaponNum, currentRoomNumber);		// attack an item with a specific item
			else if(targetNum == 0 && weaponNum >0)	// if we only got the weapon
				display("I don't know what you want to attack.");
			else if(targetNum > 0 && weaponNum ==0)	// if we only got the target
				display("I don't know what you want to attack with.");
			else									// if we got neither
				display("Huh? Attack what, with what?");
		}
		else										// inform player he can't attack his "weapon" with itself.
			display ("You can't attack that with itself?");
	}
	else	// we have no weapon and no target given
		display("Huh? What am I supposed to attack, and with what?");
	return(status);
}


/*****************************************************************************
* endGame()																	 *
******************************************************************************
* Game has been won, player can now quit, restart, or start again from a	 *
* save																		 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	int status  - restart or not to restart									 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int endGame()
{
	int option = doNothing;	// the option chosen
	string cmd;				// the answer to our questions

	display("\nYou give Joe the medicine you found with the beast and he makes a miraculous recovery.\n");
	display("Joe is happy that you came through for him, even if he finds your adventure incredible.");
	display(" You have won.\n");
	do
	{
		cmd = getInput("Do you wish to <L>oad, <R>estart, or <Q>uit the game?");

		if(cmd.c_str()[0] == 'q')		// if first char is 'Q',  we quit
			option = quit;
		else if(cmd.c_str()[0] == 'r')	// if first char is 'R', we restart
			option = restart;
		else if(cmd.c_str()[0] == 'l')	// if first char is 'L' we load a saved game
			option = loadGame;
	}
	while (option == 0);		// if we don't get any of those options, repeat the question until we do.
	return(option);
}
