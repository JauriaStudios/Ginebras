#include <math.h>
#include "map.h"

/**********************************************************
 *** INTERNAL STRUCTS
 **********************************************************/
typedef struct Layer{
	char *name;
	xmlChar *csvLayer;
	SDL_Surface* imageLayer;
	int **data;
	SDL_Rect rcDest;
	int width, height;
	struct list_head list;
} Layer;

typedef struct TileSet{
	SDL_Surface *surface;
	SDL_Rect rcSrc, rcDest;
	char *tileSetName;
	int  tileWidth, tileHeight, width, height;
	int  firstgid;
	int  numTileSet;
	struct list_head list;
} TileSet;

/**********************************************************
 *** INTERNAL PROTOTIPES
 **********************************************************/
static Layer *LayerConstructor(xmlChar *xmlchar, int numTileswidth, int numTilesheight, char *name,SDL_Surface *screen, Map *map);
static xmlChar* parseLayer(xmlDocPtr map, xmlNodePtr cur);
static void MapParseMap(Map* map, char *mapname, SDL_Surface *screen);
static TileSet* getTile(int tileNum, Map *map);
static void LayerGetSurface(Layer *layer, Map *map, SDL_Surface *screen);
static TileSet *TileSetConstructor(xmlNodePtr cur);
static void MapLoad(Map * map, SDL_Surface *screen, char *file);

/**********************************************************
 *** PRIVATE METHODS
 **********************************************************/
Layer *LayerConstructor(xmlChar *xmlchar, int numTileswidth, int numTilesheight, char *name,SDL_Surface *screen, Map *map)
{
	// Variable definition section
	Layer *this;
	int i;
	SDL_Surface *tmp; 
	
	// Alloc memory
	this = (Layer*)malloc(sizeof(Layer));

	// Create layer surface
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, map->width*map->tileWidth, 
										map->height*map->tileHeight, screen->format->BitsPerPixel, 
										screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	this->imageLayer = SDL_DisplayFormatAlpha(tmp);
/*
	Uint32 colorKey = SDL_MapRGB(this->imageLayer->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(this->imageLayer, NULL, 0xFF00FF);
	SDL_SetColorKey(this->imageLayer, SDL_SRCCOLORKEY, colorKey);
*/	
	this->name   = name;
	this->width  = numTileswidth;
	this->height = numTilesheight;
	INIT_LIST_HEAD(&this->list);
	this->csvLayer = xmlchar;

	this->data = (int **)malloc(sizeof(int*) * this->width);
	for(i = 0; i < this->width; i++)
		this->data[i] = (int *)malloc(sizeof(int) * this->height);

	return this;
}

void LayerGetSurface(Layer *layer, Map *map, SDL_Surface *screen)
{
	int i, j;
	TileSet *tileSet = NULL;
	SDL_Surface *alpha;
	SDL_Rect rcDest, rcSrc;

	alpha = loadImage("data/alpha.png");	

	// Window tiles
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w =  map->tileWidth;
	rcSrc.h = map->tileHeight;
	
	// Search the tile set 
	for(i = 0; i < layer->height; i++){
		for(j = 0; j < layer->width; j++){
			//printf("%d ", layer->data[j][i]);
			if(layer->data[j][i]){
				tileSet = getTile(layer->data[j][i], map);
				//printf("tilesetname: %s ", tileSet->tileSetName);
				tileSet->rcDest.x = j * map->tileWidth;
				tileSet->rcDest.y = i * map->tileHeight; 
				//printf("(%d, %d) ", tileSet->rcSrc.x, tileSet->rcSrc.y);
				//printf("destino(%d, %d)\n", j*32, i*32 );
				copySurface(tileSet->surface, &tileSet->rcSrc, layer->imageLayer, &tileSet->rcDest);
			}else{
				rcDest.x = j * map->tileWidth;
				rcDest.y = i * map->tileHeight;
				copySurface(alpha, &rcSrc, layer->imageLayer, &rcDest);
			}
		}
		//printf("\n");
	}
	
}

