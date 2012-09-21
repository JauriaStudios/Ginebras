#include "game.h"

Game* GameConstructor(Player *player1, Player *player2)
{
	// Variable definition section
	Game *game;
	Character *tmp, *max = NULL;
	Character **charVector;
	int i, j, add;
	int numChars = player1->numChars + player2->numChars;

	// Alloc game
	game = (Game *)malloc(sizeof(Game));
	
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

int GameMoveCharacters(Game *game)
{
	// Variable definition section
	Character* tmp;

	list_for_each_entry(tmp, &game->listCharacters, list){
		CharacterMove(tmp);
	}
	
	return 0;
}

int GameDrawCharacters(Game *game, SDL_Surface* screen)
{
	// Variable definition section
	Character* tmp;

	list_for_each_entry(tmp, &game->listCharacters, list){
		CharacterDraw(tmp, screen);
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
			CursorSetPosition(cursor, tmp->rcDest.x, tmp->rcDest.y);
			break;
		}
	}
	
}
