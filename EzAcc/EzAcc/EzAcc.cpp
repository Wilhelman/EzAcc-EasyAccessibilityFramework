#include "pch.h"

#define DLL_EXPORT 

#include "EzAcc.h"
#include "EzAcc_Log.h"

DECLDIR bool EzAcc_Init()
{
	bool ret = false;
	core = new EzAcc_Core();
	(core != NULL) ?  ret = true : ret = false;
	return ret;
}

DECLDIR bool EzAcc_Awake()
{
	return core->Awake();
}

DECLDIR bool EzAcc_Start()
{
	return core->Start();
}

DECLDIR bool EzAcc_PreUpdate()
{
	return core->PreUpdate();
}

DECLDIR bool EzAcc_Update()
{
	return core->Update();
}

DECLDIR bool EzAcc_AllUpdate()
{
	return core->AllUpdate();
}

DECLDIR bool EzAcc_PostUpdate()
{
	return core->PostUpdate();
}

DECLDIR bool EzAcc_CleanUp()
{
	return core->CleanUp();
}

DECLDIR EzAcc_KeyState EzAcc_GetKey(int id)
{
	return core->input->GetKey(id);
}

DECLDIR void EzAcc_SetTimeBetweenInputs(unsigned int new_time)
{
	if (core->input && core->input != NULL) {
		core->input->time_between_inputs = new_time;
	}
	else {
		EZACC_LOG("EzAcc: ERROR using input module, did you used \"EzAcc_Init\" to initialise the module?");
	}
}

DECLDIR unsigned int EzAcc_GetTimeBetweenInputs()
{
	if (core->input && core->input != NULL) {
		return core->input->time_between_inputs;
	}
	else {
		EZACC_LOG("EzAcc: ERROR using input module, did you used \"EzAcc_Init\" to initialise the module?");
	}
}

DECLDIR int EzAcc_BindKey()
{
	if (core->input && core->input != NULL) {
		return core->input->BindKey();
	}
	else {
		EZACC_LOG("EzAcc: ERROR using input module, did you used \"EzAcc_Init\" to initialise the module?");
	}
}

DECLDIR int EzAcc_GenerateNewMacro(EzAcc_Macro new_macro)
{
	return core->input->SetMacroForKey(new_macro);
}

DECLDIR bool EzAcc_RemoveMacro(int index)
{
	return core->input->RemoveMacro(index);
}

DECLDIR bool EzAcc_PerformRumble(float rumble_power, int rumble_time)
{
	return core->input->PerformRumble(rumble_power, rumble_time);
}

DECLDIR void EzAcc_SetDTTimeScale(float timescale)
{
	core->time->SetTimeScale(timescale);
}

DECLDIR float EzAcc_GetDT()
{
	return core->time->GetDT();
}

DECLDIR float EzAcc_GetGameTime()
{
	return core->time->GetGameTime();
}

DECLDIR EzAcc_FormatHour EzAcc_GetGameTimeHourFormat()
{
	return core->time->GetGameTimeFormatHour();
}

DECLDIR float EzAcc_GetRealtime()
{
	return core->time->GetRealTime();
}

DECLDIR EzAcc_FormatHour EzAcc_GetRealtimeHourFormat()
{
	return core->time->GetRealTimeFormatHour();
}

DECLDIR void EzAcc_PerformPixelModification(bool pixel_mod_active)
{
	core->vision->PerformPixelModification(pixel_mod_active);
}

DECLDIR void EzAcc_ModulateTextureColor(SDL_Texture* texture, SDL_Color color)
{
	core->vision->ModulateTextureColor(texture, color);
}

DECLDIR void EzAcc_ProcessTexture(SDL_Texture* texture, SDL_Surface* surface)
{
	core->vision->ProcessTexture(texture, surface);
}

DECLDIR void EzAcc_AddNewPixelMod(EzAcc_PixelModificator new_pixel_mod)
{
	core->vision->AddNewPixelMod(new_pixel_mod);
}

DECLDIR void EzAcc_ClearPixelMods()
{
	core->vision->ClearPixelMods();
}

DECLDIR void EzAcc_ActivateNewCursor(char* cursor_path, int hot_x, int hot_y)
{
	core->vision->ActivateNewCursor(cursor_path, hot_x, hot_y);
}

DECLDIR void EzAcc_DeactivateCursor()
{
	core->vision->DeactivateCursor();
}

DECLDIR bool EzAcc_PlayMusic(const char* path, float fade_time)
{
	return core->audio->PlayMusic(path, fade_time);
}

DECLDIR bool EzAcc_StopMusic()
{
	return core->audio->StopMusic();
}

DECLDIR unsigned int EzAcc_LoadFx(const char* path, const char* new_embedded_language)
{
	return core->audio->LoadFx(path, new_embedded_language);
}

DECLDIR const char* EzAcc_PlayFx(unsigned int fx, int repeat)
{
	return core->audio->PlayFx(fx, repeat);
}

DECLDIR void EzAcc_SetMusicVolume(int new_volume)
{
	core->audio->SetMusicVolume(new_volume);
}

DECLDIR void EzAcc_SetFXVolume(int new_volume)
{
	core->audio->SetFXVolume(new_volume);
}

DECLDIR EzAcc_Dictionary* EzAcc_GetDictionary()
{
	return core->language->GetDictionary();
}

DECLDIR void EzAcc_ChangeLanguage(char* new_language)
{
	core->language->ChangeCurrentLanguage(new_language);
}

DECLDIR const char* EzAcc_GetWordFromKey(char* key)
{
	return core->language->GetWordFromKey(key);
}

DECLDIR const char* EzAcc_GetCurrentLanguage()
{
	return core->language->GetCurrentLanguage();
}

DECLDIR void Function(void)
{

}
