#include "map.h"

static xmlChar* parseLayer(xmlDocPtr map, xmlNodePtr cur);
//static void MapGetReference(Map * map, xmlDocPtr map, xmlNodePtr cur);
void MapParseMap(Map* map, char *mapname);


Map* MapConstructor()
{
	Map * map;
	map = (Map *)malloc(sizeof(Map));
	
	map->rcGrassDest.x = 0;
	map->rcGrassDest.y = 0;
	
	map->rcGrassSrc.x = 0;
	map->rcGrassSrc.y = 0;
	map->rcGrassSrc.w = 32;
	map->rcGrassSrc.h = 32;

	map->numTileSet = 0;
	
	map->numLayers = 0;
	map->scroll_x = 0;
	map->scroll_y = 0;

	map->scrollVel = 5;

	map->mapWidth = MAP_SIZE_X * TILE_SIZE + SCREEN_WIDTH;
	map->mapHeight = MAP_SIZE_Y * TILE_SIZE + SCREEN_HEIGHT;

	map->layer = NULL;

	return map;
}

void MapLoad(Map * map, char* file)
{
	MapParseMap(map, file);
	
	map->tileSet = loadImage("data/mountain_landscape_19.png");


	char *str1, *str2, *token, *subtoken;
        char *saveptr1, *saveptr2;

	char *rowsDelimiter = "\n";
	char *colsDelimiter = ",";

        int i;
	int j;

        for (i = 0, str1 = (char *)map->layer; ; i++, str1 = NULL) {
		token = strtok_r(str1, rowsDelimiter, &saveptr1);
                if (token == NULL)
                	break;
		//printf("Load Row %d\n",i);
              	for (j = 0, str2 = token; ; j++, str2 = NULL) {
                	subtoken = strtok_r(str2, colsDelimiter, &saveptr2);
                	if (subtoken == NULL)
                       		break;

                   	//printf("|\t%s.bmp\t|", subtoken);

			char bgTileName[20];
			
			sprintf(bgTileName, "data/%s.bmp",subtoken);
	 	    	map -> background[j][i]= atoi(subtoken);
			map -> surfaceBackground[j][i] = loadImage(bgTileName);

               	}
        }
}


void MapUpdate(Map * map, SDL_Rect cursorCoords)
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

void MapDestructor(Map * map)
{
	int x = 0;
	int y = 0;

	for (x = 0; x < MAP_SIZE_X; x++) {
		for (y = 0; y < MAP_SIZE_Y; y++) {
			SDL_FreeSurface(map->surfaceBackground[x][y]);
		}
	}
	SDL_FreeSurface(map->tileSet);
	xmlFree(map->layer);
}

xmlChar* parseLayer (xmlDocPtr map, xmlNodePtr cur)
{
//	printf("# Parse Layer\n");

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

void MapGetReference (Map * map, xmlDocPtr doc, xmlNodePtr cur) 
{
//	printf("# Get Reference\n");
	
	// tileset
	xmlChar* tileSetName;
	//xmlChar* tileSetImage;
	xmlChar* tileSetWidth;
	xmlChar* tileSetHeight;
	xmlChar* firstgid;

	
	//cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))) {
			map->numTileSet += 1;
			printf("Loading TileSet Nº %d\n", map->numTileSet);
			tileSetName = xmlGetProp(cur, (xmlChar*)"name");
			firstgid = xmlGetProp(cur, (xmlChar*)"firstgid");
			tileSetWidth = xmlGetProp(cur, (xmlChar*)"tilewidth");
			tileSetHeight = xmlGetProp(cur, (xmlChar*)"tileheight");
			
			map->tileSetName = (char *)tileSetName;
			map->firstgid = (char *)firstgid;
			map->tileSetWidth = (char *)tileSetWidth;
			map->tileSetHeight = (char *)tileSetHeight;

			//xmlFree(tileSetName);
			//xmlFree(tileSetWidth);
			//xmlFree(tileSetHeight);

			printf("tileset name: %s\n", map->tileSetName);
			printf("tileset first gid: %s\n", map->firstgid);

			printf("tileset width: %s\n", map->tileSetWidth);
			printf("tileset height: %s\n", map->tileSetHeight);

		}
		cur = cur->next;
	}
}

void MapParseMap(Map* map, char *mapname)
{
//	printf("# Parse Map\n");
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlChar * tmp = NULL;

	doc = xmlParseFile(mapname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Map not parsed successfully. \n");
		//return NULL;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"empty map\n");
		xmlFreeDoc(doc);
		//return NULL;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "map")) {
		fprintf(stderr,"document of the wrong type, root node != map");
		xmlFreeDoc(doc);
		//return NULL;
	}
	

	
	//getReference(doc, cur);
	cur = cur->xmlChildrenNode;

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))){
			MapGetReference(map, doc, cur);
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))){
			tmp = parseLayer (doc, cur);
			//printf("%s", tmp);
		}
	
	cur = cur->next;
	}
	map->layer = tmp;
	
	//xmlFreeDoc(tmp);
}
