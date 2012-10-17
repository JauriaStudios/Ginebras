/* Copyright (C) 2012 Jauria Studios
 *
 * Sons of Battle is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Sons of Battle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sons of battle. if not, see <http://www.gnu.org/licenses/>.
 */

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
extern int GameMoveCharacters(Game *game, Map *map);
extern int GameDrawCharacters(Game *game, SDL_Surface *screen, Map *map);
extern void GameDestructor(Game *game);
extern void GameActionChar(Game *game, Cursor *cursor);

#endif // _GAMES_H_
