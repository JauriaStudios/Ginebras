#include "gameUtilities.h"

void copySurface (SDL_Surface *source, SDL_Rect *src,
                       SDL_Surface *destiny, SDL_Rect *dest)
{
    int i;
    int s_offset, d_offset;
    Uint8 *s_point, *d_point;

    SDL_LockSurface(source);
    SDL_LockSurface(destiny);

    s_point = (Uint8 *) source->pixels;
    d_point = (Uint8 *) destiny->pixels;

    for (i = 0; i < src->h; i++){	// Contador de fila

       s_offset=(((src->y)+i)*(source->pitch)+
                 ((src->x)*(source->format->BytesPerPixel)));
       d_offset=(((dest->y)+i)*(destiny->pitch)+
                 ((dest->x)*(source->format->BytesPerPixel)));

		memcpy ( & d_point[ d_offset], & s_point[s_offset], (src->w)*(source->format->BytesPerPixel));

    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(destiny);

}

void overSurface (SDL_Surface *source, SDL_Rect *src,
                       SDL_Surface *destiny, SDL_Rect *dest, int mapWidth)
{
    unsigned int i, j;
    int s_offset, d_offset;
    Uint8 *s_point, *d_point;

    SDL_LockSurface(source);
    SDL_LockSurface(destiny);

    s_point = (Uint8 *) source->pixels;
    d_point = (Uint8 *) destiny->pixels;

    for (i = 0; i < src->h; i++){	// Contador de fila

       s_offset=(((src->y)+i)*(source->pitch)+
                 ((src->x)*(source->format->BytesPerPixel)));
       d_offset=(((dest->y)+i)*(destiny->pitch)+
                 ((dest->x)*(source->format->BytesPerPixel)));
		
		for(j = 0; j < mapWidth*(source->format->BytesPerPixel); j++){
			//printf("%d!%d ", d_point[j], j);
			if(s_point[s_offset+j])
				memcpy ( & d_point[d_offset+j], & s_point[s_offset+j], (source->format->BytesPerPixel));
		}

    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(destiny);

}


SDL_Surface* loadImage(char* filename)
{
    SDL_Surface* temp = NULL;
    SDL_Surface* image = NULL;
	
	if(!filename)
		return NULL;
	 
    //cargar imagen temporal
    temp = IMG_Load(filename);
    if(!temp) 
		return NULL;
 
    //cambiar formato y liberar imagen temp
    image = SDL_DisplayFormatAlpha(temp);
	//image = SDL_DisplayFormat(temp);

    SDL_FreeSurface(temp);
    if(!image) 
		return NULL;
 
    //elegimos como color el 'rosa magico' y lo hacemos color clave
    //Uint32 colorKey = SDL_MapRGB(image->format, 0xFF, 0, 0xFF);
    //SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
 
    //devolver imagen final
    return image;
}

void GetCoor(int pixelX, int pixelY, int *coordX, int *coordY)
{
	*coordX =  (int)(floor((double)(pixelX/TILE_SIZE)));
	*coordY =  (int)(floor((double)(pixelY/TILE_SIZE)));
}


