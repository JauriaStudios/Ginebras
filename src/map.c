#include <math.h>
#include "map.h"

static xmlChar* parseLayer(xmlDocPtr map, xmlNodePtr cur);
//static void MapGetReference(Map * map, xmlDocPtr map, xmlNodePtr cur);
static void MapParseMap(Map* map, char *mapname, SDL_Surface *screen);

typedef struct Layer{
	char *name;
	xmlChar *csvLayer;
	SDL_Surface* surfaceLayer[MAP_SIZE_X][MAP_SIZE_Y];
	SDL_Surface* imageLayer;
	int data[MAP_SIZE_X][MAP_SIZE_Y];
	SDL_Rect rcDest;
	int width, height;
	struct list_head list;
} Layer;

static Layer *LayerConstructor(xmlChar *xmlchar, int numTileswidth, int numTilesheight, char *name,SDL_Surface *screen)
{
	// Variable definition section
	Layer *this; 
	
	// Alloc memory
	this = (Layer*)malloc(sizeof(Layer));

	// Create layer surface
	this->imageLayer = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 40*32, 40*32, screen->format->BitsPerPixel,
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											0x00000000);
	
	Uint32 colorKey = SDL_MapRGB(this->imageLayer->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(this->imageLayer, NULL, 0xFF00FF);
	SDL_SetColorKey(this->imageLayer, SDL_SRCCOLORKEY, colorKey);

	this->name   = name;
	this->width  = numTileswidth;
	this->height = numTilesheight;
	INIT_LIST_HEAD(&this->list);
	this->csvLayer = xmlchar;

	return this;
}

typedef struct TileSet{
	SDL_Surface *surface;
	SDL_Rect rcSrc, rcDest;
	char *tileSetName;
	int  tileWidth, tileHeight, width, height;
	int  firstgid;
	int  numTileSet;
	struct list_head list;
} TileSet;


