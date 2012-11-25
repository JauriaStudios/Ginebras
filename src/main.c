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

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

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
#include "menu.h"
#include "audio.h"
#include "projectile.h"

#define FRAMES_PER_SECOND 30

/**********************************************************
 **** GLOBAL VARIABLES
 **********************************************************/
int modeCursor    = 1;
int gameover      = 0;
int showGrid      = 0;
int showInterface = 1;
int skipIntro     = 0;
int fullScreen    = 1;
int modeMenu      = 0;
int flag		  = 0;

char *mapName;
char buffMapName[40];

/**********************************************************
 **** TYPES
 **********************************************************/
typedef struct gameCommand {
	const char* mCommand;
	const char* mDesc;
	int (*mAction)(int argc, char** argv);
} gameCommand;

/**********************************************************
 **** PROTOTYPES
 **********************************************************/
static int PrintHelp(int argc, char** argv);
static int Test(int argc, char** argv);
static int LoadMap(int argc, char** argv);
static int Skip(int argc, char** argv);
static int WindowMode(int argc, char** argv);
static void HandleEvent(SDL_Event event, SDL_Surface* screen, Game *game, Cursor *cursor, Map *map, Menu *menu);

/**********************************************************
 **** COMMANDS
 **********************************************************/
static gameCommand Commands[] =
{
	{.mCommand = "--help", .mDesc = "Prints help", .mAction = PrintHelp},
	{.mCommand = "--test", .mDesc ="Tests", .mAction = Test},
	{.mCommand = "--map", .mDesc = "Load specific map", .mAction = LoadMap},
	{.mCommand = "--skip", .mDesc = "Skip intro", .mAction = Skip},
	{.mCommand = "--window", .mDesc = "Starts in window mode", .mAction = WindowMode},
	{}
};

/**********************************************************
 **** INTERNAL METHODS
 **********************************************************/
int PrintHelp(int argc, char** argv)
{
	printf ("Sons of battle Development version \"Ginebras\" ( http://jauria.mooo.com/redmine )\n");
	printf ("Usage: ginebras [Options]\n");
	printf ("OPTIONS:\n");

	int i;

	for (i = 0; i < 5; i++) {
		printf (" %s: %s\n", Commands[i].mCommand, Commands[i].mDesc);
	}

	printf ("EXAMPLES:\n");
	printf ("  ginebras --map arena.tmx\n");
	return -1;
}

int Test(int argc, char** argv)
{
	printf("Test ok \n");
	return -1;
}

int LoadMap(int argc, char** argv)
{	

	// Check Parameters
	if (argc == 2) {
		printf("ERROR: No map name especified, read help to see more \n");
		return -1;
	}
	else if (argc > 3) {
		printf("ERROR: Too few parameters, read help to see more \n");
		return -1;
	}

	mapName = argv[2];
	return 0;
}

int Skip(int argc, char** argv)
{	

	// Check Parameters
	if (argc < 2) {
		printf("ERROR: Too few parameters, read help to see more \n");
		return -1;
	}

	printf("Skip ok \n");
	skipIntro = 1;
	return 0;
}

int WindowMode(int argc, char** argv)
{	

	// Check Parameters
	if (argc < 2) {
		printf("ERROR: Too few parameters, read help to see more \n");
		return -1;
	}

	printf("Window ok \n");
	fullScreen = 0;
	return 0;
}

/**********************************************************
 **** MAIN
 **********************************************************/
