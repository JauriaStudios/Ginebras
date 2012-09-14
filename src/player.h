#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "characters.h"

/**********************************************************
 **** STRUCTS 
 **********************************************************/
typedef struct Player {
	Character **charVector;
	int numChars;
} Player;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Player* PlayerConstructor(Character **vectorChar, int numChars);
extern void PlayerDestructor(Player *this);

#endif // _PLAYER_H_
