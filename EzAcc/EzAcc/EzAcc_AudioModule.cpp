#include "pch.h"
#include "EzAcc_AudioModule.h"

#include "EzAcc_Log.h"

#include "SDL/include/SDL.h"


EzAcc_AudioModule::EzAcc_AudioModule()
{
	music = NULL;
	EZACC_LOG("EzAcc: Generating EzAcc_AudioModule ...");
}

// Destructor
EzAcc_AudioModule::~EzAcc_AudioModule()
{

}

// Called before render is available
bool EzAcc_AudioModule::Awake(pugi::xml_node& save)
{
	EZACC_LOG("Awake Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		EZACC_LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		EZACC_LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		EZACC_LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	Mix_VolumeMusic(20);

	return ret;
}

bool EzAcc_AudioModule::Start()
{
	return true;
}

bool EzAcc_AudioModule::PreUpdate()
{


	return true;
}

bool EzAcc_AudioModule::Update(float dt)
{
	// Testing audio stuff ...

	/*if (core->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		Mix_VolumeMusic(Mix_VolumeMusic(-1) + 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		if (Mix_VolumeMusic(-1) < 10)
			Mix_VolumeMusic(0);
		else
			Mix_VolumeMusic(Mix_VolumeMusic(-1) - 10);
	}*/

	return true;
}

bool EzAcc_AudioModule::PostUpdate()
{

	return true;
}

bool EzAcc_AudioModule::CleanUp()
{
	EZACC_LOG("Freeing music, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool EzAcc_AudioModule::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		else
			Mix_HaltMusic();
		
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		EZACC_LOG("EzAcc: Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				EZACC_LOG("EzAcc: Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				EZACC_LOG("EzAcc: Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}
	EZACC_LOG("Successfully playing %s", path);
	return ret;
}

bool EzAcc_AudioModule::StopMusic()
{
	Mix_FreeMusic(music);
	music = nullptr;
	Mix_HaltMusic();
	return true;
}

unsigned int EzAcc_AudioModule::LoadFx(const char* path, const char* new_embedded_language)
{
	uint ret = 0;
	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == nullptr)
	{
		EZACC_LOG("EzAcc: Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		p2SString tmp_string;
		tmp_string.create(new_embedded_language);
		embedded_languages.PushBack(tmp_string);
		fx[last_fx] = chunk;
		ret = last_fx++;
		if (last_fx == EZACC_MAX_FX) {
			last_fx = 0;
			ret = last_fx;
		}
	}

	return ret;
}

const char* EzAcc_AudioModule::PlayFx(unsigned int id, int repeat)
{
	p2SString null_ret;
	null_ret.create("");

	if (fx[id] != nullptr)
	{
		p2SString embedded_lang = embedded_languages[id - 1];
		EZACC_LOG("EzAcc: returning embedded lang: ");
		EZACC_LOG(embedded_lang.GetString());
		Mix_PlayChannel(-1, fx[id], repeat);
		return embedded_languages[id - 1].GetString();
	}

	return "";
}

bool EzAcc_AudioModule::UnLoadFx(uint id)
{
	bool ret = false;

	if (fx[id] != nullptr)
	{
		Mix_FreeChunk(fx[id]);
		fx[id] = nullptr;
		ret = true;
		//last_fx--;
	}

	return ret;
}

void EzAcc_AudioModule::SetMusicVolume(int value)
{
	Mix_VolumeMusic(128 * value / 100);
}

void EzAcc_AudioModule::SetFXVolume(int value)
{
	Mix_Volume(-1, 128 * value / 100);
}
