#include "gameUtilities.h"

SDL_Surface* loadImage(char* filename)
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
