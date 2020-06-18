#include "pch.h"
#include "EzAcc_VisionModule.h"

#include "EzAcc_Log.h"

#include "SDL/include/SDL.h"


EzAcc_VisionModule::EzAcc_VisionModule()
{
	EZACC_LOG("EzAcc: Generating EzAcc_VisionModule ...");
}

// Destructor
EzAcc_VisionModule::~EzAcc_VisionModule()
{
}

// Called before render is available
bool EzAcc_VisionModule::Awake(pugi::xml_node&)
{return true;
}

bool EzAcc_VisionModule::Start()
{return true;
}

bool EzAcc_VisionModule::PreUpdate()
{return true;
}

bool EzAcc_VisionModule::PostUpdate()
{return true;
}

void EzAcc_VisionModule::ModulateTextureColor(SDL_Texture* texture, SDL_Color color)
{
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

void EzAcc_VisionModule::ProcessTexture(SDL_Texture* texture, SDL_Surface* surface)
{
	if (!pixel_modification) {
		SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
	}
	else {
		void* mPixels;
		int mPitch;
		//Lock texture for manipulation
		//Texture is already locked
		if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
		{
			EZACC_LOG("Unable to lock texture! %s\n", SDL_GetError());
		}
		//Copy loaded/formatted surface pixels
		memcpy(mPixels, surface->pixels, surface->pitch * surface->h);

		//Allocate format from window
		Uint32 format = SDL_PIXELFORMAT_ARGB8888; // SDL_GetWindowPixelFormat(App->win->window);
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

		//Get pixel data
		Uint32* pixels = (Uint32*)mPixels;
		int pixelCount = (mPitch / 4) * surface->h;

		//Unlock texture to update
		SDL_UnlockTexture(texture);
		mPixels = NULL;
		SDL_FreeFormat(mappingFormat);

		if (pixel_modificators.size() < 2)
			return;

		// Testing pixel mod with the first 2
		Uint32 colorKey = SDL_MapRGBA(mappingFormat, pixel_modificators[0].pixel_to_mod.r, pixel_modificators[0].pixel_to_mod.g,
			pixel_modificators[0].pixel_to_mod.b, pixel_modificators[0].pixel_to_mod.a);
		Uint32 colorKey2 = SDL_MapRGBA(mappingFormat, pixel_modificators[1].pixel_to_mod.r, pixel_modificators[1].pixel_to_mod.g,
			pixel_modificators[1].pixel_to_mod.b, pixel_modificators[1].pixel_to_mod.a);
		Uint32 newColor1key = SDL_MapRGBA(mappingFormat, pixel_modificators[0].final_pixel.r, pixel_modificators[0].final_pixel.g,
			pixel_modificators[0].final_pixel.b, pixel_modificators[0].final_pixel.a);
		Uint32 newColor2key = SDL_MapRGBA(mappingFormat, pixel_modificators[1].final_pixel.r, pixel_modificators[1].final_pixel.g,
			pixel_modificators[1].final_pixel.b, pixel_modificators[1].final_pixel.a);
		
		//Color key pixels
		for (int i = 0; i < pixelCount; ++i)
		{
			Uint32 pixel = pixels[i];
			SDL_Color rgb;
			SDL_GetRGBA(pixels[i], mappingFormat, &rgb.r, &rgb.g, &rgb.b, &rgb.a);

			if (pixels[i] == colorKey)
			{
				pixels[i] = newColor1key;
			}
			if (pixels[i] == colorKey2)
			{
				pixels[i] = newColor2key;
			}

		}
	}
}

void EzAcc_VisionModule::PerformPixelModification(bool pixel_mod_active)
{
	pixel_modification = pixel_mod_active;
}

void EzAcc_VisionModule::AddNewPixelMod(EzAcc_PixelModificator new_pixel_mod)
{
	pixel_modificators.push_back(new_pixel_mod);
}

void EzAcc_VisionModule::ClearPixelMods()
{
	pixel_modificators.clear();
}

bool EzAcc_VisionModule::CleanUp()
{
	return true;
}
