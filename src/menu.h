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

#ifndef _MENU_H_
#define _MENU_H_

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Menu{
	char **root;
	char ***subMenus;
	int numSubMenus;	
} Menu;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Menu* MenuConstructor();
extern void MenuDestructor(Menu *this);

#endif // _MENU_H_
