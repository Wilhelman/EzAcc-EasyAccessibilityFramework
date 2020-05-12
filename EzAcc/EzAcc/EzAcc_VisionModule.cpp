#include "pch.h"
#include "EzAcc_VisionModule.h"

#include "EzAcc_Log.h"

#include "SDL/include/SDL.h"


EzAcc_VisionModule::EzAcc_VisionModule()
{
	LOG("EzAcc: Generating EzAcc_VisionModule ...");
}

// Destructor
EzAcc_VisionModule::~EzAcc_VisionModule()
{

}

// Called before render is available
bool EzAcc_VisionModule::Awake(pugi::xml_node&)
{
	bool ret = true;


	return ret;
}

bool EzAcc_VisionModule::Start()
{
	return true;
}

bool EzAcc_VisionModule::PreUpdate()
{

	void* mPixels;
	int mPitch;
	if (App->ken_stage_scene->atlas_tex == texture) {
		//Lock texture for manipulation
		//Texture is already locked

		if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
		{
			LOG("Unable to lock texture! %s\n", SDL_GetError());
		}


		//Copy loaded/formatted surface pixels
		memcpy(mPixels, App->ken_stage_scene->backgroundSurface->pixels, App->ken_stage_scene->backgroundSurface->pitch * App->ken_stage_scene->backgroundSurface->h);

		//Allocate format from window
		Uint32 format = SDL_PIXELFORMAT_RGBA8888; // SDL_GetWindowPixelFormat(App->win->window);
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

		//Get pixel data
		Uint32* pixels = (Uint32*)mPixels;
		int pixelCount = (mPitch / 4) * rect.h; //TODOG puede que App->ken_stage_scene->backgroundSurface->h

		Uint32 colorKey = SDL_MapRGBA(mappingFormat, 128, 5, 50, 255);
		Uint32 transparent = SDL_MapRGBA(mappingFormat, 0, 0, 255, 50);

		//Color key pixels
		for (int i = 0; i < pixelCount; ++i)
		{
			Uint32 pixel = pixels[i];
			SDL_Color rgb;
			SDL_GetRGBA(pixels[i], mappingFormat, &rgb.r, &rgb.g, &rgb.b, &rgb.a);

			if (pixels[i] == colorKey)
			{
				pixels[i] = transparent;
			}

		}


		//Unlock texture to update
		SDL_UnlockTexture(texture);
		mPixels = NULL;
		SDL_FreeFormat(mappingFormat);
	}
	return true;
}

bool EzAcc_VisionModule::PostUpdate()
{

	return true;
}

bool EzAcc_VisionModule::CleanUp()
{
	return true;
}
