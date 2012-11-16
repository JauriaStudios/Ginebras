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

/**********************************************************
 *** INTERNAL PROTOTYPES
 **********************************************************/
static Character** GameVectorCharsGen(int option, Map *map);
static int GameParseData(Game *game, char *filename);
static void GameConfigureCharactersInput(Game *this); 

/**********************************************************
 *** METHOD IMPLEMENTATION
 **********************************************************/
Game* GameConstructor(Map *map, char *fileGame)
{
	// Variable definition section
	Game *game;
	Player *player1, *player2;
	Character *tmp, *max = NULL;
	Character **charVector;
	Character **vectorChar1, **vectorChar2;
	int i, j, add;
	int numChars = 4 + 4;

	// Alloc game
	game = (Game *)malloc(sizeof(Game));
	
	GameParseData(game, fileGame);

	// Generate character vector (provisional sólo para desarrollo)
	vectorChar1 = GameVectorCharsGen(1, map);
	vectorChar2 = GameVectorCharsGen(2, map);

	// Create players
	player1 = PlayerConstructor(vectorChar1, 4, 1);
	player2 = PlayerConstructor(vectorChar2, 4, 2);

	game->map = map;

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

	// Save sorted characters in the list
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
int GameParseData(Game *game, char *filename)
{
	// Variable definition section
	xmlDocPtr doc;
	xmlNodePtr cur;
	//xmlChar * tmp = NULL;
	int numChars;

	// Load xml file
	if(!(doc = xmlParseFile(filename))){
		printf("CharacteParseData ERROR: file %s not parsed successfully\n", filename);
		return -1;
	}

	// Get xml root	
	if (!(cur = xmlDocGetRootElement(doc))) {
		printf("CharacteParseData ERROR: empty root character\n");
		xmlFreeDoc(doc);
		return -1;
	}
	
	// Exist general characters info
	if (xmlStrcmp(cur->name, (const xmlChar *) "game")) {
		printf("CharacteParseData ERROR: document of the wrong type, root node != game\n");
		xmlFreeDoc(doc);
		return -1;
	}

	numChars = atoi((char*)xmlGetProp(cur, (xmlChar*)"numChars"));

	printf("PARSECHARS, numChars: %d\n", numChars);

	cur = cur->xmlChildrenNode;

	// Create list
	while (cur) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"character"))){
			printf("*\n");	
		}
	
		cur = cur->next;
	}
	return 0;
}

Character** GameVectorCharsGen(int option, Map *map)
{
	// Variable definition section
	Character **charVector;
	Character *character;
	//int i, j, k;

	// Alloc vector
	charVector = (Character**)malloc(sizeof(Character *)*4);

	if(option == 1){
		// Build a new Character
		if( !(character = CharacterConstructor("data/character/soja", ORIENT_SOUTH, 16*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 
												7, 3, map->charPosition[1], 1, 1))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[0] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/chain", ORIENT_SOUTH, 14*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 
												6, 4, map->charPosition[1], 1, 1))){
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[1] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/hat", ORIENT_SOUTH, 12*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2,
												5, 5, map->charPosition[1], 1, 1))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[2] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/leather", ORIENT_SOUTH, 10*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 
												2, 6, map->charPosition[1], 1, 1))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[3] = character;
	}else {
		// Build a new Character
		if( !(character = CharacterConstructor("data/character/TurBoss", ORIENT_NORTH, 15*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2, 
												7, 20, map->charPosition[2], 2, 2))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[0] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/link", ORIENT_NORTH, 13*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2,
												6, 2, map->charPosition[2], 2, 2))){
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[1] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/guard", ORIENT_NORTH, 11*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2,
												5, 3, map->charPosition[2], 2, 2))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[2] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/thief", ORIENT_NORTH, 9*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2,
												2, 4, map->charPosition[2], 2, 2))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[3] = character;
	}

/*	
	for(k=0;k<3;k++){
		for(i=0;i<map->height;i++){
			for(j=0;j<map->width;j++)
				printf("%d ", map->charPosition[k][i][j]);	
			printf("\n");
		}
		printf("\n\n");
	}
*/
	return charVector;
}
