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
* This contains the data & functions specific to an itwm & it's uses.
********************************************************************************
*	  @pram:	None
*	@return:	None
*   @throws:	N/A
*******************************************************************************/
#pragma once;

#ifndef DEATHLY_HOUSE_ITEMS__H
#define DEATHLY_HOUSE_ITEMS__H

// project includes
#include "TerminalIO.h"
#include "Room.h"
#include "common.h"

struct itemData
{
	int itemNumber;			// item ID number
	string itemName;			//item's name
	string itemDescription;		// description of item
	int itemLocation;			//where the item is located. -1 is player's invnentory, 0 is gone.
	int mapPassage;			// if it is a portal, room to move to. 0 makes it not a portal.
	int requiredItems[5];	// Array LIST of items required to use item "on"
	int hiddenItems[5];		// array list of "hidden" items until this item is moved.
	int health;				// health level of entity (for battles), # of uses otherwise
		  					// ( 0 = used up, unusable, dead (different than position: -1), -1 = infinite uses)
	int damage;				// if a weapon, how much damage it causes.
	int moves;				// Number of moves to beat in, or fail/die (for battles)
	bool isPriority;		// IF a priority, must disable to do anything else
	bool isDeath;			// determines if failure IS death
	bool isDead;			// IF dead, can be ignored, else blocks & priorities are active.
	bool isAttackable;		// can you fight the entity?
	bool isCollectable;		// Determines if Item can be taken
    bool isMoveable;		// Determines if item can be moved
    bool isWeapon;			// Is the Item a weapon
	bool isEquipped;		// is player equipped with it? (weapon item MUST be in inventory: currentPosition == 0)
    bool isDroppable;		// Can the player drop the item?
	bool isKey;				// item is used as a key to "unlock" or unblock (is true) or as a portkey (if false)
};

extern int currentRoomNumber;	// the current room number of the room the player is in.
const int maxItems = 25;		// maximum amount of items in the game +1

void itemDefault(itemData item[]);											// sets default values to all items
void makeItem(itemData item[], int itemNumber, int room);					// create an item from reqItem array
void moveItem(itemData item[],int itemNumber, int room);					// move an item, to unhide what may be hidden
void dropItem(itemData item[], int itemNum, int room);						// drop an item from inventory into room
void pickupItem(itemData item[], int itemNum, int room);					// take an item and place it into inventory
void visibleItems(itemData item[], int room);								// display all unhidden items in a room
int findItem(string name, itemData items[]);								// item is present in room, returns item number.
void equipItem(itemData item[], int itemNum);								// equip an item, if it can be done
void unequipItem(itemData item[], int itemNum);								// unequip an item
void getInventory(itemData items[]);										// the items the player is carrying
void hideItem(itemData item[], int toHideNum, int targetNum, int room);		// hide an item

void attackItem(itemData item[],int target, int weaponID, int room);		// attack an item with a specific item

#endif