Map* MapConstructor(SDL_Surface *screen)
{
	// Variable definition section
	Map * map;
	int i, j;

	// Alloc map
	map = (Map *)malloc(sizeof(Map));

	// Create two main layers: back, front 
	map->surfaceBack = SDL_CreateRGBSurface(SDL_SWSURFACE,40*32,40*32, screen->format->BitsPerPixel,
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											screen->format->Amask);
	
	Uint32 colorKey = SDL_MapRGB(map->surfaceBack->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(map->surfaceBack, NULL, 0xFF00FF);
	SDL_SetColorKey(map->surfaceBack, SDL_SRCCOLORKEY, colorKey);

	map->surfaceFront = SDL_CreateRGBSurface(SDL_SWSURFACE,40*32,40*32, screen->format->BitsPerPixel,
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											screen->format->Amask);

	Uint32 colorKey2 = SDL_MapRGB(map->surfaceFront->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(map->surfaceFront, NULL, 0xFF00FF);
	SDL_SetColorKey(map->surfaceFront, SDL_SRCCOLORKEY, colorKey2);

	// Create refresh layer
	map->surfaceRefresh = SDL_CreateRGBSurface(SDL_SWSURFACE,40*32,40*32, screen->format->BitsPerPixel,
											screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
											screen->format->Amask);
	
	Uint32 colorKey3 = SDL_MapRGB(map->surfaceRefresh->format, 0xFF, 0x0, 0xFF);
	SDL_FillRect(map->surfaceRefresh, NULL, 0xFF00FF);
	SDL_SetColorKey(map->surfaceRefresh, SDL_SRCCOLORKEY, colorKey3);

	// set initial coord
	map->rcGrassDest.x = 0;
	map->rcGrassDest.y = 0;
	
	map->rcGrassSrc.x = 0;
	map->rcGrassSrc.y = 0;
	map->rcGrassSrc.w = 32;
	map->rcGrassSrc.h = 32;

	// Inits
	map->numTileSet = 0;
	map->numLayers = 0;
	map->scroll_x = 0;
	map->scroll_y = 0;
	map->scrollVel = 32;
	map->mapWidth = MAP_SIZE_X * TILE_SIZE + SCREEN_WIDTH;
	map->mapHeight = MAP_SIZE_Y * TILE_SIZE + SCREEN_HEIGHT;

	// Init Layer
	INIT_LIST_HEAD(&map->listLayer);
	map->layer = NULL;

	// Init tileset
	INIT_LIST_HEAD(&map->listTileSet);

	// Init char position
	map->charPosition = (int **)malloc(sizeof(int*) * MAP_SIZE_X);
	for(i = 0; i < MAP_SIZE_X; i++)
		map->charPosition[i] = (int *)malloc(sizeof(int) * MAP_SIZE_Y);

	for(i = 0; i < MAP_SIZE_X; i++)
		for(j = 0; j < MAP_SIZE_Y; j++)
			map->charPosition[i][j] = 0;

	return map;
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

void LayerGetSurface(Layer *layer, Map *map, SDL_Surface *screen)
{
	int i, j;
	TileSet *tileSet;

	// Search the tile set 
	for(i = 0; i < layer->height; i++){
		for(j = 0; j < layer->width; j++){
			//printf("%d ", layer->data[j][i]);
			if(layer->data[j][i]){
				tileSet = getTile(layer->data[j][i], map);
				//printf("tilesetname: %s ", tileSet->tileSetName);
				tileSet->rcDest.x = j * 32;
				tileSet->rcDest.y = i * 32; 
				//printf("(%d, %d) ", tileSet->rcSrc.x, tileSet->rcSrc.y);
				//printf("destino(%d, %d)\n", j*32, i*32 );
				SDL_BlitSurface(tileSet->surface, &tileSet->rcSrc, layer->imageLayer, &tileSet->rcDest);
			}else{
				
			}
		}
		//printf("\n");
	}
	
}

void MapLoad(Map * map, char* file, SDL_Surface *screen)
{
	// variable definition section
	Layer *tmpLayer;
	// fills xml layer data
	MapParseMap(map, file, screen);
	
	// parse csv data
	char *str1, *str2, *token, *subtoken;
    char *saveptr1, *saveptr2;
	char nameLayer[50], pos[10];

	char *rowsDelimiter = "\n";
	char *colsDelimiter = ",";

    int i;
	int j;
	
	//SDL_Rect rcSrc, rcDest;
	
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
				char bgTileName[20];
				sprintf(bgTileName, "data/%s.bmp",subtoken);
	 	    	tmpLayer->data[j][i]= atoi(subtoken);
				//printf("DATA1(%d ,%d) = %d\n", j, i, tmpLayer->data[j][i]);
				//tmpLayer->surfaceLayer[j][i] = loadImage(bgTileName);
			}
		}
		printf("\n********************layer: %s*********************\n", tmpLayer->name);
		
		// Save data colisions in map
		if(!strcmp(tmpLayer->name, "Colisiones")){
			for(i=0; i<40; i++){
				for(j=0; j<40; j++){
					map->colisions[i][j] = tmpLayer->data[j][i];
					printf("%d ", map->colisions[i][j]);
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
			SDL_BlitSurface(tmpLayer->imageLayer, NULL, map->surfaceFront, NULL);		
		}else{
			printf("NAME LAYER BACK: %s\n", nameLayer);
			SDL_BlitSurface(tmpLayer->imageLayer, NULL, map->surfaceBack, NULL);			
			SDL_BlitSurface(tmpLayer->imageLayer, NULL, map->surfaceRefresh, NULL);
		}

	}
	
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
		if (map->scroll_x <= 64) // scroll limit
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
		if (map->scroll_x <= 64) // scroll limit
			map->scroll_x += map->scrollVel;
	}
	// scroll der.
	else if (x >= 650+30 - map->scroll_x){
		if (map->scroll_x >= -550) // scroll limit
			map->scroll_x -= map->scrollVel;
	}
	// scroll up
	else if (y <= 64 - map->scroll_y){
		if (map->scroll_y <= 64 )// scroll limit
			map->scroll_y += map->scrollVel;
	}
	//scroll down
	else if (y >= 500 - map->scroll_y){
		if (map->scroll_y >= -750) // scroll limit
			map->scroll_y -= map->scrollVel;
	}
	else{}

}

void MapDraw(Map *map, SDL_Surface* screen)
{
	//Layer *tmpLayer;
	SDL_Rect rcFront, rcBack;
	//SDL_Rect rcSrc;

	// Set initial animation sprite frame
/*	
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = SCREEN_WIDTH;
	rcSrc.h = SCREEN_HEIGHT;
*/	

	rcBack.x = map->scroll_x;
	rcBack.y = map->scroll_y;
	rcFront.x = map->scroll_x;
	rcFront.y = map->scroll_y;
/*
	rcDest.x = 650;
	rcDest.y = -1000;
*/

	//SDL_BlitSurface(map->surfaceFront, NULL, screen, &rcDest);
	SDL_BlitSurface(map->surfaceBack, NULL, screen, &rcBack);
	SDL_BlitSurface(map->surfaceFront, NULL, screen, &rcFront);
	
	SDL_BlitSurface(map->surfaceRefresh, NULL, map->surfaceBack, NULL);
}

void MapDestructor(Map * map)
{
	//SDL_FreeSurface(map->tileSet);
	//xmlFree(map->layer);
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

	cur = cur->xmlChildrenNode;
	
	// Create layer list
	while (cur) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))){
			printf("destro de tileset, cur->name: %s\n", (char *)cur->name);
			tileSet = TileSetConstructor(cur);
			list_add_tail(&tileSet->list, &map->listTileSet);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))){
			tmp = parseLayer (doc, cur);
			layer = LayerConstructor(tmp,atoi((char*)xmlGetProp(cur, (xmlChar*)"width")),atoi((char*)xmlGetProp(cur, (xmlChar*)"height")), (char*)xmlGetProp(cur, (xmlChar*)"name"), screen);
			list_add_tail(&layer->list, &map->listLayer);
			//printf("%s", tmp);
		}
	
	cur = cur->next;
	}
	
	list_for_each_entry(tmpLayer, &map->listLayer, list){
		printf("%s\n", (char *)tmpLayer->csvLayer);
		printf("**************************************************************************************************************************\n");
	}

	printf("///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");

	list_for_each_entry(tmpTileSet, &map->listTileSet, list){
		printf("%s\n", (char *)tmpTileSet->tileSetName);
		printf("-------------------------------------------------------------------------------------------------------------------------\n");
	}

	map->layer = tmp;
	
	//xmlFreeDoc(tmp);
}

