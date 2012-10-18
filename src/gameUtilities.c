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

#include "gameUtilities.h"

/*
 * Las funciones getpixel y putpixel proporcionadas por la
 * documentacion de SDL. Las vamos a usar para trabajar
 * directamente sobre las superficies
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;
    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

/*
 * Descripcion: Realiza un fundido progresivo entre la superficie de video y
 * 		una imagen proporcionada 
 * Parametro:	Imagen con la cual realizar el fundido
 * Parametro:	Duracion en milisegundos del fundido
 * Devuelve:	void
 */
void fadeScreen(SDL_Surface *imagen, Uint32 duracion)
{
	Uint32 ticksPrincipio, ticksActuales, contx, conty;

	/* Declaramos 3 variables para pixeles (uno para el de la imagen original,
	otro para la imagen que vamos a mostrar progresivamente y el tercero para
	guardar el valor de color que nos corresponde mostrar en cada iteracion).
	Ademas, declaramos los componentes RGB de cada pixelk */
	Uint32 pixelOriginal, pixelImagen, pixelNuevo;
	Uint8 rOrig, gOrig, bOrig;
	Uint8 rImagen, bImagen, gImagen;
	Uint8 rNuevo, bNuevo, gNuevo;

	/* El porcentaje del proceso que ya se realizo */
	float porcentage;
	
	
	SDL_Surface *pantalla, *copiaPantalla;
	pantalla=SDL_GetVideoSurface();
	/* Copiamos la pantalla para conservar una copia, ya que a la original la
	vamos a modificar en cada iteracion */
	copiaPantalla=SDL_CreateRGBSurface(SDL_HWSURFACE,pantalla->w,pantalla->h,pantalla->format->BitsPerPixel,pantalla->format->Rmask,pantalla->format->Gmask,pantalla->format->Bmask,pantalla->format->Amask);
	SDL_BlitSurface(pantalla,NULL,copiaPantalla,NULL);
	
	
	ticksPrincipio=ticksActuales=SDL_GetTicks();

	while(ticksActuales-ticksPrincipio<duracion){
		
		ticksActuales=SDL_GetTicks();
		/* Calculamos el procentaje del proceso que ya se llevo a cabo */
		porcentage=(float)((ticksActuales-ticksPrincipio)*100)/duracion;
		if(porcentage>100) porcentage=100;
	
			/* Antes de trabajar directamente sobre las superficies,
			posiblemente sea necesario bloquearlas */
			if(SDL_MUSTLOCK(pantalla)) SDL_LockSurface(pantalla);
			if(SDL_MUSTLOCK(copiaPantalla))SDL_LockSurface(copiaPantalla);
			if(SDL_MUSTLOCK(imagen)) SDL_LockSurface(imagen);

			for(contx=0; contx<pantalla->w; contx++){
				for(conty=0; conty<pantalla->h; conty++){
					/* Leemos los pixeles de la imagen que teniamos al principio
					y de la imagen a la cual vamos a llegar y separamos el color de
					cada pixel en sus componentes RGB */
					pixelOriginal=getpixel(copiaPantalla,contx,conty);
					pixelImagen=getpixel(imagen,contx,conty);
					SDL_GetRGB(pixelOriginal,copiaPantalla->format,&rOrig,&gOrig,&bOrig);
					SDL_GetRGB(pixelImagen,pantalla->format,&rImagen,&gImagen,&bImagen);
						
						/* Con las componentes RGB de cada pixel y el porcentaje actual
						del proceso calculamos las componentes del pixel que deberiamos
						ver en este momento*/
						if(rOrig>rImagen)
							rNuevo=rOrig-(porcentage*(rOrig-rImagen)/(float)100);
						else if(rOrig<rImagen)
							rNuevo=rOrig+(porcentage*(rImagen-rOrig)/(float)100);
						else rNuevo=rImagen;

						if(gOrig>gImagen)
							gNuevo=gOrig-(porcentage*(gOrig-gImagen)/(float)100);
						else if(gOrig<gImagen)
							gNuevo=gOrig+(porcentage*(gImagen-gOrig)/(float)100);
						else gNuevo=gImagen;
					
						if(bOrig>bImagen)
							bNuevo=bOrig-(porcentage*(bOrig-bImagen)/(float)100);
						else if(bOrig<bImagen)
							bNuevo=bOrig+(porcentage*(bImagen-bOrig)/(float)100);
						else bNuevo=bImagen;
					
					/* Formamos el nuevo pixel con las componentes RGB calculadas
					y lo ponemos en pantalla */
					pixelNuevo=SDL_MapRGB(pantalla->format,rNuevo,gNuevo,bNuevo);
					putpixel(pantalla,contx,conty,pixelNuevo);
				}
			}
			/* Desbloqueamos las superficies y actualizamos la pantalla */
			if(SDL_MUSTLOCK(pantalla)) SDL_UnlockSurface(pantalla);
			if(SDL_MUSTLOCK(copiaPantalla))SDL_UnlockSurface(copiaPantalla);
			if(SDL_MUSTLOCK(imagen)) SDL_LockSurface(imagen);
		SDL_Flip(pantalla);
	}
	/* Una vez terminado el proceso, la copia que hicimos de la superficie de video ya no nos sirve */
	SDL_FreeSurface(copiaPantalla);
}


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


