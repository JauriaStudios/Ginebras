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

#include "interface.h"

#define HEIGHT_COEF 6

char *text[] = {"Link", 
				"Life: 3000/9900", 
				"Mana: 100/150", 
				"Vigor: 150/200", 
				"Mov: 10",
				"State: none"};

char *menuText[] = {" * Attack", 
					"   Spells", 
					"   Items", 
					"   Defend"};

char *spellsText[] = {" * Fire", 
					  "   Ice", 
					  "   Lighthing", 
					  "   Shield"};

char *itemsText[] = {" * Lol potion", 
					 "   Mana potion", 
					 "   Remedy"};

int endBranch[4] = {1, 0, 0, 1}; // this mean that the selected branch has or not has a submenu

char *names[] = {"char0", "char1", "char2", "char3", "char4", "char5", "char6", "char7"};

Interface* InterfaceConstructor(int numBoxesW)
{
	// Variable definition section
    Interface *this;
	Textbox *box;
	Menu *menu;
	int i;

	this = (Interface *)malloc(sizeof(Interface));
	INIT_LIST_HEAD(&this->listBoxesW);

    // Alloc text (development)
	this->text = (char**)malloc(sizeof(char*) * 25);
	for(i = 0; i < 25; i++)
		this->text[i] = (char*)malloc(sizeof(char) * 10);	
	
	for(i = 0; text[i]; i++)
		strcpy(this->text[i], text[i]);

	// Create horizontal interface
	for(i = 0; i < numBoxesW; i++){
		box = TextboxConstructor(names[i], (SCREEN_WIDTH/numBoxesW) * i, SCREEN_HEIGHT-((SCREEN_HEIGHT/HEIGHT_COEF)+12), 
										   (SCREEN_WIDTH/numBoxesW)-1, SCREEN_HEIGHT/HEIGHT_COEF, this->text, 6, NULL, NULL);
		list_add_tail(&box->list, &this->listBoxesW);
	}

	// Create image textbox
	box = TextboxConstructor("image", (SCREEN_WIDTH/numBoxesW) * 7, SCREEN_HEIGHT-(2*(SCREEN_HEIGHT/HEIGHT_COEF)+22), 
									   (SCREEN_WIDTH/numBoxesW)-5, (SCREEN_HEIGHT/HEIGHT_COEF), NULL, 0, "data/viktor135.png", NULL);
	list_add_tail(&box->list, &this->listBoxesW);

	// Create menu
    menu = MenuConstructor(menuText, 4,endBranch, 4);
	MenuAddSubMenu(menu, NULL, 0);
	MenuAddSubMenu(menu, spellsText, 4);
	MenuAddSubMenu(menu, itemsText, 3);
	MenuAddSubMenu(menu, NULL, 0);
	this->menu = menu;
	box = TextboxConstructor("menu", (SCREEN_WIDTH/numBoxesW) * 7, SCREEN_HEIGHT-(3*(SCREEN_HEIGHT/HEIGHT_COEF)+32), 
									   (SCREEN_WIDTH/numBoxesW)-5, (SCREEN_HEIGHT/HEIGHT_COEF), NULL, 0, NULL, menu);
	list_add_tail(&box->list, &this->listBoxesW);
	
	return this;
}

void InterfaceDestructor(Interface *this)
{
	// Variable definition section
	Textbox *tmp, *tmp2;

	// Free boxes
	list_for_each_entry_safe(tmp, tmp2, &this->listBoxesW, list) {
		list_del(&tmp->list);
		TextboxDestructor(tmp);
	}

	free(this);
}

void InterfaceDraw(Interface *this, SDL_Surface *screen)
{
	Textbox* tmp;

	list_for_each_entry(tmp, &this->listBoxesW, list){
		TextboxDraw(tmp, screen);
	}

}
