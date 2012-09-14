#include "player.h"

Player* PlayerConstructor(Character **vectorChar, int numChars)
{
	// Varaiable definition section
	Player *this;
	
	// Alloc memory
	if(!(this = (Player*) malloc(sizeof(Player))))
		return NULL;	
	
	// Initialize player
	this->charVector = vectorChar;
	
	// CHAPUZA ALERT!
	this->numChars = numChars;
	
	return this;
}

void PlayerDestructor(Player *this)
{
	// Variable definition section
	int i;
	
	// Free chars
	for(i = 0; i < this->numChars; i++)
		free(this->charVector[i]);
	free(this->charVector);

}
