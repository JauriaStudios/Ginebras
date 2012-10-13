#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "list.h"
#include "textbox.h"
#include "map.h"

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Interface{
	struct list_head listBoxesW;
} Interface;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Interface* InterfaceConstructor(int numBoxesW);
extern void InterfaceDraw(Interface *this, SDL_Surface *screen);
extern void InterfaceDestructor(Interface *this);

#endif // _MAP_H_
