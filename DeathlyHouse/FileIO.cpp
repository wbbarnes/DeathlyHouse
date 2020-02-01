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
* This file is for File IO functions
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#include "common.h"
#include "FileIO.h"


/*****************************************************************************
* fGetRoomCount()															 *
******************************************************************************
* Checks the game data file to get the number of rooms in the game			 *
*																			 *
******************************************************************************
* Params:																	 *
*	string - roomFile: the name of the map data file						 *
* Returns:																	 *
*	int - rooms: the number of rooms in the game, 0 if an error or none		 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int fGetRoomCount(string roomFile)
{
	int rooms = 0;				// room count
	string keyword = "";		// keyword from file
	bool runIt = true;			// flag for searching for the specific keyword: "rooms:"

	ifstream file(roomFile);	// open the file
	if (file)					// if the file opened
	{
		do		// start our search
		{
			if (file.peek()== '*')		// if a comment, throw it away.
				getline(file,keyword);
			else						// if not, process the line for "rooms:".
			{
				file >> keyword;				// retrieve keyword
				if(!keyword.compare("rooms:"))	// if this is the one we're looking for
				{
					file >> rooms;		// get how many rooms
					runIt = false;		// we got it, so we can now exit
				}
				else	// not the keyword we're interested in, ignore the rest of the line
					file.ignore(256,'\n');
			}
			if(file.fail())		// if we ran into a failure in reading
			{
				runIt = false;		// stop the search
				rooms = -2;			// report we have a failure in reading file
			}
		} while (runIt);	// test to repeat: is runIt still true?

		file.close();						// close the file, we're done
	}
	else
		rooms = -1;		// if we didn't find "rooms: report it
	if(rooms < -2)		// if we have less than one room (Well, less than our error numbers)
		rooms = 0;		// make sure it equals zero count.
	return(rooms);		// report back
}


/*****************************************************************************
* fGetRoomData()															 *
******************************************************************************
* Loads the room data into a roomData struct array							 *
*																			 *
******************************************************************************
* Params:																	 *
*	string roomFile	- the name of the room data file						 *
*	roomData room[]	- the room data structure array							 *
* Returns:																	 *
*	bool - true on read error, false on successful load						 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
bool fGetRoomData(string roomFile, roomData room[])
{
	int lines = 0;				// file line counter
	bool runIt = true;			// run sentinal flag for our loop
	bool failure = false;		// flag to note we had problems
	int roomCount;				// the amout of rooms in the game
	int loopCounter;			// used for reading data to store in arrays

	string keyword = "";		// keyword variable
	int value= 0;				// value of a specific keyword
	int workingRoom = 0;		// room we are working on, -1 if none

	char dumpChar;				// a wasted character from the input stream
	roomCount = fGetRoomCount(roomFile);		// retrieve room count

	if(roomCount > 0)		// as long as we got more than one room
	{
		ifstream file(roomFile);	// open room data file
		if (file)					// if it opened Okay...
		{
			do		// start processing the file
			{
				lines++;					// keep track of what line we are reading, as we report errors by line numbers
				if (file.peek()== '*')		// if a comment, throw it away.
					getline(file,keyword);
				else if (file.peek() == '\n')	// if not, a new line left over?
				{
 					file.ignore(256,'\n');		// throw it away
					keyword = "";
				}
				else						// if not either of those, process the line for keywords
				{
					file >> keyword;		// retrieve keyword
					if (file.eof())			// if end of file
					{
						runIt = false;		// stop searching file
					}
					//if not end of file, continue processing the found keyword.
					else if(!keyword.compare("rooms:"))			// Keep from triggering illegal keyword sequence
						file.ignore(256,'\n');					// on the keyword "rooms:"
					else if (!keyword.compare("room:"))			// if keyword is room
					{
						file >> workingRoom;			// retrieve the room number
						if(workingRoom <1 || workingRoom > roomCount)	// if we are out of range, create a fail state
						{
							runIt = false;		// room out of range, stop the run!
							failure = true;		// we failed in loading valid rooms
						}
						else				// set our room number if it was in range.
						{
							room[workingRoom].roomNumber = workingRoom;
						}
						file.ignore(256,'\n');	// ignore the rest of the line
					}
					else if(!keyword.compare("name:"))		// if keyword is name,
					{
						file.get(dumpChar);							// waste the space as we use getline() to get the rest of the line
						getline(file,room[workingRoom].roomName);	// store room name
					}
					else if(!keyword.compare("desc:"))		// if keyword is desc
					{
						file.get(dumpChar);							// waste the space as we use getline() to get the rest of the line
						getline(file,room[workingRoom].roomDescription);	//store room description
					}
					else if (!keyword.compare("exit:"))		// keyword is exit
					{
						for(loopCounter =0;loopCounter <10; loopCounter++)	// retrieve all 10 exits
						{
							file >> value;		// retrieve room to go to in that direction
							room[workingRoom].exits[loopCounter] = value;	// and set it up
						}
						file.ignore(256,'\n');		// ignore the rest of the line
					}
					else if (!keyword.compare("blocked:"))	// setup our unblockable exits, if any
					{
						for(loopCounter =0;loopCounter <10; loopCounter++)	// loop through all 10 blocked exits
						{
							file >> value;		// retrive the blocked room number
							room[workingRoom].blockedExits[loopCounter] = value;	// set it in the proper spot
						}
						file.ignore(256,'\n');	// ignore the rest of the line
					}
					else if (!keyword.compare("permBlocked:"))	// setup our permanently blocked exits, if any
					{
						for(loopCounter =0;loopCounter <10; loopCounter++)		// loop through the 10 of them
						{
							file >> value;		// retrive the room status
							if(value)
								room[workingRoom].permBlocked[loopCounter] = true;	// if it was != 0
							else
								room[workingRoom].permBlocked[loopCounter] = false;	// if it was = 0
						}
						file.ignore(256,'\n');	// ignore the rest of the line
					}
					else if (!keyword.compare("reqItem:"))		// setup our required item list.
					{
						for(loopCounter =0;loopCounter <5; loopCounter++)	// loop through all 5 slots
						{
							file >> value;		// retrive the item number
							room[workingRoom].requiredItems[loopCounter] = value;	// and save it
						}
						file.ignore(256,'\n');		// ignore the rest of the line
					}
					else								// we ran into a keyword that was not understood
					{
						cerr << "Illegal keyword \"" << keyword << "\" in " << roomFile << " at line " << lines << endl;
						file.ignore(256,'\n');		// ignore the rest of the line
					}
				}
				if(file.fail() && runIt == true)		// if we got an error and we haven't already told it to stop
				{										// like we did with an EOF on a keyword read. process a failure
					runIt = false;						// stop the loop
					failure = true;						// report error
				}
			} while (runIt);					// while our sentinal says to run the loop, continue to do so.
		file.close();							// close the file, we're done
		}
		else		// we couldn't open the file
		{
			cerr << "Cannot Open File: " << roomFile << endl;
			failure = true;
		}
	}
	else	// we had less than one room
		failure = true;
	if(failure)		// report line where error was found.
		cerr << "Error found on line " << lines << " of " << roomFile << endl << endl;
	return(failure);	// report failure status
}

/*****************************************************************************
* fGetItemCount()															 *
******************************************************************************
* Checks the game data file to get the number of rooms in the game			 *
*																			 *
******************************************************************************
* Params:																	 *
*	string - itemFile: the name of the item data file						 *
* Returns:																	 *
*	int - items: the number of items in the game, 0 if an error or none		 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int fGetItemCount(string itemFile)
{
	int items = 0;			// how many items to expect
	string keyword = "";	// keyword in item data
	bool runIt = true;		// process loop flag

	ifstream file(itemFile);	// open file
	if (file)					// if open
	{
		do						// do our search
		{
			if (file.peek()== '*')		// if a comment, throw it away.
				getline(file,keyword);
			else						// if not, process the line for "rooms:".
			{
				file >> keyword;		// retrieve a keyword
				if(!keyword.compare("items:"))		// is it "items:"?
				{
					file >> items;		// retrieve the item count if it is
					runIt = false;		// we got it, so we can now exit
				}
				else	// not the keyword we're interested in, ignore the rest of the line
					file.ignore(256,'\n');
			}
			if(file.fail())		// if we had a file error
			{
				runIt = false;	// stop the search
				items = -2;		// we have a failure in reading
			}
		} while (runIt);	// test to continue search

		file.close();		// close the file, we're done
	}
	else
		items = -1;		// we had a file open error
	if(items < -2)		// if we have less than one room (well less than our error condions)
		items = 0;		// make sure it equals zero count.
	return(items);		// return item count
}


/*****************************************************************************
* fGetItemData()															 *
******************************************************************************
* Loads the item data into an itemData struct array							 *
*																			 *
******************************************************************************
* Params:																	 *
*	string itemFile	- the name of the item data file						 *
*	itemData room[]	- the item data structure array							 *
* Returns:																	 *
*	bool - true on read error, false on successful load						 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
bool fGetItemData(string itemFile, itemData item[])
{
int lines = 0;					// file line counter
	bool runIt = true;			// run sentinal flag for our loop
	bool failure = false;		// flag to note we had problems
	int itemCount;				// the amout of rooms in the game
	int loopCounter;			// used for reading data to store in arrays
	char dumpChar;				// a wasted character from the input stream

	string keyword = "";		// keyword variable
	int value= 0;				// value of a specific keyword
	string boolStat;			// status for a specific boolean flag
	int itemNum = -1;		// room we are working on, -1 if none

	itemCount = fGetItemCount(itemFile);	// get item count

	if(itemCount > 0)		// if we have at least one item
	{
		ifstream file(itemFile);	// open file
		if (file)					// if it opened successfully
		{
			do		// do our search
			{
				lines++;			// keep track of the line number
				if (file.peek()== '*')		// if a comment, throw it away.
					getline(file,keyword);
				else if (file.peek() == '\n')	// a new line left over?
				{
 					file.ignore(256,'\n'); //ignore the rest of the line
					keyword = "";
				}
				else						// if not, process the line.
				{
					file >> keyword;		// get keyword
					if (file.eof())			// have we hit end of file?
						runIt = false;		// yes? quit processing
					else if(!keyword.compare("items:"))			// Keep from triggering illegal keyword sequence
						file.ignore(256,'\n');					// on the keyword "items:"
					else if (!keyword.compare("item:"))			// if keyword is item
					{
						file >> itemNum;			// get item number
						if(itemNum <1 || itemNum > itemCount)	// if we are out of range, create a fail state
						{
							runIt = false;		// if not a valid item number stop the process
							failure = true;		// we failed
						}
						else				// set our room number if it was in range.
						{
							item[itemNum].itemNumber = itemNum;
						}
						file.ignore(256,'\n');	// ignore the rest of the line
					}
					else if(!keyword.compare("name:"))			// item name keyword
					{
						file.get(dumpChar);							// waste the space as we read in the room thru getline
						getline(file,item[itemNum].itemName);
					}
					else if(!keyword.compare("desc:"))			// item description keyword
					{
						file.get(dumpChar);							// waste the space
						getline(file,item[itemNum].itemDescription);
					}
					else if (!keyword.compare("location:"))		// location keyword
					{
						file >> value;
						item[itemNum].itemLocation = value;
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("roomPass:"))		// map passage keywird
					{
						file >> value;
						item[itemNum].mapPassage = value;
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("reqItem:"))		// (reqItem) required Item keyword
					{
						for(loopCounter =0;loopCounter <5; loopCounter++)
						{
							file >> value;
							item[itemNum].requiredItems[loopCounter] = value;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("hides:"))		// hides Item keyword
					{
						for(loopCounter =0;loopCounter <5; loopCounter++)
						{
							file >> value;
							item[itemNum].hiddenItems[loopCounter] = value;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("health:"))		//  health keyword
					{
						file >> value;
						item[itemNum].health = value;
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("damage:"))		//  damage keyword
					{
						file >> value;
						item[itemNum].damage = value;
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("moves:"))		//  moves keyword
					{
						file >> value;
						item[itemNum].moves = value;
						file.ignore(256,'\n');
					}

					else if (!keyword.compare("isKey:"))	// setup our permanently blocked exits, if any
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
								item[itemNum].isKey = true;
						else if(!boolStat.compare("false"))
								item[itemNum].isKey = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isKey = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isPriority:"))	// isPriority keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isPriority = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isPriority = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isPriority = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isDeath:"))	// isDeath keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isDeath = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isDeath = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isDeath = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isDead:"))	// isDead keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isDead = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isDead = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isDead = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isAttackable:"))	// isAttackable keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isAttackable = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isAttackable = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isAttackable = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isCollectable:"))	// isCollectable keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isCollectable = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isCollectable = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isCollectable = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isMovable:"))	// isMovable keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isMoveable = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isMoveable = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isMoveable = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isWeapon:"))	// isWeapon keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isWeapon = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isWeapon = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isWeapon = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isEquipped:"))	// isEquipped keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isEquipped = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isEquipped = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isEquipped = false;
						}
						file.ignore(256,'\n');
					}
					else if (!keyword.compare("isDroppable:"))	// isDroppable keyword
					{
						file >> boolStat;
						if(!boolStat.compare("true"))
							item[itemNum].isDroppable = true;
						else if(!boolStat.compare("false"))
							item[itemNum].isDroppable = false;
						else //boolean phrase/keyword is not quite right.
						{
							cerr << "error in bool status on item: " << itemNum+1 << " boolean value of " << endl;
							item[itemNum].isDroppable = false;
						}
						file.ignore(256,'\n');
					}
					else								// we ran into a keyword that was not understood
					{
						cerr << "Illegal keyword \"" << keyword << "\" in " << itemFile << " at line " << lines << endl;
						file.ignore(256,'\n');
					}
				}
				if(file.fail() && runIt == true)		// if we got an error and we haven't already told it to stop
				{										// like we did with an EOF on a keyword read. process a failure
					runIt = false;						// stop the loop
					failure = true;					// report error
				}
			} while (runIt);					// while our sentinal says to run the loop, continue to do so.
		file.close();							// close the file, we're done
		}
		else		// we had a problem opening the file
		{
			cerr << "Cannot Open File: " << itemFile << endl;
			failure = true;
		}
	}
	else			// we had no rooms
		failure = true;
	if(failure)			// report line error occurred on
		cerr << "Error found on line " << lines << " of " << itemFile << endl << endl;
	return(failure);
}


/*****************************************************************************
* loadSaveGame()															 *
******************************************************************************
* Loads the save data to continue from where someone left off				 *
*																			 *
******************************************************************************
* Params:																	 *
*	string fname	- the name of the save file								 *
*	roomData room[]	- the room data structure array							 *
*	itemData item[]	- the item data structure array							 *
* Returns:																	 *
*	bool - true on loading save error, false on successful load				 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
bool loadSaveGame(string fname, roomData room[], itemData item[])
{
	bool badLoad = false;	// we default to a good load.
	string testStr;			// test string
	unsigned int index;		// item or room index counter
	unsigned int loop;		// room blocked exit loop counter


	ifstream input(fname);		// open file for input
	if(input)					// if open was a success
	{
		input >>  testStr;
		if(!testStr.compare("ACMODCION1T2E3L"))			// test and see if this is intact, or messed with
		{
			input >> currentRoomNumber;					// if not get our players last position

			for(index = 0; index < maxRooms && !input.fail(); index++)	//retrieve blocked exit list
			{
				input >> room[index].roomNumber;					// retrieve the room num
				for(loop = 0; loop < 10 && !input.fail() ; loop++)
				{
					input >> room[index].blockedExits[loop];		// get the blocked exit's room number
					input >> room[index].exits[loop];				// and also it's regular exit list (to what rooms)
				}
			}
		}
		else				// if the string wasn't intact, we have a bad load - corruption.
			badLoad = true;

		if(!input.fail() && !badLoad)		// if we didn't fail our first part, continue loading data
		{
			input >> testStr;
			if(!testStr.compare("PCCOXCTOA3T"))		// test and see if this is intact, or messed with
			{
				for(index = 0; index < maxItems && !input.fail(); index++)		// if not get item data
				{
					input >> item[index].itemNumber >> item[index].itemLocation;	// item number & location
					input >> item[index].isDead;									// if it is dead.
					input >> item[index].isEquipped;								// if it is equipped
				}
			}
			else					// if the string wasn't intact, we have a bad load - corruption
				badLoad = true;
		}
		else						// badLoad was already set, or we had a failure, either way we have a bad load
			badLoad = true;

		if(!input.fail() && !badLoad)	// if it still isnt a bad load or a failed read
		{
			input >> testStr;			// get our last test
			if (testStr.compare("MCAOCCION1T2O3SH"))	// if the string isn't intact, 
				badLoad = true;							// the file is corrupted
		}
		else						// badLoad was already set, or we had a failure, either way we have a bad load
			badLoad = true;
		input.close();
	}
	else	// we failed to open the file, so the load is definitely bad
		badLoad = true;

	if(input.fail())	// if we had an error reading the file, anywhere, we had a bad load.
		badLoad = true;

	return(badLoad);
}


/*****************************************************************************
* saveSaveGame()															 *
******************************************************************************
* saves the save data to continue from where someone left off				 *
*																			 *
******************************************************************************
* Params:																	 *
*	string fname	- the name of the save file								 *
*	roomData room[]	- the room data structure array							 *
*	itemData item[]	- the item data structure array							 *
* Returns:																	 *
*	bool - true on writing save error, false on successful save				 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
bool saveSaveGame(string fname, roomData room[], itemData item[])
{
	bool badSave = false;	// we default to a good save
	unsigned int index;		// item or room index counter
	unsigned int loop;		// room blocked exit loop counter

	ofstream output(fname);		// open file for output/writing
	if(output)					// if successful open
	{
		output << "ACMODCION1T2E3L" << " "<< currentRoomNumber << " ";	// set test string and current room number

		for(index = 0; index < maxRooms && !output.fail(); index++)
		{
			output << room[index].roomNumber << " ";					// kick out a room
			for(loop = 0; loop < 10 && !output.fail() ; loop++)
			{
				output << room[index].blockedExits[loop] << " ";		// it's blocked exit list (to what rooms)
				output << room[index].exits[loop] << " ";				// and also it's regular exit list (to what rooms)
			}

		}
		if(!output.fail())				// if that didn't fail, write out our second test string
			output << "PCCOXCTOA3T" << " ";
		
		for(index = 0; index < maxItems && !output.fail(); index++)		// now we kick out our item data
		{
			output << item[index].itemNumber << " " << item[index].itemLocation;	// item number & location
			output << " " << item[index].isDead << " ";								// if it is already dead
			output << item[index].isEquipped << " ";								// if it is equipped
		}
		if(!output.fail())				// if that didn't fail, write out our last test string
			output << "MCAOCCION1T2O3SH" << endl;
		else						// if it did. we failed
			badSave = true;
		output.close();				// close the file
	}
	else		// since we couldn't open the file to write to it, we failed to have a good save
		badSave = true;

	if(output.fail())		// if we had a write failure, make sure we note it was a bad save.
		badSave = true;

	return(badSave);
}
