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

#include "menu.h"

/**********************************************************
 *** PUBLIC METHODS IMPLEMENTATION
 **********************************************************/
Menu* MenuConstructor(char **root, int numRows, int *endBranch, int numSubMenus)
{
	// Variable definition section
	Menu *this;

	// Alloc memory for root menu
	this = (Menu *)malloc(sizeof(Menu));

	// Save root menu text
	this->root = root;
	
	// Set end branch
	this->endBranch = endBranch;

	// Set initial position menu cursor
	this->position = 0;
	
	// Set root rows
	this->numRows = numRows;
 
	// Alloc memory for submenus vectors
	this->subMenus = (char ***)malloc(numSubMenus * sizeof(char **));

	return this;
}

int	MenuAddSubMenu(char **subMenu, int numRows)
{
	
}

void MenuDestructor(Menu *this)
{
	free(this->subMenus);

	free(this);
}

int MenuOk(Menu *this)
{

}

int	MenuBack(Menu *this)
{

}

int MenuUp(Menu *this)
{

}

int MenuDown(Menu *this)
{

}