TileSet* getTile(int tileNum, Map *map)
{
	TileSet *tileset, *previus = NULL;
	int numTilesWidth;
	int column, row;
	int offset;
	
	// List tileset
	list_for_each_entry(tileset, &map->listTileSet, list){
		//printf("tileset: %s, %d\n",tileset->tileSetName, tileset->firstgid);
		if((tileNum < tileset->firstgid)){
caca:
			//printf("	I'm the tileset: %s\n",previus->tileSetName);
			numTilesWidth = (int)(previus->width/previus->tileWidth);
			offset = tileNum - previus->firstgid;
			row = (int)(floor((double)(offset/numTilesWidth)));
			column = offset%numTilesWidth;
			previus->rcSrc.x = column * previus->tileWidth;
			previus->rcSrc.y = row * previus->tileHeight;
			previus->rcSrc.w = previus->tileWidth;
			previus->rcSrc.h = previus->tileHeight;

			return previus;
		}
		previus = tileset;
	}

	goto caca;
	
	return NULL;
}

TileSet *TileSetConstructor(xmlNodePtr cur) 
{
	// Variable definition section
	TileSet *this;
	xmlNodePtr curChild;
	xmlChar* tileSetName;
	char* tileSetPath = NULL;
	//xmlChar* tileSetImage;
	xmlChar* tileWidth;
	xmlChar* tileHeight;
	xmlChar* firstgid;

	// Alloc tileset
	this = (TileSet*)malloc(sizeof(TileSet));

	INIT_LIST_HEAD(&this->list);
	
	// Paranoid check
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))) {
			
		curChild = cur->xmlChildrenNode;
		this->numTileSet++;
		printf("Loading TileSet Nº %d\n", this->numTileSet);
		tileSetName = xmlGetProp(cur, (xmlChar*)"name");
		firstgid 	= xmlGetProp(cur, (xmlChar*)"firstgid");
		tileWidth   = xmlGetProp(cur, (xmlChar*)"tilewidth");
		tileHeight  = xmlGetProp(cur, (xmlChar*)"tileheight");
			
		this->tileSetName = (char *)tileSetName;
		this->firstgid 	  = atoi((char *)firstgid);
		this->tileWidth   = atoi((char *)tileWidth);
		this->tileHeight  = atoi((char *)tileHeight);

		//xmlFree(tileSetName);
		//xmlFree(tileSetWidth);
		//xmlFree(tileSetHeight);

		printf("tileset name: %s\n", this->tileSetName);
		printf("tileset first gid: %d\n", this->firstgid);

		printf("tile width: %d\n", this->tileWidth);
		printf("tile height: %d\n", this->tileHeight);
		

		curChild = curChild->next;

		if ((!xmlStrcmp(curChild->name, (const xmlChar *)"image"))) {
			tileSetPath   = (char*)xmlGetProp(curChild, (xmlChar*)"source");
			this->width   = atoi((char*)xmlGetProp(curChild, (xmlChar*)"width"));
			this->height  = atoi((char*)xmlGetProp(curChild, (xmlChar*)"height"));
			this->surface = loadImage((char*)tileSetPath);
		}
		
		printf("____________IMAGE________________ \n");
		printf("tilesetPath: %s \n", tileSetPath);
		printf("this->width: %d \n", this->width);
		printf("this->height: %d \n", this->height);

		if(!(this->surface = loadImage(tileSetPath))){
			printf("ERROR tileSetConstructor: couldn't load %s image\n", tileSetPath);
			return NULL;
		}
	}
	
	return this;
}

xmlChar* parseLayer (xmlDocPtr map, xmlNodePtr cur)
{
	// Variable definition section
	xmlChar *key;
	xmlChar* tmp = NULL;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
			key = xmlNodeListGetString(map, cur->xmlChildrenNode, 1);
			tmp = key; 
			//printf("layer: %s\n", tmp);   
		    	//xmlFree(key);
 		}
	cur = cur->next;
	}
	return tmp;
}


