#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"
#include "characters.h"
//#include "cursor.h"

#define SCREEN_WIDTH  	800
#define SCREEN_HEIGHT 	600

#define TILE_SIZE	32

#define BUFFER_SIZE 1024

#define MAP_SIZE_X 40
#define MAP_SIZE_Y 40

#define FRAMES_PER_SECOND 30

int NUM_TILES_Y = (SCREEN_HEIGHT / TILE_SIZE) + 1;
int NUM_TILES_X = (SCREEN_WIDTH / TILE_SIZE) + 1;


//char cursorDirection[6];

SDL_Rect rcCursor;

//void loadBackgroundImages(char *filename, int x, int y, SDL_Surface *ground[MAP_SIZE_X][MAP_SIZE_Y]);

void loadScenario(char *filename,int background[][MAP_SIZE_Y]);

//void drawSprite(SDL_Surface* image, SDL_Surface* dest, int x, int y, int frame_dir, int frame_anim);

void HandleEvent(SDL_Event event, Character* character, Cursor* cursor);

void moveCursor(char *cursorDirection);

typedef struct Timer{
	
	// The clock time when the timer started
	int startTicks;

	//The ticks stored when the timer was paused
    int pausedTicks;

	//The timer status
    unsigned paused;
    unsigned started;

} Timer;

Timer* TimerConstructor(void)
{
	Timer* timer;
	timer = (Timer *)malloc(sizeof(Timer));
	timer->startTicks  = 0;
	timer->pausedTicks = 0;
	timer->paused	   = 0;
	timer->started	   = 0;

	return timer;
}

void TimerStart(Timer *timer)
{
	// Start the timer
	timer->started = 1;

	// Unpaused the timer
	timer->paused = 0;

	timer->startTicks = SDL_GetTicks();
}

void TimerStop(Timer *timer)
{
	timer->started = 0;
	timer->paused  = 0;
}

void TimerPause(Timer *timer)
{
    //If the timer is running and isn't already paused
    if( ( timer->started ) && ( !timer->paused ) ){
        //Pause the timer
        timer->paused = 1;
 
        //Calculate the paused ticks
        timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
    }
}

int modeCursor = 0;
int gameover   = 0;
int showGrid   = 0;

int main(int argc, char** argv)
{

	// Variable initialization
	int background[MAP_SIZE_X][MAP_SIZE_Y];
	//int objects[MAP_SIZE_X][MAP_SIZE_Y];
//int collisions[MAP_SIZE_X][MAP_SIZE_Y];

	SDL_Surface *screen, *ground[MAP_SIZE_X][MAP_SIZE_Y], *grid;
	SDL_Rect rcGrass, rcGrid;

	Character *heroe, *squel, *topos;
	Timer* timer;
	Cursor* cursor;

	int x, y;

	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	// set the title bar 
	SDL_WM_SetCaption("The legend of Ginebras", "Jauria productions");
	
	// create window 
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	
	// set keyboard repeat 
	SDL_EnableKeyRepeat(70, 70);
	
	// load ground file
	loadScenario("data/mapa.txt",background);
	
	// Load grid
	grid   = loadImage("data/grid.bmp");
	
	//load background images
	for(x = 0; x < NUM_TILES_X; x++){
		for(y = 0; y < NUM_TILES_Y; y++){
			char buff[20];
			
			sprintf(buff, "data/%d.bmp",background[x][y]);
			//printf("%s\n",buff);
			
		ground[x][y] = loadImage(buff);
			//loadBackgroundImages(buff, i, j, ground);
		}
	}

	// Create a char
	heroe = CharacterConstructor("data/heroe1.bmp", ORIENT_SOUTH, 3*SPRITE_SIZE, 4*SPRITE_SIZE);
	squel = CharacterConstructor("data/esqueletico.bmp", ORIENT_EAST, 1*SPRITE_SIZE, 6*SPRITE_SIZE);
	topos = CharacterConstructor("data/topos.bmp", ORIENT_WEST, 5*SPRITE_SIZE, SPRITE_SIZE);

	// Create a cursor
	cursor = CursorConstructor(heroe->rcDest.x, heroe->rcDest.y);

	// Create a timer
	timer = TimerConstructor();


	while (!gameover)
	{
		// Statrt the timer
		TimerStart(timer);

		SDL_Event event;		
		
		// look for an event 
		if (SDL_PollEvent(&event)) {
			HandleEvent(event, heroe, cursor);
		}

		// collide with edges of screen
		/*
		if(modeCursor) 
			isCollide(&rcCursor);
		else
			isCollide(&rcSprite);
		*/
		
		// draw the background
		for (x = 0; x < NUM_TILES_X; x++) {
			for (y = 0; y < NUM_TILES_Y; y++) {
				rcGrass.x = x * TILE_SIZE;
				rcGrass.y = y * TILE_SIZE;
				SDL_BlitSurface(ground[x][y], NULL, screen, &rcGrass);
			}
		}
		
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
		CharacterMove(heroe);

		// Draw character
		CharacterDraw(heroe, screen);
		CharacterDraw(squel, screen);
		CharacterDraw(topos, screen);

		// draw the cursor
		CursorDraw(cursor, screen);		
		
		// update the screen 
		SDL_UpdateRect(screen, 0, 0, 0, 0);

		// Cap the frame rate
		if( timer->startTicks < 1000 / FRAMES_PER_SECOND ){
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer->startTicks );
        }

	}// end main while
	
	// clean up 
	for (x = 0; x < NUM_TILES_X; x++) {
		for (y = 0; y < NUM_TILES_Y; y++) {
			SDL_FreeSurface(ground[x][y]);
		}
	}

	SDL_Quit();
	return 0;
}

