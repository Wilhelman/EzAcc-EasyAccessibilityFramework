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


#ifndef _EZACC_VISIONMODULE_H_
#define _EZACC_VISIONMODULE_H_

#include "EzAcc_Defines.h"
#include "EzAcc_Module.h"

#include "SDL/include/SDL.h"

struct ExAcc_PixelModificator {
	SDL_Color pixel_to_mod;
	SDL_Color final_pixel;
};


class EzAcc_VisionModule : public EzAcc_Module
{

public:

	EzAcc_VisionModule();

	// Destructor
	virtual ~EzAcc_VisionModule();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool PostUpdate();

	void ModulateTextureColor(SDL_Texture* texture, SDL_Color color);

	// TODO: Make this easier with a vector for example ...
	void ProcessTexture(SDL_Texture* texture, SDL_Surface* surface,
		ExAcc_PixelModificator pixelMod01, ExAcc_PixelModificator pixelMod02);
	
	void PerformPixelModification(bool pixel_mod_active);

	// Called before quitting
	bool CleanUp();


private:
	bool pixel_modification = false;

};

#endif