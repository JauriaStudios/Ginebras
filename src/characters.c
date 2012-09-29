#include "characters.h"

Character* CharacterConstructor(char* file, Orientation or, int x0, int y0, int iniciative, int movement, int **pos)
{
	Character* character;
	character = (Character *)malloc(sizeof(Character));
	char spriteMove[75];
	char spriteSlash[75];
	int coordX, coordY;

	strcpy(spriteMove, file); 
	strcat(spriteMove, ".png");
	// Load sprites movement image
	if (!(character->sprite = loadImage(spriteMove))){
		printf("Character constructor ERROR: couldn't load character sprite\n");
		return NULL;
	}
	
	strcpy(spriteSlash, file); 
	strcat(spriteSlash, "Slash.png");
	// Load sprites slash image
	if (!(character->spriteSlash = loadImage(spriteSlash))){
		printf("Character constructor ERROR: couldn't load character slash sprite\n");
		return NULL;
	}

	// Set movement
	character->movement = movement;
	if (!(character->moveArea = AreaConstructor(x0, y0, movement))){
		printf("Character constructor ERROR: couldn't create movement area\n");
		return NULL;
	}
	
	// Set sprite initial position
	character->rcDest.x = x0;
	character->rcDest.y = y0;

	// Set initial animation sprite frame
	character->rcSrc.x = 0;
	character->rcSrc.y = or * SPRITE_SIZE;
	character->rcSrc.w = SPRITE_SIZE;
	character->rcSrc.h = SPRITE_SIZE;
	
	// Set initial character position
	GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);
	pos[coordY][coordX] = 1;
	
	// Set initial velocity
	character->velocity = 4;

	// Set initial destination point
	character->destinationPoint.x = x0;
	character->destinationPoint.y = y0;
	
	// Set initial state
	character->moving = 0;
	character->actualStep = 0;
	character->moveSteps  = 0;
	character->moveState = 0;

	// Set linked list
	INIT_LIST_HEAD(&character->list);
	INIT_LIST_HEAD(&character->listSort);

	// Set initial turn variables
	character->check = 0;
	character->iniciative = iniciative;

	// Set initial attack variables
	character->rcSrcAttack.x = 0;
	character->rcSrcAttack.y = or * SPRITE_SIZE;
	character->rcSrcAttack.w = SPRITE_SIZE;
	character->rcSrcAttack.h = SPRITE_SIZE;
	
	character->attackState = 0;
	character->attacking = 0;
	character->actualAttackStep = 0;

	return character;
}

void CharacterDestructor(Character *character)
{
	// Free character image
	SDL_FreeSurface(character->sprite);
	
	// Free movement area
	AreaDestructor(character->moveArea);
	
	// Free character
	free(character);
}

void CharacterSetDestination(Character* character, Cursor* cursor)
{
	float m, b; 
	int i, dx, dy;
	int stepsX, stepsY;
	
	// Set status move
	character->moving     = 1;
	character->actualStep = 0;
	character->moveState  = 1;
	character->moveSteps  = 0;
	character->skipFrames = 0;

	// Set destination point
	character->destinationPoint.x = cursor->rcDest.x - 16;
	character->destinationPoint.y = cursor->rcDest.y - 32;
	
	//  Slope of the line
	dx = character->destinationPoint.x - character->rcDest.x;
	dy = character->destinationPoint.y - character->rcDest.y;
	
	// skip infinite slope for the vertical movement
	if(dx)
		m = (float) dy/dx;
	else
		m = 0.0;

	// b of the line
	b = character->rcDest.y - ( m*character->rcDest.x );
	
	// steps
	stepsX = abs( dx ) / character->velocity;
	//printf("STEPS X: %d\n", stepsX);
	stepsY = abs( dy ) / character->velocity;
	//printf("STEPS Y: %d\n", stepsY);
	
	character->moveSteps = ceil(sqrt( (stepsX*stepsX) + (stepsY*stepsY) ));
	
	// Set orientation movement
	if( (dy < 0) && (stepsY > stepsX) )
		character->moveOrient = ORIENT_NORTH;
	else if( (dy > 0) && ( stepsY > stepsX ) )
		character->moveOrient = ORIENT_SOUTH;
	else if( (dx > 0) && ( stepsX >= stepsY) )
		character->moveOrient = ORIENT_EAST;
	else if( (dx < 0) && ( stepsX >= stepsY ) )
		character->moveOrient = ORIENT_WEST;

	// Initialitate move vectors
	character->moveX = (int *)malloc(sizeof(int)*character->moveSteps);
	character->moveY = (int *)malloc(sizeof(int)*character->moveSteps);

	// Set move vectors
	if(dx){
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x + ( (i *  dx ) / character->moveSteps ) ;			
			character->moveY[i] = m*character->moveX[i] + b; 
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x - 16;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y - 32;
	}else{
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x;			
			character->moveY[i] = character->rcDest.y + ( (i *  dy ) / character->moveSteps ) ;	
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x - 16;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y - 32;
	}
}

