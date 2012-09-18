#include "characters.h"

Character* CharacterConstructor(char* file, Orientation or, int x0, int y0, int iniciative)
{
	Character* character;
	character = (Character *)malloc(sizeof(Character));
	
	// Load sprites movement image
	if ((character->sprite = loadImage(strcat(file, ".png"))) == NULL)
		return NULL;
	
	// Load sprites slash image
	if ((character->sprite = loadImage(strcat(file, "Slash.png"))) == NULL)
		return NULL;
	
	// Set sprite initial position
	character->rcDest.x = x0;
	character->rcDest.y = y0;	

	// Set initial animation sprite frame
	character->rcSrc.x = 0;
	character->rcSrc.y = or * SPRITE_SIZE;
	character->rcSrc.w = SPRITE_SIZE;
	character->rcSrc.h = SPRITE_SIZE;
	
	// Set initial velocity
	character->velocity = 2;

	// Set initial destination point
	character->destinationPoint.x = x0;
	character->destinationPoint.y = y0;
	
	// Set initial state
	character->moving = 0;
	character->actualStep = 0;
	character->moveSteps  = 0;

	// Set linked list
	INIT_LIST_HEAD(&character->list);
	INIT_LIST_HEAD(&character->listSort);

	// Set initial turn variables
	character->check = 0;
	character->iniciative = iniciative;

	// Set initial attack variables
	character->attackState = 0;

	return character;
}

void CharacterDestructor(Character *character)
{
	// Free character image
	SDL_FreeSurface(character->sprite);

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
	character->destinationPoint.x = cursor->rcDest.x;
	character->destinationPoint.y = cursor->rcDest.y;
	
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
		character->moveX[character->moveSteps-1] = cursor->rcDest.x;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y;
	}else{
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x;			
			character->moveY[i] = character->rcDest.y + ( (i *  dy ) / character->moveSteps ) ;	
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y;
	}
}



void CharacterMove(Character *character)
{
	// Set the coordenates, if character is moving
	if( character->actualStep < character->moveSteps ){
		character->rcDest.x = character->moveX[character->actualStep];
		character->rcDest.y = character->moveY[character->actualStep];
		character->actualStep++;

		// Animation
		character->rcSrc.y = character->moveOrient * SPRITE_SIZE;
		if(character->skipFrames == NUM_SKIP_FRAMES) {
			if (character->moveState == 8)
				character->moveState = 1;
			else
				character->moveState++;
			character->skipFrames = 0;
		}else
			character->skipFrames++;
	}else
		character->moveState = 0;
	character->rcSrc.x = character->moveState * SPRITE_SIZE;
}

void CharacterDraw(Character* character, SDL_Surface* screen)
{
	SDL_BlitSurface(character->sprite, &character->rcSrc, screen, &character->rcDest);
}

void CharacterAttack(Character *character)
{
	if(character->skipFrames == NUM_SKIP_FRAMES) {
		if (character->attackStateState == 5)
			character->attackState = 1;
		else
			character->attackState++;
		character->skipFrames = 0;
	}else
		character->skipFrames++;

	character->rcSrc.x = character->moveState * SPRITE_SIZE;

}
