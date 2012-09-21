#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"
#include "characters.h"
#include "timer.h"
#include "cursor.h"
#include "game.h"
#include "gameUtilities.h"
#include "map.h"
#include "player.h"
#include "area.h"

#define SCREEN_WIDTH  		800
#define SCREEN_HEIGHT 		600
#define FRAMES_PER_SECOND 	10

static void HandleEvent(SDL_Event event, Game *game, Cursor *cursor);
static Character** vectorCharsGen(int option);

int modeCursor = 0;
int gameover   = 0;
int showGrid   = 0;

int main(int argc, char **argv)
{
	// Variable definition section
	SDL_Surface *screen, *grid, *selector;
	SDL_Rect rcGrid, rcSelector;
	SDL_Event event;	

	Game *game;
	Player *player1, *player2;
	Map *map;
	Timer *timer;
	Cursor *cursor;

	// Only for developement
	Character **vectorChar1, **vectorChar2;

	int x, y;
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	// set the title bar 
	SDL_WM_SetCaption("The legend of Ginebras", "Jauria productions");
	
	// create window 
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	
	// set keyboard repeat 
	SDL_EnableKeyRepeat(70, 70);
	
	// Load background
	map = MapConstructor();
	MapLoad(map, "data/map.tmx");

	// Load grid
	grid = loadImage("data/Grid.png");

	// Load selector
	selector = loadImage("data/Selector.png");

	// Generate character vector (provisional sólo para desarrollo)
	vectorChar1 = vectorCharsGen(1);
	vectorChar2 = vectorCharsGen(2);

	// Create players
	player1 = PlayerConstructor(vectorChar1, 4);
	player2 = PlayerConstructor(vectorChar2, 4);

	// Create game
	game = GameConstructor(player1, player2);

	// Create a cursor
	cursor = CursorConstructor(game->actualCharacter->rcDest.x, game->actualCharacter->rcDest.y);	

	// Create a timer
	timer = TimerConstructor();

	// Main while
	while (!gameover)
	{
		// Start the timer
		TimerStart(timer);

		// look for an event 
		if (SDL_PollEvent(&event)) {
			HandleEvent(event, game, cursor);
		}
	
		// Map update
		MapUpdate(map, cursor->rcDest);

		// Fill screen black
		SDL_FillRect(screen, NULL, 0);

		// Draw background
		MapDraw(map, screen);

		// draw the grid
		if(showGrid){
			for (x = 0; x < SCREEN_WIDTH / (SPRITE_SIZE/2); x++) {
				for (y = 0; y < SCREEN_HEIGHT / (SPRITE_SIZE/2); y++) {
					rcGrid.x = x * SPRITE_SIZE/2;
					rcGrid.y = y * SPRITE_SIZE/2;
					SDL_BlitSurface(grid, NULL, screen, &rcGrid);
				}
			}
		}

		// Draw area
		AreaDraw(game->actualCharacter->moveArea, screen);		

		// Draw cursor
		CursorDraw(cursor, screen);	

		// Move characters
		CharacterMove(game->actualCharacter);
	
		// Attack character
		CharacterAttack(game->actualCharacter);

		// Draw Characters
		GameDrawCharacters(game, screen);
		
		// Draw selector
		rcSelector.x = game->actualCharacter->rcDest.x + 16;
		rcSelector.y = game->actualCharacter->rcDest.y - 16;
		SDL_BlitSurface(selector, NULL, screen, &rcSelector);

		// Draw actual character
		CharacterDraw(game->actualCharacter, screen);		

		// Update the screen 
		SDL_UpdateRect(screen, 0, 0, 0, 0);

		// Cap the frame rate
		if( timer->startTicks < 1000 / FRAMES_PER_SECOND ){
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer->startTicks); 
        }

	}// end main while
	
	// Clean game and characters
	GameDestructor(game);
	MapDestructor(map);
	CursorDestructor(cursor);

	SDL_Quit();

	return 0;
}

void HandleEvent(SDL_Event event, Game *game, Cursor* cursor)
{
	switch (event.type) {
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;
			
		/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;
				case SDLK_LEFT:
					if(modeCursor) CursorMove(cursor, ORIENT_WEST);
					break;
				case SDLK_RIGHT:
					if(modeCursor) CursorMove(cursor, ORIENT_EAST);
					break;
				case SDLK_UP:
					if(modeCursor) CursorMove(cursor, ORIENT_NORTH);
					break;
				case SDLK_DOWN:
					if(modeCursor) CursorMove(cursor, ORIENT_SOUTH);
					break;
				case SDLK_c:
					if (modeCursor == 1)
						modeCursor = 0;
					else
						modeCursor = 1;
					break;
				case SDLK_g:
					if (showGrid == 1)
						showGrid = 0;
					else
						showGrid = 1;
					break;
				case SDLK_SPACE:
					if(game->actualCharacter->rcDest.x +16 == cursor->rcDest.x && game->actualCharacter->rcDest.y +32 == cursor->rcDest.y )
						break;
					else
						CharacterSetDestination(game->actualCharacter, cursor);		
					break;
				case SDLK_p:
					GameActionChar(game, cursor);
					break;
				case SDLK_a:
					CharacterSetAttack(game->actualCharacter);
					break;
				default:
					break;
			}
		/*
		//handle the mouse
		case SDL_MOUSEBUTTONDOWN:
			printf("Mouse button %d pressed at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
			if(game->actualCharacter->rcDest.x == cursor->rcDest.x && game->actualCharacter->rcDest.y == cursor->rcDest.y )
				break;
			else
				CharacterSetDestination(game->actualCharacter, cursor);		
			break;
		*/
		default:
			break;
	}
}

Character** vectorCharsGen(int option)
{
	// Variable definition section
	Character **charVector;
	Character *character;

	// Alloc vector
	charVector = (Character**)malloc(sizeof(Character *)*4);

	if(option == 1){
		// Build a new Character
		if( !(character = CharacterConstructor("data/heroe1", ORIENT_SOUTH, 5*SPRITE_SIZE-(16), 4*SPRITE_SIZE, 7, 3))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[0] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/esqueletico", ORIENT_SOUTH, 2*SPRITE_SIZE-(16), 4*SPRITE_SIZE, 6, 4))){
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[1] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/topos", ORIENT_SOUTH, 3*SPRITE_SIZE-(16), 4*SPRITE_SIZE, 5, 5))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[2] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/topos", ORIENT_SOUTH, 4*SPRITE_SIZE-(16), 4*SPRITE_SIZE, 2, 6))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[3] = character;
	}else {
		// Build a new Character
		if( !(character = CharacterConstructor("data/heroe1", ORIENT_NORTH, 5*SPRITE_SIZE-(16), 7*SPRITE_SIZE, 7, 1))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[0] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/esqueletico", ORIENT_NORTH, 2*SPRITE_SIZE-(16), 7*SPRITE_SIZE, 6, 2))){
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[1] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/topos", ORIENT_NORTH, 3*SPRITE_SIZE-(16), 7*SPRITE_SIZE, 5, 3))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[2] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/topos", ORIENT_NORTH, 4*SPRITE_SIZE-(16), 7*SPRITE_SIZE, 2, 4))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[3] = character;
	}

	return charVector;
}