void MapParseMap(Map* map, char *mapname, SDL_Surface *screen)
{
	// Variable definition section
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlChar * tmp = NULL;
	Layer *layer, *tmpLayer;
	TileSet *tileSet = NULL, *tmpTileSet = NULL; 
	
	// Load xml file
	if(!(doc = xmlParseFile(mapname))){
		fprintf(stderr,"Map not parsed successfully. \n");
		//return NULL;
	}

	// Get xml root	
	if (!(cur = xmlDocGetRootElement(doc))) {
		fprintf(stderr,"empty map\n");
		xmlFreeDoc(doc);
		//return NULL;
	}
	
	// Exist map
	if (xmlStrcmp(cur->name, (const xmlChar *) "map")) {
		fprintf(stderr,"document of the wrong type, root node != map\n");
		xmlFreeDoc(doc);
		//return NULL;
	}
	
	map->width = atoi((char*)xmlGetProp(cur, (xmlChar*)"width"));
	map->height = atoi((char*)xmlGetProp(cur, (xmlChar*)"height"));
	map->tileWidth = atoi((char*)xmlGetProp(cur, (xmlChar*)"tilewidth"));
	map->tileHeight = atoi((char*)xmlGetProp(cur, (xmlChar*)"tileheight"));
	
	printf("W: %d, H: %d, tW: %d, tH: %d\n", map->width, map->height, map->tileWidth, map->tileHeight);
		
	cur = cur->xmlChildrenNode;
	
	// Create layer list
	while (cur) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))){
			printf("dentro de tileset, cur->name: %s\n", (char *)cur->name);
			tileSet = TileSetConstructor(cur);
			list_add_tail(&tileSet->list, &map->listTileSet);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))){
			tmp = parseLayer (doc, cur);
			layer = LayerConstructor(tmp,atoi((char*)xmlGetProp(cur, (xmlChar*)"width")),atoi((char*)xmlGetProp(cur, (xmlChar*)"height")), (char*)xmlGetProp(cur, (xmlChar*)"name"), screen, map);
			list_add_tail(&layer->list, &map->listLayer);
			//printf("%s", tmp);
		}
	
	cur = cur->next;
	}
	
	list_for_each_entry(tmpLayer, &map->listLayer, list){
		printf("%s\n", (char *)tmpLayer->csvLayer);
		printf("*************************************************************************************************************\n");
	}

	printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");

	list_for_each_entry(tmpTileSet, &map->listTileSet, list){
		printf("%s\n", (char *)tmpTileSet->tileSetName);
		printf("--------------------------------------------------------------------------------------------------------------\n");
	}

	map->layer = tmp;
	
	//xmlFreeDoc(tmp);
}

/**********************************************************
 *** PUBLIC METHODS
 **********************************************************/
Map* MapConstructor(SDL_Surface *screen, char *file)
{
	// Variable definition section
	Map * map;
	SDL_Surface *temp, *alpha;
	//SDL_Rect rcSrc, rcDest;
	//int i, j;
	
	// Alloc map
	map = (Map *)malloc(sizeof(Map));
	
	// For fill front surface with alpha
	if(!(alpha = loadImage("data/alpha.png"))){
		printf("MAP CONSTRUCTOR FAIL: impossible load data/alpha.png file\n");
		return NULL;
	}		
	
	// Inits
	map->numTileSet = 0;
	map->numLayers = 0;
	map->scroll_x = 0;
	map->scroll_y = 0;
	map->scrollVel = 32;

	// Init Layer
	INIT_LIST_HEAD(&map->listLayer);
	
	// Init tileset
	INIT_LIST_HEAD(&map->listTileSet);

	// Fill xml layer data
	MapParseMap(map, file, screen);
/*
	rcDest.x = 0;
	rcDest.y = 0;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w =  map->tileWidth;
	rcSrc.h = map->tileHeight;
*/
	// Create two main layers: back, front
	
	//temp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, , (height+1)*16, 32, 0, 0, 0, 0x000000FF);
	
		

	temp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, map->width * map->tileWidth, map->height * map->tileHeight, 
											screen->format->BitsPerPixel,
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											screen->format->Amask);

	map->surfaceBack = SDL_DisplayFormatAlpha(temp);

/*	
	Uint32 colorKey = SDL_MapRGB(map->surfaceBack->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(map->surfaceBack, NULL, 0xFF00FF);
	SDL_SetColorKey(map->surfaceBack, SDL_SRCCOLORKEY, colorKey);
*/
	temp = SDL_CreateRGBSurface(SDL_SWSURFACE, map->width * map->tileWidth, map->height * map->tileHeight, 
											screen->format->BitsPerPixel,
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											screen->format->Amask);

	map->surfaceFront = SDL_DisplayFormatAlpha(temp);
/*
	// Fill with alpha
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			rcDest.x = j * map->tileWidth;
			rcDest.y = i * map->tileHeight; 
			copySurface(alpha, &rcSrc, 	map->surfaceFront, &rcDest);
		}
	}
*/

/*
	Uint32 colorKey2 = SDL_MapRGB(map->surfaceFront->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(map->surfaceFront, NULL, 0xFF00FF);
	SDL_SetColorKey(map->surfaceFront, SDL_SRCCOLORKEY, colorKey2);
*/
	// Create refresh layer
	temp = SDL_CreateRGBSurface(SDL_SWSURFACE, map->width * map->tileWidth, map->height * map->tileHeight, 
											screen->format->BitsPerPixel, 
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											screen->format->Amask);
	
	map->surfaceRefresh = SDL_DisplayFormatAlpha(temp);