void CharacterMove(Character *character, Map *map)
{
	int coordNwX, coordNwY, coordNeX, coordNeY, coordSwX, coordSwY, coordSeX, coordSeY;
	int firstX, firstY;
	
	// Set the coordenates, if character is moving
	if( character->actualStep < character->moveSteps ){
		
		// Check collition
		printf("(%d, %d)\n", 11, 12);
		GetCoor(character->moveX[0]+16+16, character->moveY[0]+32+16, &firstX, &firstY);
		GetCoor(character->moveX[character->actualStep]+16+1, character->moveY[character->actualStep]+32+1, &coordNwX, &coordNwY);
		GetCoor(character->moveX[character->actualStep]+16+32-1, character->moveY[character->actualStep]+32+1, &coordNeX, &coordNeY);
		GetCoor(character->moveX[character->actualStep]+16+1, character->moveY[character->actualStep]+32+32-1, &coordSwX, &coordSwY);
		GetCoor(character->moveX[character->actualStep]+16+32-1, character->moveY[character->actualStep]+32+32-1, &coordSeX, &coordSeY);

		//printf("(%d, %d) charpos: %d ", coordX, coordY, map->charPosition[coordY][coordX]);
		//printf("(%d, %d) collision: %d\n", coordX, coordY, map->colisions[coordY][coordX]);

		// Check collision and skip first coordenate
		if(((map->charPosition[coordNwY][coordNwX] || map->colisions[coordNwY][coordNwX]) || 
		    (map->charPosition[coordNeY][coordNeX] || map->colisions[coordNeY][coordNeX]) ||
		    (map->charPosition[coordSwY][coordSwX] || map->colisions[coordSwY][coordSwX]) ||																			 (map->charPosition[coordSeY][coordSeX] || map->colisions[coordSeY][coordSeX])) && 
		    (!map->charPosition[firstY][firstX])){

			character->actualStep = character->moveSteps;
			goto fin;
		}else{
			character->rcDest.x = character->moveX[character->actualStep];
			character->rcDest.y = character->moveY[character->actualStep];
			character->actualStep++;
		}
	
		// Animation
		character->rcSrc.y = character->moveOrient * SPRITE_SIZE;
		printf("%d \n", character->skipFrames);
		if(character->skipFrames == NUM_SKIP_FRAMES) {
			if (character->moveState == 8)
				character->moveState = 1;
			else
				character->moveState++;
			character->skipFrames = 0;
		}else
			character->skipFrames++;
	}else{
fin:
		character->moveState = 0;
		character->moving = 0;
		//free(character->moveX);
		//free(character->moveY);
	}
	character->rcSrc.x = character->moveState * SPRITE_SIZE;
}

void CharacterDraw(Character* character, SDL_Surface* screen, Map *map)
{
	if(character->moving)
		SDL_BlitSurface(character->sprite, &character->rcSrc, map->surfaceBack, &character->rcDest);
	else if(character->attacking)
		SDL_BlitSurface(character->spriteSlash, &character->rcSrcAttack, map->surfaceBack, &character->rcDest);
	else
		SDL_BlitSurface(character->sprite, &character->rcSrc, map->surfaceBack, &character->rcDest);	
}

void CharacterSetAttack(Character *character)
{
	// Set status attack
	character->attacking  		= 1;
	character->actualAttackStep = 0;
	character->attackState  	= 1;
	character->attackSteps  	= 4;
	character->skipFrames 		= 0;

}

void CharacterAttack(Character *character)
{
	
	if( character->actualAttackStep < character->attackSteps ){
		character->rcSrcAttack.y = character->moveOrient * SPRITE_SIZE;
		if(character->skipFrames == NUM_SKIP_FRAMES) {
			if (character->attackState == 5)
				character->attackState = 1;
			else{
				character->attackState++;
				character->actualAttackStep++;
			}
			character->skipFrames = 0;
		}else{
			character->skipFrames++;
		}
	}else{
		character->attackState = 0;
		character->attacking = 0;
	}
	character->rcSrcAttack.x = character->attackState * SPRITE_SIZE;
	
}
