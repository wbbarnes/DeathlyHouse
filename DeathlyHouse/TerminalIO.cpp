/*******************************************************************************
*   Program: The Deathly House
*      File: TerminalIO.cpp
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
//#include "common.h"
#include "TerminalIO.h"


/*****************************************************************************
* pause()																	 *
******************************************************************************
* function to pause the screen.												 *
*																			 *
* NOTE: Uses Microsoft VC++ function _getch() as the pause mechanism. Should *
* work on most ANSI compliant compilers in the DOS or WINDOWS environment    *
* with minor modification (dropping the _  and leaving it as getch()).		 *
* *nix World either needs to run in curses window, hope termio/termcaps has	 *
* it, or completely rewrite this function. It WILL NOT work on streams, it   *
* will ONLY on the local console.											 *
*																			 *
* _getch() waits for keyboard activity and returns the key pressed. Since we *
* don't really care what key that was, we ignore the returned value of this  *
* function.																	 *
******************************************************************************
* Params:																	 *
*	None																	 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None 																	 *
*****************************************************************************/
void pause(void)
{
	string msg = "Press any key to continue";	// pause prompt message
	char bksp = '\b';							// backspace character
	unsigned int bkspCounter;					// counter used as we erase

	cout << msg;
	_getch();					//Using MSVC++ C function for the pause,
								// *Nix users need to change this up.
	for(bkspCounter=0; bkspCounter < msg.length() ; bkspCounter++)
		cout << bksp << ' ' << bksp;		//erase our pause prompt
	return;
}


