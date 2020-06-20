/*
  EzAcc - Easy Accessibility Framework
  Copyright (C) 2020 Guillermo García Subirana <guillermogarsub@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/


#ifndef _EZACC_AUDIOMODULE_H_
#define _EZACC_AUDIOMODULE_H_

#define EZACC_DEFAULT_MUSIC_FADE_TIME 2.0f
#define EZACC_MAX_FX 200

#include "EzAcc_Defines.h"
#include "EzAcc_Module.h"

#include "p2DynArray.h"
#include "p2SString.h"

#include "SDL_mixer\include\SDL_mixer.h"

struct _Mix_Music;
struct Mix_Chunk;

class EzAcc_AudioModule : public EzAcc_Module
{

public:

	EzAcc_AudioModule();

	// Destructor
	virtual ~EzAcc_AudioModule();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = EZACC_DEFAULT_MUSIC_FADE_TIME);
	bool StopMusic();

	// Load a WAV in memory
	unsigned int LoadFx(const char* path, const char* new_embedded_language);

	// Play a previously loaded WAV
	const char* PlayFx(unsigned int fx, int repeat = 0);

	// UnLoad WAV
	bool UnLoadFx(uint id);

	void SetMusicVolume(int value);
	void SetFXVolume(int value);

private:

	_Mix_Music* music = nullptr;
	Mix_Chunk* fx[EZACC_MAX_FX];
	p2DynArray<p2SString> embedded_languages;

	uint				last_fx = 1;

};

#endif