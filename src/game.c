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

CharacterInput charData[] = {
	{.orientation= ORIENT_SOUTH, .x0 = 16*SPRITE_SIZE-(16), .y0 = 4*SPRITE_SIZE/2, .iniciative = 8, .player = 1},
	{.orientation= ORIENT_SOUTH, .x0 = 14*SPRITE_SIZE-(16), .y0 = 4*SPRITE_SIZE/2, .iniciative = 7, .player = 1},
	{.orientation= ORIENT_SOUTH, .x0 = 12*SPRITE_SIZE-(16), .y0 = 4*SPRITE_SIZE/2, .iniciative = 6, .player = 1},
	{.orientation= ORIENT_SOUTH, .x0 = 10*SPRITE_SIZE-(16), .y0 = 4*SPRITE_SIZE/2, .iniciative = 5, .player = 1},
	{.orientation= ORIENT_NORTH, .x0 = 15*SPRITE_SIZE-(16), .y0 = 7*SPRITE_SIZE/2, .iniciative = 4, .player = 2},
	{.orientation= ORIENT_NORTH, .x0 = 13*SPRITE_SIZE-(16), .y0 = 7*SPRITE_SIZE/2, .iniciative = 3, .player = 2},
	{.orientation= ORIENT_NORTH, .x0 = 11*SPRITE_SIZE-(16), .y0 = 7*SPRITE_SIZE/2, .iniciative = 2, .player = 2},
	{.orientation= ORIENT_NORTH, .x0 =  9*SPRITE_SIZE-(16), .y0 = 7*SPRITE_SIZE/2, .iniciative = 1, .player = 2},
	{}
};

/**********************************************************
 *** INTERNAL PROTOTYPES
 **********************************************************/
static Character** GameParseData(Game *game, char *filename, Map *map);

/**********************************************************
 *** METHOD IMPLEMENTATION
 **********************************************************/
Game* GameConstructor(Map *map, char *fileGame)
{
	// Variable definition section
	Game *game;
	Player *player1, *player2;
	Character *tmp, *max = NULL;
	Character **vectorChar, **vectorChar1, **vectorChar2;
	int i, j, add;
	//int numChars = 4 + 4;

	// Alloc game
	game = (Game *)malloc(sizeof(Game));
	
	//game->characters = 
	vectorChar = GameParseData(game, fileGame, map);

	vectorChar1 = (Character **)malloc(sizeof(Character*) * game->numChar/2);
	vectorChar2 = (Character **)malloc(sizeof(Character*) * game->numChar/2);

	for(i = 0; i < game->numChar/2; i++)
		vectorChar1[i] = vectorChar[i];
	
	for(j = 0; j < game->numChar/2; j++){
		vectorChar2[j] = vectorChar[i];
		i++;
	}

	// Create players
	player1 = PlayerConstructor(vectorChar1, 4, 1);
	player2 = PlayerConstructor(vectorChar2, 4, 2);

	game->player1 = player1;
	game->player2 = player2;

	game->map = map;

	// Alloc player1 and player2 characters
	//charVector = (Character**)malloc(sizeof(Character *) * (numChars));

	/*
	for(i = 0; i < player1->numChars; i++)
		charVector[i] = player1->charVector[i];
	
	for(j = 0; j < player2->numChars; j++){
		charVector[i] = player2->charVector[j];
		i++;
	}
	*/
	
	// Game initialization
	//game->numChar = numChars;
	INIT_LIST_HEAD(&game->listCharacters);
	game->numChecks = 0;

	// Save sorted characters in the list
	add = 0;
	while(add != game->numChar){
		for(j = 0; j < game->numChar; j++){
			if(!vectorChar[j]->check){
				max = vectorChar[j];
				goto yes;
			}
		}	
yes:

		for(i = 0; i < game->numChar; i++){
			if((vectorChar[i]->iniciative > max->iniciative) && (!vectorChar[i]->check)){
				max = vectorChar[i];
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

			if(cursor->free)
				CursorSetNonFree(cursor, game->actualCharacter);

			game->actualCharacter = tmp;
			//tmp->moveArea->state = NONE;
			AreaSetPosition(tmp->moveArea, tmp->rcDest.x, tmp->rcDest.y);
			CursorSetPosition(cursor, tmp->rcDest.x, tmp->rcDest.y);
			CursorSetCollisionArea(cursor, tmp->movement);
			tmp->rcInitTurn.x = tmp->rcDest.x;
			tmp->rcInitTurn.y = tmp->rcDest.y;

			break;
		}
	}
	
}

/**********************************************************
 *** INTERNAL METHOD IMPLEMENTATION
 **********************************************************/
Character** GameParseData(Game *game, char *filename, Map *map)
{
	// Variable definition section
	xmlDocPtr doc;
	xmlNodePtr cur;
	//xmlChar * tmp = NULL;
	int numChars;
	Character **charVector;
	int i = 0;

	// Load xml file
	if(!(doc = xmlParseFile(filename))){
		printf("CharacteParseData ERROR: file %s not parsed successfully\n", filename);
		return NULL;
	}

	// Get xml root
	if (!(cur = xmlDocGetRootElement(doc))) {
		printf("CharacteParseData ERROR: empty root character\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	// Exist general characters info
	if (xmlStrcmp(cur->name, (const xmlChar *) "game")) {
		printf("CharacteParseData ERROR: document of the wrong type, root node != game\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	numChars = atoi((char*)xmlGetProp(cur, (xmlChar*)"numChars"));

	game->numChar = numChars;

	printf("PARSECHARS, numChars: %d\n", numChars);

	// Alloc vector
	charVector = (Character**)malloc(sizeof(Character *)*numChars);

	cur = cur->xmlChildrenNode;

	// Create list
	while (cur) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"character"))){
			printf("*\n");	
			charVector[i] = CharacterConstructor(cur, charData[i], map);
			i++;
		}
		cur = cur->next;
	}

	return charVector;
}