/*****************************************************************************
* display()																	 *
******************************************************************************
* Display a string to stdout (cout) with word wrap and optional new line	 *
*																			 *
******************************************************************************
* Params:																	 *
*	string str - the string to be displayed to the screen					 *
*	bool newLine - print a new line at the end of the string				 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void display(string str,bool newLine)
{
	unsigned int col = screenWidth-1;	// start position of our last screen column in string
	int start = 0;		// start position of our first screen column in string
	int tooLongs = 0;	// how many times to increase the screen width
	string row;			// the part of the string destined to be printed per loop itteration.

	while (col > 0)		// as log as we haven't ran out of columns
	{
		if(str.length() < col)		// if our unprinted string length is less than the screen width
		{
			cout << str.substr(start);		// go ahead and print it
			if(newLine)
				cout   << endl;
			col = 0;						// and set our column count to 0. Done
		}
		else if( str.c_str()[col] == ' ')	// if we found our "last" space before the screen width
		{
			row = str.substr(start,col-start);	// get our section we want to print
			cout << row << endl;
			start = col + 1;				// adjust our start for the space we found AND the end of our last print.
			col += (screenWidth-1);			// adjust our starting point by the width of the screen.
		}
		else		// we haven't found a space yet, and our unprinted string section is wider than our screen
		{
			col--;	// so we back up our "screen width" point until we run into a space.
			if (col == 0)		//if we hit the end, let's expand the search
			{
				tooLongs++;		// how many times to increase the screen width
				col = tooLongs * screenWidth;
				if (col > str.length())	// if we have exceeded the length, reset it to length
					col = str.length();

				// if we are over one screen width larger, give up and print the whole thing
				if((tooLongs * screenWidth) > (str.length() + screenWidth))
				{
					cout << str;		// print the string
					if(newLine)			// print a newline if newLine is true
						cout << endl;
					col = 0;			// reset columns to the very first one
				}
			}
		}
	}
	return;
}

/*****************************************************************************
* display()																	 *
******************************************************************************
* WRAPPER for display(string str) to allow the use of a C string array to	 *
* Display a string to stdout (cout) with word wrap and optional new line	 *
******************************************************************************
* Params:																	 *
*	char *str - pointer to the C string array to display					 *
*	bool newLine - print a new line at the end of the string				 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void display(char *str,bool newLine)
{
	string printStr;	// string to receive our C string array
	printStr = str;		// convert C string array to C++ string class
	display(printStr, newLine);	// call our display function with the C++ string
	return;
}


/*****************************************************************************
* display()																	 *
******************************************************************************
* WRAPPER to Display a string to stdout (cout) with word wrap and automatic	 *
* new line																	 *
******************************************************************************
* Params:																	 *
*	char *str - pointer to the C string array to display					 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void display(string str)
{
	display(str,true);
}

/*****************************************************************************
* display()																	 *
******************************************************************************
* WRAPPER for display(string str) to allow the use of a C string array to	 *
* Display a string to stdout (cout) with word wrap and automatic new line	 *
******************************************************************************
* Params:																	 *
*	char *str - pointer to the C string array to display					 *
* Returns:																	 *
*	None																	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
void display (char *str)
{
	display(str,true);
}


/*****************************************************************************
* displayFile()																 *
******************************************************************************
* "prints" a file to the output stream. Allows for tabs and the backspace    *
* escape character to be printed normally. Also handles new lines in the	 *
* format of <CR> ('\r'), <LF> ('\n'), or <CRLF> ("\r\n").					 *
*																			 *
* This is the only TerminalIO file that is also a FileIO type				 *
******************************************************************************
* Params:																	 *
*	string - The file to open and print out.								 *
* Returns:																	 *
*	bool - how successful the print was. false = fail, true = successful	 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
bool displayFile(string fname)
{
	bool failure;		// flag to signal a failed read and display.
	int columnPos;		// where our cursor is on the current line.
	int lineCount;		// the line number we are on
	char character;		// the character read in from the data std::stringeam
	char nextChar;		// used for a peek.
	int tabMove;		// number of spaces to add to make a tab;

	ifstream input(fname);		// setup our input std::stringeam from the given file
	
	//initialze variables
	failure = false;	// We haven't encounterd a failure, yet.
	columnPos = 0;		// We are starting at the very first character on the screen
	lineCount = 0;		// we are starting at the very first line on the screen

	if(input)			// successful opeining, go ahead and print the file.
	{
		while (!input.eof())		// as long as we haven't encountered an EOF, we do the following:
		{
			input.get(character);	// read a character from the file
			switch (character)		// test our character for special characters
			{
				case '\b':	if (columnPos>0)	// BACKSPACE unless we are already on the first column.
							{
								cout << '\b' << ' ' << '\b';		//backspace (bs) with destruct
								columnPos--;
							}
							break;
				case '\t':	tabMove = tabStop - (columnPos % tabStop);	// TAB - find out how many spaces needed
							while(tabMove !=0 )	// if we need to add spaces for our tab
							{
								tabMove--;		// decrease the amount of spaces to add.
								columnPos++;	// count our cursor advanced
								cout << ' ';	// add our space
							}
							break;
				case '\f':	while (lineCount++ < screenLen)		// FORM FEED (ff), New Page (np)
								cout << endl;
							columnPos=0;
							lineCount = 0;
							break;
				case '\r':	// CARRIAGE RETURN - Falls Through to NEW LINE:
							// \r for Mac & CoCo, \n for *nix, \r\n for DOS/Windows new line.
							nextChar = input.peek();
							if(nextChar == '\n')	//throw away the NL if previous was a CR.
								input.get(nextChar);
				case '\n':	lineCount++;		// NEW LINE - increment our line count.
							columnPos = 0;		// reset our cursor position
							cout << endl;		// print a new line (let compiler handle differences)
							break;
//				case '\\':	cout << '\\';		// print our BACKSLASH
//							columnPos++;		// count our cursor advanced
//							break;
				default:	if (character < ' ')	//throw away any OTHER CONTROL CHARACTERS
								;
							else						// print our character out
							{
								columnPos++;			
								cout << character;
							}
			}
			if (lineCount > screenLen)			// if we got a full screen, pause
			{
				pause();		// pause the screen
				lineCount = 0;	// reset line count
			}
		}
		input.close();			// close the file, we're done
	}
	else				// failure to open file. display a error message.
		failure = true;
	return(failure);
}


/*****************************************************************************
* getInput()																 *
******************************************************************************
* Displays promt text, if present, and displays a prompt point. It then		 *
* reads	in stdin (cin), makes the input text lower case and prints a blank	 *
* line after the user input													 *
******************************************************************************
* Params:																	 *
*	string - The prompt text string.										 *
* Returns:																	 *
*	string - the string read in from the console							 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
string getInput(string prompt)
{
	string input;				// our input string

	if (prompt.length() > 0)	// if prompt text exists,
		display(prompt);		// display it
	cout << "> ";				// display our prompt
	getline(cin,input);			// read in our whole line
	input = lowerString(input);	// make it lower case
	display(" ");				// place a blank line on the screen
	return(input);
}


// STRING MANIPULATION FUNCTIONS

/*****************************************************************************
* lowerString()																 *
******************************************************************************
* HELPER FUNCTION: converts a string to a lower case string					 *
******************************************************************************
* Params:																	 *
*	string str - the string to convert to lower case						 *
* Returns:																	 *
*	string newStr - the converted sstring									 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
 string lowerString(string str)
{
	char *conversion;		// pointer used for a new char array to use for case conversion
	unsigned int index;		// loop counter index for char array string
	string newStr;			// newly converted string

	conversion = new char[str.length()+1];		// create a new array of the needed size.
	strcpy(conversion,str.c_str());			// copy the string into a char array
	for(index =0; index < strlen(conversion); index++)
	{
		if(isupper(conversion[index]))	// convert the characters to lower case
			conversion[index] = tolower(conversion[index]);
	}
	newStr = conversion;				// replace the unconverted string with the converted string
	delete conversion;					// memory clean up, this version of conversion no longer needed

	return(newStr);
}


/*****************************************************************************
* stripLeading()															 *
******************************************************************************
* HELPER FUNCTION: strips leading non-alpha characters from beginning of a   *
* string																	 *
******************************************************************************
* Params:																	 *
*	string str - the string to convert to lower case						 *
* Returns:																	 *
*	string newStr - the converted sstring									 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
string stripLeading(string str)
{
	string newStr;			// the cleaned up string
	unsigned int index;		// index for the for-loop

	for(index =0; index < str.length(); index++)	//strip any leading non-alpha chars
	{
		if (isalpha(str.c_str()[index]))	// on first alphabetic char
			{
				newStr = str.substr(index);	//retrieve the command without the leading space.
				break;
			}
	}
	return(newStr);
}


/*****************************************************************************
* stripTrailing()															 *
******************************************************************************
* HELPER FUNCTION: strips trailing non-alpha characters from beginning of a  *
* string																	 *
******************************************************************************
* Params:																	 *
*	string str - the string to convert to lower case						 *
* Returns:																	 *
*	string newStr - the converted sstring									 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
string stripTrailing(string str)			// strip trailing whitespace & numbers off a string
{
	string newStr;
	unsigned int index;

	for(index = str.length(); index >=0; index--)	//strip any leading non-alpha chars
	{
		if (isalpha(str.c_str()[index]))	// on first alphabetic char
			{
				newStr = str.substr(0,index+1);	// retrieve the command without the leading space.
				break;							// we're done searching the string.
			}
	}
	return(newStr);		// send back the changed string
}


/*****************************************************************************
* leftStr()																	 *
******************************************************************************
* HELPER FUNCTION: retrieves the left-most word from a string.				 *
******************************************************************************
* Params:																	 *
*	string str - the string to convert to lower case						 *
* Returns:																	 *
*	string left - the left-most word										 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
string leftStr(string str)
{
	string left;			// the left-most word in the given string
	unsigned int index;		// the index to find the first space in the string

	for(index = 0; index < str.length(); index++)
	{
		if(str.c_str()[index] == ' ')	// if we found that first space
		{
			left = str.substr(0,index);	// load our string with the first word
			break;						// we're done looking for a space.
		}
	}
	if (index == str.length())		// if we didn't find a space at all
		left = str;					// we only had one word, so it's the left-most
	return(left);			// send it back
}


/*****************************************************************************
* rightStr()																 *
******************************************************************************
* HELPER FUNCTION: retrieves the right-most word from a string.				 *
******************************************************************************
* Params:																	 *
*	string str - the string to convert to lower case						 *
* Returns:																	 *
*	string right - the right-most word										 *
* Throws/Catches:															 *
*	None																	 *
*****************************************************************************/
string rightStr(string str)
{
	string right;			// the right-most word in a string
	unsigned int index;		// the index to find the first space in the string

	for(index = str.length()-1; index > 0; index--)
	{
		if(str.c_str()[index] == ' ')	// if we found that last space
		{
			right = str.substr((index+1));	// load our string with the last word
			break;						// we're done looking for a space.
		}
	}
	if (index == 0)		// if we didn't find a space at all
		right = str;					// we only had one word, so it's the right-most
	return(right);			// send it back
}
