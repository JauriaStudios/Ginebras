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

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "audio.h"

/**********************************************************
 *** PUBLIC METHODS IMPLEMENTATION
 **********************************************************/

Audio* AudioConstructor()
{
	// Variable definition section
	Audio *this;

	// Alloc memory
	this = (Audio *)malloc(sizeof(Audio));

	this->sound		= NULL;
	this->channel	= -1;
	this->rate		= 44100;
	this->format	= AUDIO_S16SYS;
	this->channels	= 2;
	this->buffers	= 4096;

	// initializer audio
	if(Mix_OpenAudio(this->rate, this->format, this->channels, this->buffers) != 0) {
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}

	return this;
}

void AudioDestructor(Audio *this)
{
	// Flush Audio Channel
	while(Mix_Playing(this->channel) != 0);

	// Free sound file
	Mix_FreeChunk(this->sound);
	Mix_CloseAudio();

	free(this);
}

void AudioLoadFile(Audio *this, char *fileName){
	// Load Sound File
		
	if(!(this->sound = Mix_LoadWAV(fileName))) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
}

void AudioPlaySound(Audio *this){
	this->channel = Mix_PlayChannel(-1, this->sound, 0);
	if(this->channel == -1) {
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	}
}

