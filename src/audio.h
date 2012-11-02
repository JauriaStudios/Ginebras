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

#ifndef _AUDIO_H_
#define _AUDIO_H_

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Audio{

	Mix_Chunk *sound;
	
	// Config for OpenAudio()
	int channel;
	int rate;
	Uint16 format;
	int channels;
	int buffers;	// Only for developement

} Audio;
extern Audio* AudioConstructor();
extern void AudioLoadFile(Audio *this, char *fileName);
extern void AudioPlaySound(Audio *this);
extern void AudioDestructor(Audio *this);

#endif // _AUDIO_H_
