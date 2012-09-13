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

#define SCREEN_WIDTH  		800
#define SCREEN_HEIGHT 		600
#define FRAMES_PER_SECOND 	10

static void HandleEvent(SDL_Event event, Game *game, Cursor *cursor);

int modeCursor = 0;
int gameover   = 0;
int showGrid   = 0;

int main(int argc, char **argv)
{
	// Variable initialization

	SDL_Surface *screen, *grid;
	SDL_Rect rcGrid;
	SDL_Event event;	

	Game *game;
	Map *map;
	Timer *timer;
	Cursor *cursor;

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
	grid  = loadImage("data/grid.png");

	// Create game
	game = GameConstructor(4);

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
			for (x = 0; x < SCREEN_WIDTH / SPRITE_SIZE; x++) {
				for (y = 0; y < SCREEN_HEIGHT / SPRITE_SIZE; y++) {
					rcGrid.x = x * SPRITE_SIZE;
					rcGrid.y = y * SPRITE_SIZE;
					SDL_BlitSurface(grid, NULL, screen, &rcGrid);
				}
			}
			
		}
	
		// Move characters
		GameMoveCharacters(game);

		// Draw Characters
		GameDrawCharacters(game, screen);

		// Draw the cursor
		CursorDraw(cursor, screen);		

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
					if(game->actualCharacter->rcDest.x == cursor->rcDest.x && game->actualCharacter->rcDest.y == cursor->rcDest.y )
						break;
					else
						CharacterSetDestination(game->actualCharacter, cursor);		
					break;
				case SDLK_p:
					GameActionChar(game);
					break;
				default:
					break;
			}
		/* handle the mouse */
		/*case SDL_MOUSEBUTTONDOWN:
			printf("Mouse button %d pressed at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
			if(character->rcDest.x == cursor->rcDest.x && character->rcDest.y == cursor->rcDest.y )
				break;
			else
				CharacterSetDestination(character, cursor);		
			break;
		*/

			break;
	}
}


