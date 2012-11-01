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

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "list.h"
#include "textbox.h"
#include "map.h"
#include "menu.h"

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Interface{
	struct list_head listBoxesW;
	Menu *menu;
	char **text;
} Interface;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Interface* InterfaceConstructor(int numBoxesW);
extern void InterfaceDraw(Interface *this, SDL_Surface *screen);
extern void InterfaceDestructor(Interface *this);

#endif // _INTERFACE_H_
