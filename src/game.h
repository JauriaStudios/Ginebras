#ifndef _GAME_H_
#define _GAME_H_

#include "list.h"
#include "characters.h"
#include "player.h"
#include "cursor.h"

typedef struct Game {
	struct list_head listCharacters;
	Character *actualCharacter;
	Player *player1, *player2;
	int numChar;
	int numChecks;
} Game;

extern Game* GameConstructor(Player *player1, Player *player2);
extern int GameMoveCharacters(Game *game);
extern int GameDrawCharacters(Game *game, SDL_Surface *screen, Map *map);
extern void GameDestructor(Game *game);
extern void GameActionChar(Game *game, Cursor *cursor);

#endif // _GAMES_H_
