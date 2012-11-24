#
#   Makefile
#
#   Copyright 2012 Jauria Studios
#
#   This file is part of Sons of Battle.
#
#   Sons of Battle is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Sons of Battle is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Sons of Battle.  If not, see <http://www.gnu.org/licenses/>.
#

LDFLAGS = `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lz `xml2-config --libs`
CCFLAGS = -O2 -Wall `sdl-config --cflags` -g `xml2-config --cflags`
#
OBJS = 	src/main.o src/intro.o src/cursor.o src/characters.o src/timer.o  \
		src/game.o src/gameUtilities.o src/map.o src/player.o \
		src/area.o src/textbox.o src/interface.o src/grid.o \
		src/menu.o src/audio.o src/projectile.o
#
default:	ginebras

%.o:		%.c
		gcc -c -o $@ $? ${CCFLAGS}

ginebras:	${OBJS}
		gcc -o $@ $+ ${LDFLAGS}

clean:		
		rm ${OBJS}

