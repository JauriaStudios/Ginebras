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

#include "game.h"

Game* GameConstructor(Player *player1, Player *player2, Map *map)
{
	// Variable definition section
	Game *game;
	Character *tmp, *max = NULL;
	Character **charVector;
	int i, j, add;
	int numChars = player1->numChars + player2->numChars;

	// Alloc game
	game = (Game *)malloc(sizeof(Game));
	
	game ->map = map;
	// Alloc player1 and player2 characters
	charVector = (Character**)malloc(sizeof(Character *) * (numChars));

	for(i = 0; i < player1->numChars; i++)
		charVector[i] = player1->charVector[i];
	
	for(j = 0; j < player2->numChars; j++){
		charVector[i] = player2->charVector[j];
		i++;
	}
	
	// Game initialization
	game->numChar = numChars;
	INIT_LIST_HEAD(&game->listCharacters);
	game->numChecks = 0;

	// Sort the list
	add = 0;
	while(add != numChars){
		for(j = 0; j < numChars; j++){
			if(!charVector[j]->check){
				max = charVector[j];
				goto yes;
			}
		}	
yes:
		for(i = 0; i < numChars; i++){
			if((charVector[i]->iniciative > max->iniciative) && (!charVector[i]->check)){
				max = charVector[i];
			}
		}
		list_add_tail(&max->list, &game->listCharacters);
		max->check = 1;
		add++;
	}

	list_for_each_entry(tmp, &game->listCharacters, list){
		tmp->check = 0;
	}

	game->actualCharacter = list_first_entry(&game->listCharacters, Character, list);
	game->actualCharacter->check = 1;

	return game;
}

void GameDestructor(Game *game)
{
	/*
	// Variable definition section
	Character *tmp, *tmp2;

	list_for_each_entry_safe(tmp, tmp2, &game->listCharacters, list) {
		list_del(&tmp->list);
		CharacterDestructor(tmp);
	}
	*/
	free(game);

}

int GameMoveCharacters(Game *game, Map *map)
{
	// Variable definition section
	Character* tmp;

	list_for_each_entry(tmp, &game->listCharacters, list){
		CharacterMove(tmp, map);
	}
	
	return 0;
}

int GameDrawCharacters(Game *game, SDL_Surface *screen, Map *map)
{
	// Variable definition section
	Character* tmp;

	list_for_each_entry(tmp, &game->listCharacters, list){
		CharacterDraw(tmp, screen, map);
	}
	
	return 0;
}

void GameActionChar(Game *game, Cursor *cursor)
{
	// Variable definition section
	Character* tmp;
	int endTurn = 1;
	list_for_each_entry(tmp, &game->listCharacters, list){
		if(!tmp->check)
			endTurn = 0;
	}

	if(endTurn)
		list_for_each_entry(tmp, &game->listCharacters, list){
			tmp->check = 0;
		}
	
	
	list_for_each_entry(tmp, &game->listCharacters, list){
		if(!tmp->check){
			tmp->check = 1;
			game->actualCharacter = tmp;
			AreaSetPosition(tmp->moveArea, tmp->rcDest.x, tmp->rcDest.y);
			CursorSetPosition(cursor, tmp->rcDest.x, tmp->rcDest.y);
			CursorSetCollisionArea(cursor, tmp->movement);
			break;
		}
	}
	
}
