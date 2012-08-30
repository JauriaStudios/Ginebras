#include "characters.h"

Character* CharacterConstructor(char* file, Orientation or, int x0, int y0)
{
	Character* character;
	character = (Character *)malloc(sizeof(Character));
	// Load sprites image
	if ((character->sprite = loadImage(file)) == NULL)
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
	character->velocity = 3;

	// Set initial destination point
	character->destinationPoint.x = x0;
	character->destinationPoint.y = y0;
	
	// Set initial state
	character->moving = 0;
	character->actualStep = 0;
	character->moveSteps = 0;

	return character;
}

void CharacterSetDestination(Character* character, Cursor* cursor)
{
	float m, b; 
	int i, dx, dy;
	int stepsX, stepsY;
	
	// Set status move
	character->moving = 1;
	character->actualStep = 0;
	character->moveState = 1;

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

	if(stepsX > stepsY){
		character->moveSteps = stepsX;
	}else{
		character->moveSteps = stepsY;
	}
	
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
	// Set the coordenates
	if( character->actualStep < character->moveSteps ){
		character->rcDest.x = character->moveX[character->actualStep];
		character->rcDest.y = character->moveY[character->actualStep];
		character->actualStep++;

		// Animation
		character->rcSrc.y = character->moveOrient * SPRITE_SIZE;
		if (character->moveState == 8)
			character->moveState = 1;
		else
			character->moveState++;
	}else
		character->moveState = 0;
	character->rcSrc.x = character->moveState * SPRITE_SIZE;
}

void CharacterDraw(Character* character, SDL_Surface* screen)
{
	SDL_BlitSurface(character->sprite, &character->rcSrc, screen, &character->rcDest);
}


Cursor* CursorConstructor(int x0, int y0)
{
	Cursor* cursor;
	cursor = (Cursor *)malloc(sizeof(Cursor));
	// Load sprites image
	if ((cursor->sprite = loadImage("data/cursor.png")) == NULL)
		return NULL;
	
	// Set sprite initial position
	cursor->rcDest.x = x0;
	cursor->rcDest.y = y0;	

	return cursor;
}

void CursorDraw(Cursor* cursor, SDL_Surface* screen)
{
	SDL_BlitSurface(cursor->sprite, NULL, screen, &cursor->rcDest);
}

void CursorMove(Cursor* cursor, Orientation orientation)
{
	switch (orientation) {
		case ORIENT_WEST:
			cursor->rcDest.x = cursor->rcDest.x - SPRITE_SIZE; 
			break;
		case ORIENT_EAST:
			cursor->rcDest.x = cursor->rcDest.x + SPRITE_SIZE;
			break;
		case ORIENT_SOUTH:
			cursor->rcDest.y = cursor->rcDest.y + SPRITE_SIZE;
			break;
		case ORIENT_NORTH:
			cursor->rcDest.y = cursor->rcDest.y - SPRITE_SIZE;
			break;
		default:
			break;
	}

}

SDL_Surface* loadImage(char* filename)
{
    SDL_Surface* temp = NULL;
    SDL_Surface* image = NULL;
 
    //cargar imagen temporal
    temp = IMG_Load(filename);
    if(temp == NULL ) return NULL;
 
    //cambiar formato y liberar imagen temp
    image = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    if(image == NULL) return NULL;
 
    //elegimos como color el 'rosa magico' y lo hacemos color clave
    Uint32 colorKey = SDL_MapRGB(image->format, 0xFF, 0, 0xFF);
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
 
    //devolver imagen final
    return image;
}
