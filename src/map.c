#include "map.h"

Map* MapConstructor()
{
	Map * map;
	map = (Map *)malloc(sizeof(Map));
	
	map -> rcGrassDest.x = 0;
	map -> rcGrassDest.y = 0;
	
	map -> rcGrassSrc.x = 0;
	map -> rcGrassSrc.y = 0;
	map -> rcGrassSrc.w = 32;
	map -> rcGrassSrc.h = 32;

	map -> scroll_x = 0;
	map -> scroll_y = 0;

	map -> scrollVel = 5;

	map -> mapWidth = MAP_SIZE_X * TILE_SIZE + SCREEN_WIDTH;
	map -> mapHeight = MAP_SIZE_Y * TILE_SIZE + SCREEN_HEIGHT;

	map -> layer = NULL;

	return map;
}

void MapLoad(Map * map, char* file)
{
	map -> layer = parseMap("data/map.tmx");
	
	map -> tileSet = loadImage2lol("data/mountain_landscape_19.png");
	
	int x = 0;
	int y = 0;
	
	char rowsDelimiter[]="\n";
	char colsDelimiter[]=",";
	
	char *tileRows = strtok((char*)map->layer, rowsDelimiter);
	
	char *lines[1024]; 
	
	for (x = 0; x < MAP_SIZE_X; x++) {
		lines[x] = tileRows;
		//printf("%s\n",lines[x]);
		tileRows = strtok(NULL, rowsDelimiter);
	}
	
	for (x = 0; x < MAP_SIZE_X; x++) {
		char *tileCols = strtok(lines[x], colsDelimiter);
		for (y = 0; y < MAP_SIZE_Y; y++) {
			
			char bgTileName[20];
			
			sprintf(bgTileName, "data/%s.bmp",tileCols);
	 	        map -> background[y][x]= atoi(tileCols);
			map -> surfaceBackground[y][x] = loadImage2lol(bgTileName);
			tileCols = strtok(NULL, colsDelimiter);
		}
	}
}


void MapUpdate(Map * map,SDL_Rect cursorCoords)
{
	int x = 0;
	int y = 0;

	x = cursorCoords.x;
	y = cursorCoords.y;


	if ((x <= 64) && (y <= 64)){
		map -> scroll_x += map -> scrollVel;
		map -> scroll_y += map -> scrollVel;
	}
	else if ((x <= 64) && (y >= 500)){
		map -> scroll_x += map -> scrollVel;
		map -> scroll_y -= map -> scrollVel;
	}
	else if ((y >= 500) && (x >= 650)){
		map -> scroll_x -= map -> scrollVel;
		map -> scroll_y -= map -> scrollVel;
	}
	else if ((y <= 64) && (x >= 650)){
		map -> scroll_x -= map -> scrollVel;
		map -> scroll_y += map -> scrollVel;
	}
	else if (x <= 64){
		map -> scroll_x += map -> scrollVel;
	}
	else if (x >= 650){
		map -> scroll_x -= map -> scrollVel;
	}
	else if (y <= 64){
		map -> scroll_y += map -> scrollVel;
	}
	else if (y >= 500){
		map -> scroll_y -= map -> scrollVel;
	}
	else{}

}

void MapDraw(Map * map, SDL_Surface* screen)
{
	int x = 0;
	int y = 0;

	for (x = 0; x < MAP_SIZE_X; x++) {
		for (y = 0; y < MAP_SIZE_Y; y++) {
			map -> rcGrassDest.x = x * TILE_SIZE + map -> scroll_x;
			map -> rcGrassDest.y = y * TILE_SIZE + map -> scroll_y;
			SDL_BlitSurface(map->surfaceBackground[x][y], &map -> rcGrassSrc, screen, &map -> rcGrassDest);
			
		}
	}
}

void MapClean(Map * map)
{
	int x = 0;
	int y = 0;

	for (x = 0; x < MAP_SIZE_X; x++) {
		for (y = 0; y < MAP_SIZE_Y; y++) {
			SDL_FreeSurface(map->surfaceBackground[x][y]);
		}
	}
	SDL_FreeSurface(map->tileSet);
	xmlFree(map -> layer);
}

SDL_Surface* loadImage2lol(char* filename)
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

xmlChar* parseLayer (xmlDocPtr map, xmlNodePtr cur)
{
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

xmlChar* parseMap(char *mapname)
{

	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlChar * tmp = NULL;

	doc = xmlParseFile(mapname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Map not parsed successfully. \n");
		return NULL;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"empty map\n");
		xmlFreeDoc(doc);
		return NULL;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "map")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return NULL;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))){
			tmp = parseLayer (doc, cur);
			//printf("%s", tmp);
		}
	
	cur = cur->next;
	}
	
	//xmlFreeDoc(tmp);
	return tmp;
}
