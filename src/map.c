

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

	return map;
}

void MapLoad(Map * map, char* file)
{
	char lineRead[1024];

	parseMap("data/map.tmx");
	
	map -> pInput = fopen( file, "r");

	int x = 0;
	int y = 0;

	while(fgets(lineRead, sizeof(lineRead), map -> pInput) != NULL)
	{
		char *tok = strtok(lineRead, ",");
		int max = 40;
		for(x = 0; x < max;x++){
			char bgTileName[20];
			
			sprintf(bgTileName, "data/%d.bmp",atoi(tok));
	 	        map -> background[x][y]= atoi(tok);
			map -> surfaceBackground[x][y] = loadImage2lol(bgTileName);
			tok = strtok(NULL, ",");
		}
		y++;
	}
	fclose (map -> pInput);
}


void MapUpdate(Map * map)
{
	printf("lol");
}

void MapDraw(Map * map, SDL_Surface* screen)
{
	int x = 0;
	int y = 0;

	for (x = 0; x < 40; x++) { //NUM_TILES_X
		for (y = 0; y < 40; y++) { //NUM_TILES_Y
			map -> rcGrassDest.x = x * 32;
			map -> rcGrassDest.y = y * 32;
			SDL_BlitSurface(map->surfaceBackground[x][y], &map->rcGrassSrc, screen, &map -> rcGrassDest);
			
		}
	}
}

void MapClean(Map * map)
{
	int x = 0;
	int y = 0;

	for (x = 0; x < 40; x++) { //NUM_TILES_X
		for (y = 0; y < 40; y++) { //NUM_TILES_Y
			SDL_FreeSurface(map->surfaceBackground[x][y]);
		}
	}
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

void parseLayer (xmlDocPtr map, xmlNodePtr cur)
{

	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
		    key = xmlNodeListGetString(map, cur->xmlChildrenNode, 1);
		    printf("layer: %s\n", key);
		    xmlFree(key);
 	    }
	cur = cur->next;
	}
    return;
}

void parseMap(char *mapname)
{

	xmlDocPtr map;
	xmlNodePtr cur;

	map = xmlParseFile(mapname);
	
	if (map == NULL ) {
		fprintf(stderr,"Map not parsed successfully. \n");
		return;
	}
	
	cur = xmlDocGetRootElement(map);
	
	if (cur == NULL) {
		fprintf(stderr,"empty map\n");
		xmlFreeDoc(map);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "map")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(map);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))){
			parseLayer (map, cur);
		}
		 
	cur = cur->next;
	}
	
	xmlFreeDoc(map);
	return;
}
