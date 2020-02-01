/*******************************************************************************
*   Program: The Deathly House
*      File: Items.h
*
*	 Author: William Barnes
*	  Class: 11MAR-CS106-WCO-V2D3 Fundamentals of Programming
*Instructor: Panos Mastrogiannis
*   Created: 23 April 2011
*  Modified:
*
********************************************************************************
* Abstract:
*	Text adventure game "The Deathy House," a Westwood CS106 Final Project, in
*	which programming elements learned in the class are demonstrated.
*
* This contains the data & functions specific to an itwm & it's navigation.
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#include "common.h"
#include "Items.h"


/*****************************************************************************
* itemDefault()																 *
******************************************************************************
* sets each item encountered to a default value								 *
******************************************************************************
* Params:																	 *
*	itemData items[] - array of rooms in the game							 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void itemDefault(itemData item[])
{
	int itemNum;		// item number index
	int counter;		// item internal array index

	for(itemNum = 0; itemNum < maxItems; itemNum++)		// loop through all items
	{
		item[itemNum].itemNumber = 0;					// set item number to 0
		item[itemNum].itemName = "<EMPTY>";				// default item name
		item[itemNum].itemDescription = "<EMPTY>";		// default item description
		item[itemNum].itemLocation = 0;					// item location, room 0
		item[itemNum].mapPassage = 0;					// room 0 (not portal, portal if room was a valid room #)
		for (counter =0 ; counter < 5; counter++)		// go through internally indexed data arrays
		{
			item[itemNum].requiredItems[counter] = 0;	// no required items
			item[itemNum].hiddenItems[counter] = 0;		// no items hid by this one
		}
		item[itemNum].health = 0;						// no health
		item[itemNum].damage = 0;						// no damage
		item[itemNum].moves = 0;						// no moves to defeat in
		item[itemNum].isPriority = false;				// not a priority
		item[itemNum].isDeath = false;					// is not a threat
		item[itemNum].isDead = true;					// isn't alive
		item[itemNum].isAttackable = false;				// can't attack it
		item[itemNum].isCollectable = false;			// can't pick it up
		item[itemNum].isMoveable = false;				// can't move it
		item[itemNum].isWeapon = false;					// not a weapon
		item[itemNum].isEquipped = false;				// not equipped
		item[itemNum].isDroppable = false;				// can't drop it
		item[itemNum].isKey = false;					// not a key (just a port key)
	}
 	return;
}



/*****************************************************************************
* visibleItems()															 *
******************************************************************************
* displays all items in a room that are not hidden  in room 0				 *
******************************************************************************
* Params:																	 *
*	itemData item[] - array of items in the game							 *
*	int room - the room number we are finding visible items in.				 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void visibleItems(itemData item[], int room)
{
	int index;				// item counter index
	bool noItems = true;	// no items found flag.
	string visible;			// visible items string

	visible = "You see ";
	for(index = 0; index < maxItems;index++)	// loop through items
	{
		if(item[index].itemLocation == room)	// if it's in the same room
		{
			noItems = false;					// no items is false, as there are items
			visible.append(item[index].itemName);		// place item name in our string
			visible.append(", ");
		}
	}
	if(!noItems)		// if we have items, clean up the string a bit & display it.
	{
		visible.append("\b \b\b \b.");
		display(visible);
	}
	return;
}


/*****************************************************************************
* makeItem()																 *
******************************************************************************
* "creates" a new item (unhides it) if the preReq items are in inventory	 *
*	(inventory is considered room -1.) and places it in the given room		 *
******************************************************************************
* Params:																	 *
*	itemData item[] - array of items in the game							 *
*	int itemNumber - the specific item we wish to make						 *
*	int room - the room number we are in.									 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void makeItem(itemData item[],int itemNumber, int room)
{
	int counter;			// loop index counter
	int reqItem;			// item number of a required item
	bool canMake = true;	// flag to let us know if we can make the item or not
	string makeStr;			// status string

	if(item[itemNumber].itemLocation!=0)	// if the made item exists in inventory or a room
		display("you already made that.");	//	let the player know
	else
	{
		for(counter = 0; counter < 5; counter++)	// see if we got everything
		{
			if(item[itemNumber].requiredItems[counter] != 0)
			{
				reqItem = item[itemNumber].requiredItems[counter];
				if(item[reqItem].itemLocation != inventory )	// if we don't
					canMake = false;							//	we can't make
			}
		}
		if (canMake && counter > 0)
		{
			for(counter = 0; counter < 5 ; counter++)	// remove items from inventory
			{
				if(item[itemNumber].requiredItems[counter] != 0)
				{
					reqItem = item[itemNumber].requiredItems[counter];
					if(item[reqItem].itemLocation == inventory )	// if in inventory
						item[reqItem].itemLocation = 0;				//	remove it
				}
			}
			item[itemNumber].itemLocation = room;					// pace the new item in the room
			makeStr = "You made a ";								// inform player
			makeStr.append(item[itemNumber].itemName);
		}
		else			//inform the player we can't make it right now (we don't have everything)
			 makeStr = "As hard as you try, you cannot make that right now.";
		display(makeStr);
	}
	return;
}



/*****************************************************************************
* moveItem()																 *
******************************************************************************
* moves an item to unhide every item hidden	by it							 *
******************************************************************************
* Params:																	 *
*	itemData item[] - array of items in the game							 *
*	int itemNumber - the item to move										 *
*	int room - the room number we are in.									 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void moveItem(itemData item[],int itemNumber, int room)
{
	string moveStr;
	int index;
	int foundItem;
	bool unhid = false;

	//first two ifs are specials to this game
	if(!item[itemNumber].itemName.compare("Joe") && room == 1)		/// if Joe and in room 1, bad idea to move joe
		display("Moving Joe is a bad idea.");
	else if (!item[itemNumber].itemName.compare("hospital bed") && room == 1)		// if room 1 and hospital bed, bad idea to move that too
		display("Moving Joe's hospital bed is a bad idea.");

	else if(!item[itemNumber].isDead)									// can't move it if it isn't dead
		display("As you try to move it, it moves away from you!");
	else if(item[itemNumber].itemLocation == inventory)					// can't move what's in inventory
		display("You can't move what you're carrying!");
	else if(item[itemNumber].itemLocation != room)						// can't move what's not here
		display("You can't move what's not here");
	else if (item[itemNumber].isMoveable)					// if it IS moveable
	{
		moveStr = "You move the ";
		moveStr.append(item[itemNumber].itemName);		// move it, identify moving it
		moveStr.append(" and discover: ");				// and announce discovery of hidden items
		for(index = 0;index <5;index++)					// we search for here
		{
			if(item[itemNumber].hiddenItems[index] > 0)	//did we find an item?
			{
				unhid = true;							// we found an item
				foundItem = item[itemNumber].hiddenItems[index];	// get the item number
				item[foundItem].itemLocation = room;				// place found item in the room
				moveStr.append(item[foundItem].itemName);
				moveStr.append(", ");
				item[itemNumber].hiddenItems[index] = 0;
			}
		}
		if(unhid)		// if we found at least one item, clean up the string
			moveStr.append("\b \b\b \b.");
		else			// esle announce we found nothing
			moveStr.append("Nothing");
		display(moveStr);
	}
	else // we can't move it, so inform player
		display("No matter how hard you try, you can't make it budge an inch.");
	return;
}


/*****************************************************************************
* dropItem()																 *
******************************************************************************
* drops an item in the current room, if isDroppable							 *
******************************************************************************
* Params:																	 *
*	itemData item[] - the item array										 *
*	int itemNum - the item number of the item to drop						 *
*	int room - the room number we in.										 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void dropItem(itemData item[],int itemNum, int room)
{
	string dropStr;		// string for dropping item status

	if (item[itemNum].itemLocation != inventory)	// tell player if not in inventory
		display("You are not carrying that!");
	else if(item[itemNum].isEquipped)				// tell player if it is still equipped
		display("You need to unequip it first");
	else if (item[itemNum].isDroppable)				// drop it if it is droppable
	{
		item[itemNum].itemLocation = room;			// put it in the room
		dropStr = "You dropped the ";
		dropStr.append(item[itemNum].itemName);		// tell user it is dropped
		dropStr.append(".");
		display(dropStr);
	}
	else		// if not droppable, tell user
		display("Try as you might, you can't seem to drop it.");
	return;
}


/*****************************************************************************
* pickupItem()																 *
******************************************************************************
* picks up an item and places it in inventory if it is in the same room and  *
* isCollectable.															 *
******************************************************************************
* Params:																	 *
*	itemData item[] - the item array.										 *
*	int itemNum - the item number of the item to pick up					 *
*	int room - the room number we are in.									 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void pickupItem(itemData item[],int itemNum, int room)
{
	string puStr;	// string to build out pickup line on

	if(item[itemNum].itemLocation == inventory)	// if it is already in our inventory, say so
		display("You already have that.");
	else if(item[itemNum].itemLocation != room)	// if it isn't in the room, say so
		display("That Item is not here!");
	else if(!item[itemNum].itemName.compare("Joe"))	// if it is Joe, tell user, he can't take joe
		display("It's best to leave Joe here.");
	else if (item[itemNum].isCollectable)		// if we can pick it up
	{
		item[itemNum].itemLocation = inventory ;	// place it in inventory
		puStr = item[itemNum].itemName;				// tell player we did pick it up
		puStr.append(" is now in your posession.");
		display(puStr);
	}
	else			// if we can't pick it up
		display("You can't pick that up.");
	return;
}


/*****************************************************************************
* findItem()																 *
******************************************************************************
* Finds out if item is present in the items array							 *
******************************************************************************
* Params:																	 *
*	string name - the name of the item to find.								 *
*	itemData item[] - the item array										 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
int findItem(string name, itemData items[])
{
	unsigned int index;		// index counter
	string itemStr;			// name of item to compare to
	int found = 0;			// item # of found item, 0 if none found

	for(index = 0; index < maxItems; index++)		// check hidden list
	{
		itemStr = lowerString(items[index].itemName);	// in lower case
		if(!itemStr.compare(name))		// if item names match
		{
			found = items[index].itemNumber;	// we found it, so get item number
			break;								// don't bother searching anymore
		}
	}
	return(found);		// tell us what item was identified in itemData array
}

/*****************************************************************************
* equipItem()																 *
******************************************************************************
* Equips an item, if isEquipable is true									 *
******************************************************************************
* Params:																	 *
*	itemData item[] - the item array										 *
*	int itemNum - the number of the item to equip							 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void  equipItem(itemData item[],int itemNum)
{
	if(item[itemNum].isEquipped)		// if item is already equipped, tell player
		display("You have already Equipped that.");
	else if (item[itemNum].itemLocation != inventory)	// if it isn't in inventory, tell player
		display("You Don't have that!");
	else		//else equip it and tell player
	{
		item[itemNum].isEquipped = true;
		display("You successfully equip the item.");
	}
	return;
}


/*****************************************************************************
* unequipItem()																 *
******************************************************************************
* unequips an equiped item													 *
******************************************************************************
* Params:																	 *
*	itemData item[] - the item array										 *
*	int itemNum - the item number of the item to unequip					 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void  unequipItem(itemData item[], int itemNum)
{
	if(item[itemNum].itemLocation != inventory)	// if item isn't in inventory, tell player
		display("You don't even have that!");
	else if (item[itemNum].isEquipped)			// if it is equipped
	{
		display("You unequip it");			// tell player
		item[itemNum].isEquipped = false;	// unequip it
	}
	else			// tell player it isn't equipped
		display("It isn't equipped!");
	return;
}

/*****************************************************************************
* getinventory()															 *
******************************************************************************
* displays player's inventory list											 *
******************************************************************************
* Params:																	 *
*	itemData item[] - array of items in the game							 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void getInventory(itemData items[])
{
	int index;				// loop index
	string inventoryStr;	// inventory list
	bool noItems = true;	// no items in player inventory

	display("You are carrying the following Items:");
	inventoryStr = "";

	for(index = 0; index < maxItems;index++)	// search itemData array
	{
		if(items[index].itemLocation == inventory)	// if it is in inventory
		{
			noItems = false;						// we got items
			inventoryStr.append(items[index].itemName);		// add the name to the list
			if(items[index].isEquipped)
				inventoryStr.append(" [Equipped]");		// let player know if it is equipped
			inventoryStr.append(", ");
		}
	}
	if(!noItems)	// if we got items, clean up the list
		inventoryStr.append("\b \b\b \b.");
	else			// else report back no items
		inventoryStr = "Nothing";
	display(inventoryStr);
	return;
}

/*****************************************************************************
* hideItem()																 *
******************************************************************************
* Hides an item in a target. (places item in room 0, and places item in		 *
* target's hidden  array. if target is non existant (room 0,) it removes the *
* item from the game.														 *
******************************************************************************
* Params:																	 *
*	itemData tohide - the item to hide										 *
*	itemData target - the item to hide the other item behind, under,  or	 *
*					with.													 *
*	int room - the room number we are in.									 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void hideItem(itemData item[], int toHide, int target, int room)		// hide an item
{
	string hideStr;	// status of hiding an item
	int index;		// search index

	for(index = 0; index < 5; index++)		// find an empty spot in the list of the item to hide it with
	{
		if(item[target].hiddenItems[index] ==0)
			break;
	}

	if(item[toHide].itemLocation != inventory && item[toHide].itemLocation != room)		// if not in the room or inventory, tell player
		display("I can't find what you want to hide");
	else if (index == 5 || item[target].itemLocation  == inventory)		// if our index hits 5 (no emty slots,) or hiding item is in inventory, tell player
		display("You can't hide this here.");
	else			// hide the item
	{
		if(item[toHide].itemLocation == inventory)		// tell player we pull it from inventory
			hideStr = "Removing it from your inventory, you";
		else
			hideStr = "You pick it up and";				// else tell player we pick it up to hide it
		hideStr.append(" hide it using the ");
		hideStr.append(item[target].itemName);

		item[toHide].itemLocation = 0;		// make the hidden item's location 0
		item[target].hiddenItems[index] = item[toHide].itemNumber;	// save the hidden item's number in our empty slot

		display(hideStr,true);	// inform player of above actions
	}
	return;
}


/*****************************************************************************
* attackItem()																 *
******************************************************************************
* sets each room encountered to a default value								 *
******************************************************************************
* Params:																	 *
*	itemData weapon - the weapon item										 *
*	int target - the item number of the target.								 *
*	int weaponID - the item number of the weapon.							 *
*	int room - the room number we are in.									 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void attackItem(itemData item[],int target, int weaponID, int room)
{
	string attackStr;	// our attackstatus

	if(item[target].itemLocation != room)		// if the target isn't in the room
		display("You can't attack what isn't here.",true);
	else if(!item[target].isAttackable)		// if the target isn't attackable
		display("You can't attack that!,true");
	else if(item[target].isDead)				// if the target is already dead
		display("Your target is already defeated!",true);
	else if(item[weaponID].damage == 0 || !item[weaponID].isWeapon)		// if not a weapon, or can't do damage
		display("It's useless to attack with that!",true);
	else								// complete the attack
	{
		attackStr = "You Attack and ";
		item[target].health -= item[weaponID].damage;

		if(item[target].health <= 0)			// if we kill target		
		{
			item[target].health = 0;
			item[target].isDead = true;
			attackStr.append("defeat the ");
		}
		else						// if not, we hurt the target
			attackStr.append("hurt the ");
		attackStr.append(item[target].itemName);
		display(attackStr,true);					// tell user of attack
	}
	return;
}
