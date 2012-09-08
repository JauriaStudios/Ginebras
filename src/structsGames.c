#include "SDL.h"
#include "SDL_image.h"
#include "structsGames.h"

Game* GameConstructor(int numChars)
{
	// Variable definition section
	Game *game;
	Character *character;
	Character *tmp, *tmp2, *max;
	Character **charVector;
	int i, j, founded;

	// Alloc game
	game = (Game *)malloc(sizeof(Game));
	charVector = (Character**)malloc(sizeof(Character *)*numChars);

	// Game initialization
	game->numChar = 0;
	INIT_LIST_HEAD(&game->listCharacters);
	INIT_LIST_HEAD(&game->listCharactersSort);
	game->numChecks = 0;

	// Build a new Character
	if( !(character = CharacterConstructor("data/heroe1.png", ORIENT_SOUTH, 3*SPRITE_SIZE, 4*SPRITE_SIZE, 4))) {
		printf("GAME: error building a new character\n");
		return NULL;
	}
	
	game->actualCharacter = character;
	list_add_tail(&character->list, &game->listCharacters);
	charVector[0] = character;
	game->numChar++;

	// Build a new Character
	if( !(character = CharacterConstructor("data/esqueletico.png", ORIENT_EAST, 1*SPRITE_SIZE, 6*SPRITE_SIZE, 3))){
		printf("GAME: error building a new character\n");
		return NULL;
	}
	list_add_tail(&character->list, &game->listCharacters);
	charVector[1] = character;
	game->numChar++;

	// Build a new Character
	if( !(character = CharacterConstructor("data/topos.png", ORIENT_WEST, 5*SPRITE_SIZE, SPRITE_SIZE, 5))) {
		printf("GAME: error building a new character\n");
		return NULL;
	}
	list_add_tail(&character->list, &game->listCharacters);
	charVector[2] = character;
	game->numChar++;

	// Build a new Character
	if( !(character = CharacterConstructor("data/topos.png", ORIENT_WEST, 2*SPRITE_SIZE, 6*SPRITE_SIZE, 2))) {
		printf("GAME: error building a new character\n");
		return NULL;
	}
	list_add_tail(&character->list, &game->listCharacters);
	charVector[3] = character;
	game->numChar++;

/*
	// Sort the list
	while(founded < numChars){
		i = 0;
		//max = list_first_entry(&game->listCharacters, Character, list);
		max = charVector[0]; 
		printf("ini: %d\n", max->iniciative);
		for(j = 0; j < numChars ; j++) {
			tmp = charVector[j];
			printf("qqqqqqq\n");
			if(tmp->iniciative > max->iniciative){
				printf("iniciative in: %d\n", tmp->iniciative);
				//list_move_tail(&tmp->listSort, &game->listCharactersSort);
				//list_del_init(&tmp->list);
				//list_add_tail(&tmp->listSort, &game->listCharactersSort);
				//found++;
			}
		}
	}
*/
	return game;
	
}

void GameDestructor(Game *game)
{
	// Variable definition section
	Character *tmp, *tmp2;

	list_for_each_entry_safe(tmp, tmp2, &game->listCharacters, list) {
		list_del(&tmp->list);
		CharacterDestructor(tmp);
	}
	
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

void GameActionChar(Game *game)
{
	// Variable definition section
	Character* tmp;
	list_for_each_entry(tmp, &game->listCharactersSort, listSort){
		printf("iniciative: %d\n", tmp->iniciative);
	}

}