/*
	Uint32 colorKey3 = SDL_MapRGB(map->surfaceRefresh->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(map->surfaceRefresh, NULL, 0xFF00FF);
	SDL_SetColorKey(map->surfaceRefresh, SDL_SRCCOLORKEY, colorKey3);
*/
	// Map load
	MapLoad(map, screen, file);
 	
	return map;
}


void MapLoad(Map * map, SDL_Surface *screen, char *file)
{
	// variable definition section
	Layer *tmpLayer;
	SDL_Rect rcDest, rcSrc, rcSrcAlpha, rcDestAlpha;
	SDL_Surface *temp, *auxFront, *alpha;

	char *str1, *str2, *token, *subtoken;
    
	char *saveptr1, *saveptr2;
	char nameLayer[50], pos[10];

	char *rowsDelimiter = "\n";
	char *colsDelimiter = ",";

	int i;
	int j;
	
	printf("W: %d, H: %d", map->width, map->height);
	
	rcDest.x = 0;
	rcDest.y = 0;

	rcDestAlpha.x = 0;
	rcDestAlpha.y = 0;
	
	rcSrc.x = 0;	
	rcSrc.y = 0;
	rcSrc.w = map->width * map->tileWidth;
	rcSrc.h = map->height * map->tileHeight;

	rcSrcAlpha.x = 0;	
	rcSrcAlpha.y = 0;
	rcSrcAlpha.w = map->tileWidth;
	rcSrcAlpha.h = map->tileHeight;
	
	// For fill front surface with alpha
	alpha = loadImage("data/alpha.png");

	temp = SDL_CreateRGBSurface(SDL_SWSURFACE, map->width * map->tileWidth, map->height * map->tileHeight, 
									screen->format->BitsPerPixel,
									screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
									screen->format->Amask);
	auxFront = SDL_DisplayFormatAlpha(temp);

	//auxFront = SDL_SetVideoMode(map->width * map->tileWidth,  map->height * map->tileHeight, 0, 0);

	// Fill with alpha

	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){
			rcDestAlpha.x = j * map->tileWidth;
			rcDestAlpha.y = i * map->tileHeight; 
			copySurface(alpha, &rcSrcAlpha, auxFront, &rcDestAlpha);
		}
	}

	// Init char position
	map->charPosition = (int **)malloc(sizeof(int*) * map->height);
	for(i = 0; i < map->height; i++)
		map->charPosition[i] = (int *)malloc(sizeof(int) * map->width);
	
	for(i = 0; i < map->height; i++)
		for(j = 0; j < map->width; j++)
			map->charPosition[i][j] = 0;
	
	map->collisions = (int **)malloc(sizeof(int*) * map->height);
	for(i = 0; i < map->height; i++)
		map->collisions[i] = (int *)malloc(sizeof(int) * map->width);
	
	// for all list parse xmlChars  
	list_for_each_entry(tmpLayer, &map->listLayer, list){

		for (i = 0, str1 = (char *)tmpLayer->csvLayer; ; i++, str1 = NULL) {
			token = strtok_r(str1, rowsDelimiter, &saveptr1);
			if (token == NULL)
				break;

			for (j = 0, str2 = token; ; j++, str2 = NULL) {
				subtoken = strtok_r(str2, colsDelimiter, &saveptr2);
				if (subtoken == NULL)
					break;
                		//printf("|\t%s.bmp\t|", subtoken);
	 	    		tmpLayer->data[j][i] = atoi(subtoken);
				//printf("DATA1(%d ,%d) = %d\n", j, i, tmpLayer->data[j][i]);
				//tmpLayer->surfaceLayer[j][i] = loadImage(bgTileName);
			}
		}
		
		printf("\n********************layer: %s*********************\n", tmpLayer->name);
		
		// Save data collisions in map
		if(!strcmp(tmpLayer->name, "Colisiones")){
			for(i=0; i < map->height; i++){
				for(j=0; j < map->width; j++){
					map->collisions[i][j] = tmpLayer->data[j][i];
					printf("%d ", map->collisions[i][j]);
				}
			printf("\n");
			}
		}
		LayerGetSurface(tmpLayer, map, screen);
	}
	
	// Load main layers
	list_for_each_entry(tmpLayer, &map->listLayer, list){
		sscanf(tmpLayer->name, "%s %s", nameLayer, pos);
		
		if(!strcmp(pos, "front")){
			printf("NAME LAYER FRONT: %s\n", nameLayer);
			overSurface(tmpLayer->imageLayer, &rcSrc, auxFront, &rcDest, map->width * map->tileHeight);		
		}else{
			printf("NAME LAYER BACK: %s\n", nameLayer);
			SDL_BlitSurface(tmpLayer->imageLayer, &rcSrc, map->surfaceBack, &rcDest);			
			SDL_BlitSurface(tmpLayer->imageLayer, &rcSrc, map->surfaceRefresh, &rcDest);
		}
	}
	copySurface(auxFront, &rcSrc, map->surfaceFront, &rcDest);			
}

