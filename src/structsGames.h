#ifndef _STRUCTS_GAMES_H_
#define _STRUCTS_GAMES_H_

#include "list.h"
#include "characters.h"

typedef struct Game {
	struct list_head listCharacters;
	struct list_head listCharactersSort;
	Character *actualCharacter;
	int numChar;
	int numChecks;
} Game;

extern Game* GameConstructor(int numChars);
extern int GameMoveCharacters(Game *game);
extern int GameDrawCharacters(Game *game, SDL_Surface *screen);
extern void GameDestructor(Game *game);
extern void GameActionChar(Game *game);

#endif // _STRUCTS_GAMES_H_