int main(int argc, char **argv)
{
	// Variable definition section
	SDL_Surface *screen, *selector;
	SDL_Rect rcSelector;
	SDL_Event event;	
	
	Intro *intro;	
	Game *game;
	Map *map;
	Timer *timer;
	Cursor *cursor;
	Interface *interface;
	Grid *grid;	
	Audio *introSound;
	
	// Default map to load
	mapName = "Pueblo60x80.tmx";
	
	// handle main arguments
	int i = 0;
	
	// Check
	if(argc > 1) {
		// Find command
		while(Commands[i].mAction) {
			if(!strncmp(argv[1], Commands[i].mCommand, strlen(Commands[i].mCommand))){
				if(Commands[i].mAction(argc, argv))
					return -1;
			}
			i++;
		}
	}

	// initialize SDL Video & Audio mixer
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	// set the title bar
	SDL_WM_SetCaption("The legend of Ginebras - Jauria Studios", "Jauria Studios");
	
	// create sdl window 
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF| SDL_FULLSCREEN);

	if(!fullScreen)
		SDL_WM_ToggleFullScreen(screen);
	
	// set keyboard repeat 
	SDL_EnableKeyRepeat(70, 70); // SDL_DEFAULT_REPEAT_INTERVAL
	
	// Load Sound
	introSound = AudioConstructor();
	AudioLoadFile(introSound, "data/sound/intro.wav");
	
	// Game Intro
	if(!skipIntro){
		SDL_Delay(1000);

		AudioPlaySound(introSound);
		intro = IntroConstructor();
		IntroDraw(intro, screen);
		IntroDestructor(intro);
	}

	// Load background
	sprintf(buffMapName,"data/%s",mapName);
	map = MapConstructor(screen, buffMapName);

	// Load grid
	grid = GridConstructor(map->tileWidth, map->tileHeight);

	// Load selector
	if(!(selector = loadImage("data/Selector.png"))){
		printf("Main ERROR: impossible load data/Selector.png\n");
	}

	// Create game
	game = GameConstructor(map, "data/character/turboss.xml");

	// Create game interface
	interface = InterfaceConstructor(8, game);

	// Create a cursor
	cursor = CursorConstructor(game->actualCharacter->rcDest.x, game->actualCharacter->rcDest.y);	

	// (provisional)
	game->cursor = cursor;
	game->menu = interface->menu;

	// Create a timer
	timer = TimerConstructor();

	// Set initial cursor collision
	CursorSetCollisionArea(cursor, game->actualCharacter->movement);
	
	// Main while
	while (!gameover)
	{
		// Start the timer
		TimerStart(timer);

		// Look for an event 
		if (SDL_PollEvent(&event)) {
			HandleEvent(event, screen, game, cursor, map, interface->menu);
		}
				
		// Map update
		MapUpdate(map, cursor->rcDest);

		// Fill screen black
		SDL_FillRect(screen, NULL, 0x0);

		// draw the grid
		if(showGrid)
			GridDraw(grid, map);

		// Draw area
		AreaDraw(game->actualCharacter->moveArea, map, cursor);

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
			InterfaceDraw(interface, screen, game->actualCharacter->avatar, game);

		// Check character end turn
		if(game->actualCharacter->attacking)
			flag = 1;
		if(!(game->actualCharacter->attacking) && flag == 1){
			flag = 0;
			modeMenu = 0;
			MenuClose(interface->menu);
			//GameActionChar(game, cursor); // CHAPUZA ALERT
		}

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
	AudioDestructor(introSound);
	

	SDL_Quit();

	return 0;
}

/**********************************************************
 **** HANDLE EVENTS
 **********************************************************/
void HandleEvent(SDL_Event event, SDL_Surface *screen, Game *game, Cursor *cursor, Map *map, Menu *menu)
{
	// Variable definition section

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
					if(modeMenu && game->actualCharacter->moveArea->state == MOVEMENT && menu->visible) 
						MenuBack(menu); 
					else if(modeCursor) 
						CursorMove(cursor, ORIENT_WEST, map);
					break;
				case SDLK_RIGHT:
					if(modeMenu && game->actualCharacter->moveArea->state == MOVEMENT && menu->visible) 
						MenuOk(menu);
					else if(modeCursor) 
						CursorMove(cursor, ORIENT_EAST, map);
					break;
				case SDLK_UP:
					if(modeMenu && game->actualCharacter->moveArea->state == MOVEMENT && menu->visible) 
						MenuUp(menu);
					else if(modeCursor) 
						CursorMove(cursor, ORIENT_NORTH, map);
					break;
				case SDLK_DOWN:
					if(modeMenu && game->actualCharacter->moveArea->state == MOVEMENT && menu->visible) 
						MenuDown(menu);
					else if(modeCursor)
						CursorMove(cursor, ORIENT_SOUTH, map);
					break;
				case SDLK_t:
					if (fullScreen == 1){
						fullScreen = 0;
						SDL_WM_ToggleFullScreen(screen);
					}
					else{
						fullScreen = 1;
						SDL_WM_ToggleFullScreen(screen);
					}
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
					switch(game->actualCharacter->moveArea->state){
						case MOVEMENT:
							if(game->actualCharacter->rcDest.x +16 == cursor->rcDest.x && game->actualCharacter->rcDest.y +32 == cursor->rcDest.y )
								break;
							else if(!cursor->free){
								CharacterSetDestination(game->actualCharacter, cursor, map);
								break;
							}
						case ATTACK:
							if(CursorCheckEnemy(cursor, map, game->actualCharacter))
								CharacterSetAttack(game->actualCharacter, SLASH192);
							//MenuClose(menu);
							break;
						default:
							break;
					}
					break;
				case SDLK_p:
					MenuClose(menu);
					GameActionChar(game, cursor);
					break;
				case SDLK_a:
					CharacterSetAttack(game->actualCharacter, SLASH);
					game->actualCharacter->projectile->action = 1;
					break;
				case SDLK_s:
					CharacterSetAttack(game->actualCharacter, SLASH192);
					break;
				case SDLK_d:
					CharacterSetAttack(game->actualCharacter, SPELL);
					break;
				case SDLK_i:
					if (showInterface == 1)
						showInterface = 0;
					else
						showInterface = 1;
					break;
				case SDLK_m:
					if (modeMenu == 1){
						modeMenu = 0;
						
						MenuClose(menu);

					}else{
						modeMenu = 1;
						menu->visible = 1;
					}
					break;
				case SDLK_f:
					if(cursor->free){
						CursorSetNonFree(cursor, game->actualCharacter);
					}else{
						CursorSetFree(cursor);
					}
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