void MapUpdate(Map * map, SDL_Rect cursorCoords)
{
	int x;
	int y;

	x = cursorCoords.x;
	y = cursorCoords.y;

	
	//printf("SCROLL X = %d || SCROLL Y = %d\n", map->scroll_x, map->scroll_y);
/*	
	// scroll diagonal superior izq.
	if ((x <= 64) && (y <= 64)){
		if (map->scroll_x <= BORDER) // scroll limit
			map->scroll_x += map->scrollVel;
		if (map->scroll_y <= 64) // scroll limit
			map->scroll_y += map->scrollVel;
	}
	// scroll diagonal superior der.
	else if ((y <= 64) && (x >= 650)){
		if (map->scroll_x >= -550) // scroll limit
			map->scroll_x -= map->scrollVel;
		if (map->scroll_y <= 64) // scroll limit
			map->scroll_y += map->scrollVel;
	}
	// scroll diagonal inferior izq.
	else if ((x <= 64) && (y >= 500)){
		if (map->scroll_x <= 64) // scroll limit
			map->scroll_x += map->scrollVel;
		if (map->scroll_y >= -750) // scroll limit
			map->scroll_y -= map->scrollVel;
	}
	// scroll diagonal inferior der.
	else if ((y >= 500) && (x >= 650)){
		if (map->scroll_x >= -550)
			map->scroll_x -= map->scrollVel;
		if (map->scroll_y >= -750)
			map->scroll_y -= map->scrollVel;
	}
*/
	// scroll izq.
	if (x <= 64+30 - map->scroll_x) {
		if (map->scroll_x <= BORDER) // scroll limit
			map->scroll_x += map->scrollVel;
	}
	// scroll der.
	else if (x >= 650+30 - map->scroll_x){
		if (map->scroll_x >= -((map->width*map->tileWidth) - SCREEN_WIDTH + BORDER)) // scroll limit
			map->scroll_x -= map->scrollVel;
	}
	// scroll up
	else if (y <= 64 - map->scroll_y){
		if (map->scroll_y <= BORDER )// scroll limit
			map->scroll_y += map->scrollVel;
	}
	//scroll down
	else if (y >= 500 - map->scroll_y){
		if (map->scroll_y >= -((map->height * map->tileHeight) - SCREEN_HEIGHT + BORDER)) // scroll limit
			map->scroll_y -= map->scrollVel;
	}
}

void MapDraw(Map *map, SDL_Surface* screen)
{
	//Layer *tmpLayer;
	SDL_Rect rcBack;
/*
	SDL_Rect Front, rcBack;
	SDL_Rect rcSrc, rcSrc2, rcDest;

	rcDest.x = 0;
	rcDest.y = 0;
	
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = map->width * map->tileWidth;
	rcSrc.h = map->height * map->tileHeight;

	rcSrc2.x = 0;
	rcSrc2.y = 0;
	rcSrc2.w = map->width * map->tileWidth;
	rcSrc2.h = map->height * map->tileHeight;


	rcDest.x = 650;
	rcDest.y = -1000;

	rcFront.x = map->scroll_x;
	rcFront.y = map->scroll_y;
*/

	rcBack.x = map->scroll_x;
	rcBack.y = map->scroll_y;

	//SDL_BlitSurface(map->surfaceFront, NULL, screen, &rcDest);
	SDL_BlitSurface(map->surfaceFront, NULL, map->surfaceBack, NULL);
	SDL_BlitSurface(map->surfaceBack, NULL, screen, &rcBack);
	//SDL_BlitSurface(map->surfaceFront, NULL, screen, &rcFront);
	
	SDL_BlitSurface(map->surfaceRefresh, NULL, map->surfaceBack, NULL);
}

void MapDestructor(Map * map)
{
	//SDL_FreeSurface(map->tileSet);
	//xmlFree(map->layer);
}