/*
void loadBackgroundImages(char *filename, int x, int y, SDL_Surface *ground[MAP_SIZE_X][MAP_SIZE_Y])
{
	SDL_Surface *temp;
	// load ground
	temp   = IMG_Load(filename);
	ground[x][y] = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
}
*/


void loadScenario(char* filename, int background[MAP_SIZE_X][MAP_SIZE_Y])
{
	FILE* pInput;

	char buf[BUFFER_SIZE];
	
	pInput = fopen(filename, "r");
	int x = 0;
	int y = 0;

	while(fgets(buf, sizeof(buf), pInput) != NULL)
	{
		char *tok = strtok(buf, ",");
		int max = MAP_SIZE_Y;
		for(x = 0; x < max;x++){
	 	        background[x][y]= atoi(tok);
			tok = strtok(NULL, ",");
		}
		y++;
	}
	fclose (pInput);
}

/*void drawSprite(SDL_Surface* image, SDL_Surface* dest, int x, int y, int frame_dir, int frame_anim)
{q
	//posicion
	SDL_Rect  rcSrc, posicion;
	posicion.x = x;
	posicion.y = y;

	// 
	frame_anim = frame_anim * 64;
	frame_dir = frame_dir * 64;

	// Coordenadas de la imagen fuente
	rcSrc.x = frame_anim;
	rcSrc.y = frame_dir;
	
	// Tamaño imagen fuente
	rcSrc.w = SPRITE_SIZE;
	rcSrc.h = SPRITE_SIZE;

	//escribir imagen en destino, posicion (x,y)
	SDL_BlitSurface(image, &rcSrc, dest, &posicion);
}
*/
void HandleEvent(SDL_Event event, Character* character, Cursor* cursor)
{
	switch (event.type) {
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;
		/* handle the mouse */
	/*	case SDL_MOUSEBUTTONDOWN:
			if(character->rcDest.x == cursor->rcDest.x && character->rcDest.y == cursor->rcDest.y )
				break;
			else
				CharacterSetDestination(character, cursor);		
			break;
		*/	
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
					//printf("Mouse button %d pressed at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
					if(character->rcDest.x == cursor->rcDest.x && character->rcDest.y == cursor->rcDest.y )
						break;
					else
						CharacterSetDestination(character, cursor);		
					break;

				default:
					break;
			}
		break;
	}
}


