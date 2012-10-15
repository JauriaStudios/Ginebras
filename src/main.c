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
#include "interface.h"
#include "textbox.h"
#include "intro.h"
#include "grid.h"

#define FRAMES_PER_SECOND 30

static void HandleEvent(SDL_Event event, SDL_Surface* screen, Game *game, Cursor *cursor, Map *map);
static Character** vectorCharsGen(int option, int **pos, Map *map);

int modeCursor = 0;
int gameover   = 0;
int showGrid   = 0;
int showInterface = 1;

int main(int argc, char **argv)
{
	// Variable definition section
	SDL_Surface *screen, *selector;
	SDL_Rect rcSelector;
	SDL_Event event;	

	Intro *intro;	
	Game *game;
	Player *player1, *player2;
	Map *map;
	Timer *timer;
	Cursor *cursor;
	Interface *interface;
	Grid *grid;	

	// Only for developement
	Character **vectorChar1, **vectorChar2;


	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	// set the title bar 
	SDL_WM_SetCaption("The legend of Ginebras - Jauria productions", "Jauria productions");
	
	// create window 
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF); // SDL_FULLSCREEN
	
	// set keyboard repeat 
	SDL_EnableKeyRepeat(70, 70); // SDL_DEFAULT_REPEAT_INTERVAL
	
	// Game Intro
	intro = IntroConstructor();
	IntroDraw(intro, screen);
	IntroDestructor(intro);
		
	// Load background
	map = MapConstructor(screen, "data/Pueblo60x80.tmx");
	
	// Load grid
	grid = GridConstructor(map->tileWidth, map->tileHeight);

	// Load selector
	selector = loadImage("data/Selector.png");

	// Create game interface
	interface = InterfaceConstructor(8);

	// Generate character vector (provisional sólo para desarrollo)
	vectorChar1 = vectorCharsGen(1, map->charPosition, map);
	vectorChar2 = vectorCharsGen(2, map->charPosition, map);

	// Create players
	player1 = PlayerConstructor(vectorChar1, 4);
	player2 = PlayerConstructor(vectorChar2, 4);

	// Create game
	game = GameConstructor(player1, player2);

	// Create a cursor
	cursor = CursorConstructor(game->actualCharacter->rcDest.x, game->actualCharacter->rcDest.y);	
	
	// Create a timer
	timer = TimerConstructor();

	// Set initial cursor collision
	CursorSetCollisionArea(cursor, game->actualCharacter->movement);
	
	// Main while
	while (!gameover)
	{
		// Start the timer
		TimerStart(timer);

		// look for an event 
		if (SDL_PollEvent(&event)) {
			HandleEvent(event, screen, game, cursor, map);
		}
				
		// Map update
		MapUpdate(map, cursor->rcDest);

		// Fill screen black
		SDL_FillRect(screen, NULL, 0x0);

		// draw the grid
		if(showGrid)
			GridDraw(grid, map);

		// Draw area
		//AreaDraw(game->actualCharacter->moveArea, screen, map);		
		AreaSmartDraw(game->actualCharacter->moveArea, map, cursor);

		// Draw cursor
		CursorDraw(cursor, map);	

		// Move characters 
		GameMoveCharacters(game, map);	
		
		// Attack character
		CharacterAttack(game->actualCharacter);

		// Draw Characters
		GameDrawCharacters(game, screen, map);
		
		// Draw actual character
		CharacterDraw(game->actualCharacter, screen, map);		

		// Draw selector
		rcSelector.x = game->actualCharacter->rcDest.x + 16;
		rcSelector.y = game->actualCharacter->rcDest.y - 16;
		SDL_BlitSurface(selector, NULL, map->surfaceBack, &rcSelector);
				
		// Draw background
		MapDraw(map, screen);

		// Draw interface
		if(showInterface)
			InterfaceDraw(interface, screen);

		// Update the screen 
		SDL_Flip(screen);

		// Cap the frame rate
		if( timer->startTicks < 1000 / FRAMES_PER_SECOND ){
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer->startTicks); 
		}

	}// end main while
	
	// Free
	GridDestructor(grid);
	GameDestructor(game);
	MapDestructor(map);
	CursorDestructor(cursor);
	InterfaceDestructor(interface);
	SDL_Quit();

	return 0;
}

void HandleEvent(SDL_Event event, SDL_Surface* screen, Game *game, Cursor* cursor, Map *map)
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
				case SDLK_t:
					SDL_WM_ToggleFullScreen(screen);
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
					else if(!cursor->free)
						CharacterSetDestination(game->actualCharacter, cursor, map);		
					break;
				case SDLK_p:
					GameActionChar(game, cursor);
					break;
				case SDLK_a:
					CharacterSetAttack(game->actualCharacter, SLASH);
					break;
				case SDLK_s:
					CharacterSetAttack(game->actualCharacter, SLASH192);
					break;
				case SDLK_i:
					if (showInterface == 1)
						showInterface = 0;
					else
						showInterface = 1;
					break;
				case SDLK_f:
					if(cursor->free){
						// Set cursor on character position
						CursorSetPosition(cursor, game->actualCharacter->rcDest.x, game->actualCharacter->rcDest.y);
				
						// Set cursor in the middle of the collision matrix
						cursor->coordX = (cursor->sideLength-1)/2;
						cursor->coordY = (cursor->sideLength-1)/2;	
			
						cursor->free = 0;
					}else
						cursor->free = 1;
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

Character** vectorCharsGen(int option, int **pos, Map *map)
{
	// Variable definition section
	Character **charVector;
	Character *character;
	int i, j;

	// Alloc vector
	charVector = (Character**)malloc(sizeof(Character *)*4);

	if(option == 1){
		// Build a new Character
		if( !(character = CharacterConstructor("data/character/plate", ORIENT_SOUTH, 16*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 7, 3, pos))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[0] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/chain", ORIENT_SOUTH, 14*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 6, 4, pos))){
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[1] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/hat", ORIENT_SOUTH, 12*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 5, 5, pos))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[2] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/leather", ORIENT_SOUTH, 10*SPRITE_SIZE-(16), 4*SPRITE_SIZE/2, 2, 6, pos))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[3] = character;
	}else {
		// Build a new Character
		if( !(character = CharacterConstructor("data/character/bone", ORIENT_NORTH, 15*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2, 7, 1, pos))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[0] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/link", ORIENT_NORTH, 13*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2, 6, 2, pos))){
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[1] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/guard", ORIENT_NORTH, 11*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2, 5, 3, pos))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[2] = character;

		// Build a new Character
		if( !(character = CharacterConstructor("data/character/thief", ORIENT_NORTH, 9*SPRITE_SIZE-(16), 7*SPRITE_SIZE/2, 2, 4, pos))) {
			printf("GAME: error building a new character\n");
			return NULL;
		}
		charVector[3] = character;
	}

	printf("\n");
	for(i=0;i<map->height;i++){
		for(j=0;j<map->width;j++)
			printf("%d ", map->charPosition[i][j]);	
		printf("\n");
	}

	return charVector;
}
