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

int Attack(void *data)
{
	Game *game = (Game *)data;
	Character *character = game->actualCharacter;
	int aRadius = 1, x, y;

	// Construct area attack
	if(CharacterCheckEnemy(character, game->map, aRadius, &x, &y)){
		AreaSetAttackArea(character->moveArea, character, aRadius, game->cursor);
		//CursorSetPosition(game->cursor, x, y);	
		game->menu->colorRow = game->menu->position;
	}
	return 0;
}

int Defend(void *data)
{
	Game *game = (Game *)data;
	Character *character = game->actualCharacter;

	CharacterSetAttack(character, SPELL);

	return 0;
}

int Back(void *data)
{
	Game *game = (Game *)data;
	Character *character = game->actualCharacter;

	AreaSetMovArea(character->moveArea, character, game->cursor);
	game->menu->colorRow = -1;
	
	return 0;
}

Interface* InterfaceConstructor(int numBoxesW, Game *game)
{
	// Variable definition section
    Interface *this;
	Textbox *box;
	Menu *menu;
	int i;

	this = (Interface *)malloc(sizeof(Interface));
	INIT_LIST_HEAD(&this->listBoxesW);

	this->showMenu = 0;

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

	// Root menu functions
	this->MenuRootFunc[0] = Attack;
	this->MenuRootFunc[1] = NULL;
	this->MenuRootFunc[2] = NULL;
	this->MenuRootFunc[3] = Defend;

	// Root data functions
	this->dataRootFunc[0] = game;
	this->dataRootFunc[1] = NULL;
	this->dataRootFunc[2] = NULL;
	this->dataRootFunc[3] = game;

	// Spells menu functions
	this->MenuSpellsFunc[0] = Attack;
	this->MenuSpellsFunc[1] = NULL;
	this->MenuSpellsFunc[2] = NULL;
	this->MenuSpellsFunc[3] = Defend;

	// Root data functions
	this->dataSpellsFunc[0] = game;
	this->dataSpellsFunc[1] = NULL;
	this->dataSpellsFunc[2] = NULL;
	this->dataSpellsFunc[3] = game;

	// Root menu functions
	this->MenuItemsFunc[0] = Attack;
	this->MenuItemsFunc[1] = NULL;
	this->MenuItemsFunc[2] = NULL;

	// Root data functions
	this->dataItemsFunc[0] = game;
	this->dataItemsFunc[1] = NULL;
	this->dataItemsFunc[2] = NULL;

	// Create menu
    menu = MenuConstructor(menuText, 4,endBranch, 4, this->MenuRootFunc, this->dataRootFunc);
	MenuAddSubMenu(menu, NULL, 0, NULL, NULL);
	MenuAddSubMenu(menu, spellsText, 4, this->MenuSpellsFunc, this->dataSpellsFunc);
	MenuAddSubMenu(menu, itemsText, 3, this->MenuItemsFunc, this->dataItemsFunc);
	MenuAddSubMenu(menu, NULL, 0, NULL, NULL);
	menu->visible = 0;
	menu->MenuBack = &Back;
	menu->menuBackData = game;
	this->menu = menu;
	
	// Construt the menu text box
	box = TextboxConstructor("menu", (SCREEN_WIDTH/numBoxesW) * 7, SCREEN_HEIGHT-(3*(SCREEN_HEIGHT/HEIGHT_COEF)+32), 
									   (SCREEN_WIDTH/numBoxesW)-5, (SCREEN_HEIGHT/HEIGHT_COEF), NULL, 0, NULL, menu);
	box->visible = 0;
	list_add_tail(&box->list, &this->listBoxesW);
	
	return this;
}

void InterfaceDestructor(Interface *this)
{
	// Variable definition section
	Textbox *tmp, *tmp2;

	// Free boxes
	list_for_each_entry_safe(tmp, tmp2, &this->listBoxesW, list) {
		//printf("free %s\n", tmp->name);
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
