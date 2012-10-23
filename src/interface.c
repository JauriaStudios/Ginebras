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

Interface* InterfaceConstructor(int numBoxesW)
{
	Interface *this;
	Textbox *box;
	int i;

	char *names[] = {"char0", "char1", "char2", "char3", "char4", "char5", "char6", "char7"};

	this = (Interface *)malloc(sizeof(Interface));
	INIT_LIST_HEAD(&this->listBoxesW);

	// Create horizontal interface
	for(i = 0; i < numBoxesW; i++){
		box = TextboxConstructor(names[i], (SCREEN_WIDTH/numBoxesW) * i, SCREEN_HEIGHT-((SCREEN_HEIGHT/HEIGHT_COEF)+12), 
										   (SCREEN_WIDTH/numBoxesW)-1, SCREEN_HEIGHT/HEIGHT_COEF, text, 6, NULL);
		list_add_tail(&box->list, &this->listBoxesW);
	}

	// Create image textbox
	box = TextboxConstructor("image", (SCREEN_WIDTH/numBoxesW) * 7, SCREEN_HEIGHT-(2*(SCREEN_HEIGHT/HEIGHT_COEF)+22), 
									   (SCREEN_WIDTH/numBoxesW)-5, (SCREEN_HEIGHT/HEIGHT_COEF), NULL, 0, "data/viktor135.png");
	list_add_tail(&box->list, &this->listBoxesW);

	return this;
}

void InterfaceDestructor(Interface *this)
{
	free(this);
}

void InterfaceDraw(Interface *this, SDL_Surface *screen)
{
	Textbox* tmp;

	list_for_each_entry(tmp, &this->listBoxesW, list){
		TextboxDraw(tmp, screen);
	}

}
