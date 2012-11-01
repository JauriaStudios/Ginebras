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
#include "game.h"

#define MAX_MENU_FUNC 4

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Interface{
	struct list_head listBoxesW;
	Menu *menu;
	char **text;

	// Functions root menu
	int (*MenuRootFunc[MAX_MENU_FUNC])(void *data);
	Game *dataRootFunc[MAX_MENU_FUNC];

	// Functions spell menu
	int (*MenuSpellsFunc[MAX_MENU_FUNC])(void *data);
	Game *dataSpellsFunc[MAX_MENU_FUNC];

	// Functions menu root
	int (*MenuItemsFunc[MAX_MENU_FUNC])(void *data);
	Game *dataItemsFunc[MAX_MENU_FUNC];

} Interface;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Interface* InterfaceConstructor(int numBoxesW, Game *game);
extern void InterfaceDraw(Interface *this, SDL_Surface *screen);
extern void InterfaceDestructor(Interface *this);

#endif // _INTERFACE_H_
