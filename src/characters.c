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

#include "characters.h"

#define NUM_ROWS_ATT 6
#define BUFFER_LEN	 50

typedef enum OrientCollision {
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_WEST,
	SOUTH_EAST,
} OrientCollision;

Character* CharacterConstructor(xmlNodePtr cur, CharacterInput input, Map *map)
{
	// Variable definition section
	Character* character;
	xmlNodePtr attributes, sprites;
	xmlChar *nameSprite;
	int coordX, coordY;
	int attackRadius = 1; // CHAPUZA ALERT
	int i;

	character = (Character *)malloc(sizeof(Character));

	strcpy(character->name, (char *)xmlGetProp(cur, (xmlChar*)"name"));

	attributes = cur->xmlChildrenNode;
	attributes = attributes->next;
	
	// Parse xml
	while (attributes) {
		
		if ((!xmlStrcmp(attributes->name, (const xmlChar *)"stats"))){
			character->attributes.strength 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"strength"));
			character->attributes.agility 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"agility"));
			character->attributes.vitality 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"vitality"));
			character->attributes.intelligence 	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"intelligence"));
			character->attributes.spirit 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"spirit"));
			printf("stats\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"attributes"))){
			character->attributes.life  	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"life"));
			character->attributes.mana  	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"mana"));
			character->attributes.vigor 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"vigor"));
			printf("attributes: %d %d %d", character->attributes.life, character->attributes.mana, character->attributes.vigor);
			printf("\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"attack"))){
			character->attributes.attackMelee  	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"melee"));
			character->attributes.attackRange  	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"range"));
			character->attributes.attackMagic  	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"magic"));
			printf("attack\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"defense"))){
			character->attributes.defensePhysic  = atoi((char *)xmlGetProp(attributes, (xmlChar*)"physic"));
			character->attributes.defenseMagic   = atoi((char *)xmlGetProp(attributes, (xmlChar*)"magic"));
			printf("defense\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"other"))){
			character->attributes.critic 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"critic"));
			character->attributes.evasion 	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"evasion"));
			character->attributes.move	  	   	 = atoi((char *)xmlGetProp(attributes, (xmlChar*)"move"));
			printf("other\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"activespells"))){
			//character->spells.  = atoi((char *)xmlGetProp(cur, (xmlChar*)"spell1"));
			//character->spells.  = atoi((char *)xmlGetProp(cur, (xmlChar*)"spell2"));
			printf("activespells\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"pasives"))){
			printf("pasives\n");

		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"spritesheet"))){
			printf("spritesheet\n");
			nameSprite = xmlGetProp(attributes, (xmlChar*)"name");

			if((!xmlStrcmp(nameSprite, (const xmlChar *)"walk"))){
				// Parse walk sprite
				printf("  walk\n");

				sprites = attributes->xmlChildrenNode;
				sprites = sprites->next;

				if((!xmlStrcmp(sprites->name, (const xmlChar *)"image"))){
					if(!(character->sprite = loadImage((char *)xmlGetProp(sprites, (xmlChar*)"source")))){
						printf("CharacterConstructor ERROR: impossible load %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
						return NULL;
					}
					printf("  SRC: %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
				}
			}else if ((!xmlStrcmp(nameSprite, (const xmlChar *)"spell"))){
				// Parse spell sprite
				printf("  spell\n");

				sprites = attributes->xmlChildrenNode;
				sprites = sprites->next;

				if((!xmlStrcmp(sprites->name, (const xmlChar *)"image"))){
					if(!(character->spriteSpell = loadImage((char *)xmlGetProp(sprites, (xmlChar*)"source")))){
						printf("CharacterConstructor ERROR: impossible load %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
						return NULL;
					}
					printf("  SRC: %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
				}
			}else if ((!xmlStrcmp(nameSprite, (const xmlChar *)"slash"))){
				printf("  slash\n");

				sprites = attributes->xmlChildrenNode;
				sprites = sprites->next;

				if((!xmlStrcmp(sprites->name, (const xmlChar *)"image"))){
					if(!(character->spriteSlash = loadImage((char *)xmlGetProp(sprites, (xmlChar*)"source")))){
						printf("CharacterConstructor ERROR: impossible load %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
						return NULL;
					}
					printf("  SRC: %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
				}
			}else if ((!xmlStrcmp(nameSprite, (const xmlChar *)"slash192"))){
				printf("  slash192\n");

				sprites = attributes->xmlChildrenNode;
				sprites = sprites->next;

				if((!xmlStrcmp(sprites->name, (const xmlChar *)"image"))){
					if(!(character->sprite192Slash = loadImage((char *)xmlGetProp(sprites, (xmlChar*)"source")))){
						printf("CharacterConstructor ERROR: impossible load %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
						return NULL;
					}
					printf("  SRC: %s image\n", (char *)xmlGetProp(sprites, (xmlChar*)"source"));	
				}
			}


		} else if ((!xmlStrcmp(attributes->name, (const xmlChar *)"avatar"))){
			printf("avatar\n");
				if(!(character->avatar = loadImage((char *)xmlGetProp(attributes, (xmlChar*)"source")))){
					printf("CharacterConstructor ERROR: impossible load %s image\n", (char *)xmlGetProp(attributes, (xmlChar*)"source"));	
					return NULL;
				}
				printf("  SRC: %s image\n", (char *)xmlGetProp(attributes, (xmlChar*)"source"));
		}
		attributes = attributes->next;
	}
	
	character->attributes.lifeTotal  = character->attributes.life;	
	character->attributes.manaTotal  = character->attributes.mana;
	character->attributes.vigorTotal = character->attributes.vigor;

	// Set player
	character->player = input.player;

	character->movement = character->attributes.move;

	// Set movement
	if (!(character->moveArea = AreaConstructor(input.x0, input.y0, character->attributes.move))){
		SDL_FreeSurface(character->sprite);
		SDL_FreeSurface(character->spriteSlash);
		printf("Character constructor ERROR: couldn't create movement area\n");
		return NULL;
	}
	
    // Set initial state
    character->state = MOVING;

	// Set sprite initial position
	character->rcDest.x 	= input.x0;
	character->rcDest.y 	= input.y0;
	character->rcInitTurn.x = input.x0;
	character->rcInitTurn.y = input.y0;

	// Set initial animation sprite frame
	character->rcSrc.x = 0;
	character->rcSrc.y = input.orientation * SPRITE_SIZE;
	character->rcSrc.w = SPRITE_SIZE;
	character->rcSrc.h = SPRITE_SIZE;
	
	// Set initial character position
	GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);
	map->charPosition[character->player][coordY][coordX] = 1;

	// Set initial orientation
	character->moveOrient = input.orientation;
		
	// Set initial velocity
	character->velocity = 5;

	// Set initial destination point
	character->destinationPoint.x = input.x0;
	character->destinationPoint.y = input.y0;
	
	// Set initial state
	character->moving 	  = 0;
	character->actualStep = 0;
	character->moveSteps  = 0;
	character->moveState  = 0;

	// Set linked list
	INIT_LIST_HEAD(&character->list);
	INIT_LIST_HEAD(&character->listSort);

	// Set initial turn variables
	character->check = 0;
	character->iniciative = input.iniciative;

	// Set initial attack variables
	character->rcSrcAttack.x = 0;
	character->rcSrcAttack.y = input.orientation * SPRITE_SIZE;
	character->rcSrcAttack.w = SPRITE_SIZE;
	character->rcSrcAttack.h = SPRITE_SIZE;
	
	// Set attack atributes
	character->attackState 		= 0;
	character->attacking 		= 0;
	character->actualAttackStep = 0;
	character->attackRadius 	= attackRadius;

	character->projectile = ProjectileConstructor("data/arrow.png");

	strcpy(character->status, "none");	

	// Alloc text for the interface
	character->text = (char**)malloc(sizeof(char*) * NUM_ROWS_ATT);
	for(i = 0; i < NUM_ROWS_ATT; i++)
		character->text[i] = (char*)malloc(sizeof(char) * BUFFER_LEN);
		
	// Fill the text
	sprintf(character->text[0], "%s", 			character->name);
	sprintf(character->text[1], "Life: %d/%d", 	character->attributes.life,  character->attributes.lifeTotal);
	sprintf(character->text[2], "Mana: %d/%d", 	character->attributes.mana,  character->attributes.manaTotal);
	sprintf(character->text[3], "Vigor: %d/%d", character->attributes.vigor, character->attributes.vigorTotal);
	sprintf(character->text[4], "Mov: %d", 		character->attributes.move);
	sprintf(character->text[5], "Status: %s", 	character->status);

	return character;
}

void CharacterDestructor(Character *character)
{
	// Free character images
	SDL_FreeSurface(character->sprite);
	SDL_FreeSurface(character->spriteSlash);
	SDL_FreeSurface(character->sprite192Slash);
	SDL_FreeSurface(character->spriteSpell);
	
	// Free movement area
	AreaDestructor(character->moveArea);
	
	// Free character
	free(character);
}

CharacterInput* CharacterFillConfigure(Orientation orientation, int x0, int y0, int iniciative, int player)
{
	// Variable definition section
	CharacterInput *charInput;

	charInput = (CharacterInput *) malloc(sizeof(CharacterInput));

	charInput->orientation = orientation;
	charInput->x0 		   = x0; 
	charInput->y0 		   = y0;
    charInput->iniciative  = iniciative;
	charInput->player	   = player;

	return charInput;
}

void CharacterSetDestination(Character* character, Cursor* cursor, Map *map)
{
	float m, b; 
	int i, dx, dy;
	int stepsX, stepsY;
	int coordX, coordY;
	
	// Set status move
	character->moving     = 1;
	character->state	  = MOVING;
	character->actualStep = 0;
	character->moveState  = 1;
	character->moveSteps  = 0;
	character->skipFrames = 0;
	character->collision  = 0;
	
	// Set to 0 character position
	GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);
	map->charPosition[character->player][coordY][coordX] = 0;
	map->charPosition[0][coordY][coordX] = 0;

	// Set destination point
	character->destinationPoint.x = cursor->rcDest.x - 16;
	character->destinationPoint.y = cursor->rcDest.y - 32;
	
	//  Slope of the line
	dx = character->destinationPoint.x - character->rcDest.x;
	dy = character->destinationPoint.y - character->rcDest.y;
	
	// skip infinite slope for the vertical movement
	if(dx)
		m = (float) dy/dx;
	else
		m = 0.0;

	// b of the line
	b = character->rcDest.y - ( m*character->rcDest.x );
	
	// steps
	stepsX = abs( dx ) / character->velocity;
	stepsY = abs( dy ) / character->velocity;
	
	character->moveSteps = ceil(sqrt( (stepsX*stepsX) + (stepsY*stepsY) ));
	
	// Set orientation movement
	if( (dy < 0) && (stepsY > stepsX) )
		character->moveOrient = ORIENT_NORTH;
	else if( (dy > 0) && ( stepsY > stepsX ) )
		character->moveOrient = ORIENT_SOUTH;
	else if( (dx > 0) && ( stepsX >= stepsY) )
		character->moveOrient = ORIENT_EAST;
	else if( (dx < 0) && ( stepsX >= stepsY ) )
		character->moveOrient = ORIENT_WEST;

	// Initialitate move vectors
	character->moveX = (int *)malloc(sizeof(int)*character->moveSteps);
	character->moveY = (int *)malloc(sizeof(int)*character->moveSteps);

	// Set move vectors
	if(dx){
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x + ( (i *  dx ) / character->moveSteps ) ;			
			character->moveY[i] = m*character->moveX[i] + b; 
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x - 16;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y - 32;
	}else{
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x;			
			character->moveY[i] = character->rcDest.y + ( (i *  dy ) / character->moveSteps ) ;	
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x - 16;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y - 32;
	}
}

void CharacterGetCoor(Character *character, int *x, int *y)
{
	// Get character coordenate
	GetCoor(character->rcDest.x +16, character->rcDest.y +32, 
			x, y);
}

void CharacterMove(Character *character, Map *map)
{
	int coordNwX, coordNwY, coordNeX, coordNeY, coordSwX, coordSwY, coordSeX, coordSeY;
	int firstX, firstY;
	int coordX, coordY;
	
	// Set the coordenates, if character is moving
	if( character->actualStep < character->moveSteps ){
		
		// Get character coordenate
		GetCoor(character->moveX[0]+16+16, character->moveY[0]+32+16, 
				&firstX, &firstY);
		
		// Get north west corner coordenate
		GetCoor(character->moveX[character->actualStep]+16+COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+COLLISIONS_DEPTH, 
				&coordNwX, &coordNwY);
		
		// Get north east corner coordanate
		GetCoor(character->moveX[character->actualStep]+16+32-COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+COLLISIONS_DEPTH, 
				&coordNeX, &coordNeY);
		
		// Get south west corner coordenate 
		GetCoor(character->moveX[character->actualStep]+16+COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+32-COLLISIONS_DEPTH, 
				&coordSwX, &coordSwY);
		
		// Get south east corner coordenate
		GetCoor(character->moveX[character->actualStep]+16+32-COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+32-COLLISIONS_DEPTH, 
				&coordSeX, &coordSeY);

		// Check collision and skip first coordenate for the initial movement
		if(((map->charPosition[0][coordNwY][coordNwX] || map->collisions[coordNwY][coordNwX]) || 
		    	(map->charPosition[0][coordNeY][coordNeX] || map->collisions[coordNeY][coordNeX]) ||
		    	(map->charPosition[0][coordSwY][coordSwX] || map->collisions[coordSwY][coordSwX]) || 
				(map->charPosition[0][coordSeY][coordSeX] || map->collisions[coordSeY][coordSeX])) && 
		    	(!map->charPosition[0][firstY][firstX])){
			
			character->actualStep = character->moveSteps;

			character->collision = 1;

			goto fin; 
		}else{
			character->rcDest.x = character->moveX[character->actualStep];
			character->rcDest.y = character->moveY[character->actualStep];
			character->actualStep++;
		}
	
		// Animation
		character->rcSrc.y = character->moveOrient * SPRITE_SIZE;
		if(character->skipFrames >= NUM_SKIP_FRAMES) {
			if (character->moveState == 3)
				character->moveState = 1;
			else
				character->moveState++;
			character->skipFrames = 0;
		}else
			character->skipFrames++;
	}else{
fin:
		character->moveState = 0;
		character->moving = 0;
		character->state = STOP;
		
		// Set to 0 character position
		GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);

		map->charPosition[0][coordY][coordX] = 1;
		map->charPosition[character->player][coordY][coordX] = 1;
		//free(character->moveX);
		//free(character->moveY);
	}
	character->rcSrc.x = character->moveState * SPRITE_SIZE;
}

void CharacterDraw(Character* character, SDL_Surface* screen, Map *map)
{
	SDL_Rect rc192;

	if(character->moving)
		SDL_BlitSurface(character->sprite, &character->rcSrc, map->surfaceBack, &character->rcDest);
	else if(character->attacking){
		if(character->attackType == SLASH)
			SDL_BlitSurface(character->spriteSlash, &character->rcSrcAttack, map->surfaceBack, &character->rcDest);
		else if(character->attackType == SPELL)
			SDL_BlitSurface(character->spriteSpell, &character->rcSrcAttack, map->surfaceBack, &character->rcDest);
		else if(character->attackType == SLASH192){
			rc192.x = character->rcDest.x - SPRITE_SIZE;
			rc192.y = character->rcDest.y - SPRITE_SIZE;
			SDL_BlitSurface(character->sprite192Slash, &character->rcSrcAttack, map->surfaceBack, &rc192);		
		}
	}else
		SDL_BlitSurface(character->sprite, &character->rcSrc, map->surfaceBack, &character->rcDest);	
	
	ProjectileDraw(character->projectile, map);
}

void CharacterSetAttack(Character *character, AttackType type)
{
	// Set status attack
	character->attacking  		= 1;
	character->state			= ATTACKING;
	character->actualAttackStep	= 0;
	character->attackState  	= 1;
	character->skipFrames 		= 0;
	character->attackType 		= type;
	switch(type){
		case SLASH:
			character->attackSteps = 3; // +1
			break;
		case SPELL:
			character->attackSteps = 3;
			break;
		case SLASH192:
			character->attackSteps = 4;
			break;
		default:
			break;
	}
}

void CharacterAttack(Character *character)
{
	int spriteSize = SPRITE_SIZE;
	int x, y;
	

	if((character->attackType == SLASH) || (character->attackType == SPELL)){
		spriteSize = SPRITE_SIZE;
		character->rcSrcAttack.w = SPRITE_SIZE;
		character->rcSrcAttack.h = SPRITE_SIZE;
	}else if(character->attackType == SLASH192){
		spriteSize = 192;
		character->rcSrcAttack.w = 192;
		character->rcSrcAttack.h = 192;
	}

	if((character->actualAttackStep < character->attackSteps) && character->attacking){
		character->rcSrcAttack.y = character->moveOrient * spriteSize;
		if(character->skipFrames == NUM_SKIP_FRAMES) {
			if ((character->attackState == 5) && ((character->attackType == SLASH) || (character->attackType == SLASH192)))
				character->attackState = 1;
			else if ((character->attackState == 4) && (character->attackType == SPELL))
				character->attackState = 1;	
			else{
				character->attackState++;
				character->actualAttackStep++;
			}
			character->skipFrames = 0;
		}else{
			character->skipFrames++;
		}
	}else{
		character->attackState = 0;
		character->attacking = 0;
		character->state = STOP;
		switch(character->moveOrient){
			case ORIENT_NORTH:
				x = 0;
				y = -32*10;
				character->projectile->rcSrc.x = 0;
				character->projectile->rcSrc.y = 0 * PROJECTILE_TILE_SIZE;
				break;
			case ORIENT_SOUTH:
				x = 0;
				y = 32*10;
				character->projectile->rcSrc.x = 0;
				character->projectile->rcSrc.y = 2 * PROJECTILE_TILE_SIZE;
				break;
			case ORIENT_EAST:
				x = 32*10;
				y = 0;
				character->projectile->rcSrc.x = 0;
				character->projectile->rcSrc.y = 3 * PROJECTILE_TILE_SIZE;
				break;
			case ORIENT_WEST:
				x = -32*10;
				y = 0;
				character->projectile->rcSrc.x = 0;
				character->projectile->rcSrc.y = 1 * PROJECTILE_TILE_SIZE;
				break;
			default:
				x = 0;
				y = 0;
				character->projectile->rcSrc.x = 0;
				character->projectile->rcSrc.y = 0;
				break;
		}
		if(character->projectile->action){
			ProjectileSetTrajectory(character->projectile, character, character->rcDest.x + x, character->rcDest.y + y);
		}
	}
	character->rcSrcAttack.x = character->attackState * spriteSize;
	
}

int CharacterCheckEnemy(Character *character, Map *map, int radius, int *xFind, int *yFind)
{
	// Variable definition section
	int i, j, x, y, charX, charY;
	int **shade;
	int sideLen = 2*radius+1;
	int playerEnemy;

	// Get character position
	CharacterGetCoor(character, &charX, &charY);

	shade = AreaGetShade(radius);

	x = charX - radius;
	y = charY - radius;

	if(character->player == 1)
		playerEnemy = 2;
	else
		playerEnemy = 1;

	// Seach an enemy in the attack area
	for(i = 0; i < sideLen; i++){
		for(j = 0; j < sideLen; j++){
			if(map->charPosition[playerEnemy][y+i][x+j] && shade[i][j] && (i != radius || j != radius)){
				*yFind = y+i;
				*xFind = x+j; 
				// Free Submenus
    			for(i = 0; i < sideLen; i++)
    				free(shade[i]);
    			free(shade);
				return 1;
			}
		}
	}

	 // Free Submenus
    for(i = 0; i < sideLen; i++)
    	free(shade[i]);
    free(shade);

	return 0;
}
