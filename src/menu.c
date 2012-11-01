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

#define MAX_ROWS    10
#define MAX_ROW_LEN 25

/**********************************************************
 *** PUBLIC METHODS IMPLEMENTATION
 **********************************************************/
Menu* MenuConstructor(char **root, int numRows, int *endBranch, int numSubMenus)
{
	// Variable definition section
	Menu *this;
    int i;

	// Alloc memory for root menu
	this = (Menu *)malloc(sizeof(Menu));

    // Alloc root menu text
	this->root = (char**)malloc(sizeof(char*) * MAX_ROW_LEN);
	for(i = 0; i < MAX_ROW_LEN; i++)
		this->root[i] = (char*)malloc(sizeof(char) * MAX_ROWS);
    
    // Save the text
    for(i = 0; root[i]; i++)
		strcpy(this->root[i], root[i]);

	// Set root rows
	this->numRows = numRows;

	// Set end branch
	this->endBranch = endBranch;

	// Alloc memory for submenus vectors
	this->subMenus = (char ***)malloc(numSubMenus * sizeof(char **));

	// Set number of submenus
	this->numSubMenus = numSubMenus;

	// Alloc rows number submenus
	this->numRowsSubMenu = (int *)malloc(numSubMenus *  sizeof(int));
	
	// Set initial position menu cursor
	this->position = 0;

	// Set initial submenus count
	this->countSubMenus = 0;

	// Set actual menu	
	this->actualMenu = this->root;
	this->actualMenuRows = this->numRows;
	this->isRoot = 1;

	return this;
}

int	MenuAddSubMenu(Menu *this, char **subMenu, int numRows)
{
    // Variable definition section
    int i;

	// Check if the menu is not full
	if(this->countSubMenus >= this->numSubMenus){
		printf("MenuAddSubMenu ERROR: Sub menu vector is full\n");
		return -1;
	}

    if(subMenu){
        // Alloc submenu
        this->subMenus[this->countSubMenus] = (char **)malloc(sizeof(char*) * MAX_ROW_LEN);
        for(i = 0; i < MAX_ROW_LEN; i++)
            this->subMenus[this->countSubMenus][i] = (char*)malloc(sizeof(char) * MAX_ROWS);

        // Save the text
        for(i = 0; subMenu[i]; i++)
		    strcpy(this->subMenus[this->countSubMenus][i], subMenu[i]);

	    // Set number of rows in the sub menu
	    this->numRowsSubMenu[this->countSubMenus] = numRows;
    }
	// Increment number of sub menus
	this->countSubMenus++;

	return 0;
}

void MenuDestructor(Menu *this)
{
    // Variable definition section
    int i, j;

    // Free Submenus
    for(i = 0; i < MAX_ROWS; i++)
        for(j = 0; j < MAX_ROW_LEN; j++)
            free(this->subMenus[i][j]);
    free(this->subMenus);        

    // Free root text
    for(i = 0; i < MAX_ROWS; i++)
        free(this->root[i]);
    free(this->root);

	free(this->subMenus);
	free(this->numRowsSubMenu);
	free(this);
}

int MenuOk(Menu *this)
{
	if(!this->endBranch[this->position] && this->isRoot){
		this->previusMenu = this->actualMenu;
		this->previusMenuRows = this->actualMenuRows;
			
		this->actualMenu = this->subMenus[this->position];
		this->actualMenuRows = this->numRowsSubMenu[this->position];

		this->previusPosition = this->position;
		this->position = 0;
		this->isRoot = 0;	
	}
	return 0;
}

int	MenuBack(Menu *this)
{
	if(!this->isRoot){
		this->actualMenu[this->position][1] = ' ';
		this->actualMenu[0][1] = '*';
	
		this->actualMenu = this->previusMenu;
		this->actualMenuRows = this->previusMenuRows;
		this->position = this->previusPosition;
		
		this->isRoot = 1;
	}	

	return 0;
}

int MenuUp(Menu *this)
{
	if(this->position){
		this->actualMenu[this->position][1] = ' ';
		this->position--;
		this->actualMenu[this->position][1] = '*';
	}	
	return 0;
}


int MenuDown(Menu *this)
{
	if(this->position != this->actualMenuRows - 1){
		this->actualMenu[this->position][1] = ' ';
		this->position++;
		this->actualMenu[this->position][1] = '*';
	}	
	return 0;
}
