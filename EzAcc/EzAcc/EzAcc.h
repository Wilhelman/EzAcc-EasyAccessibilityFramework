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


#ifndef _EZACC_H_
#define _EZACC_H_

#include "EzAcc_Defines.h"

#include "EzAcc_Core.h"

// Modules
#include "EzAcc_Module.h"
#include "EzAcc_InputModule.h"
#include "EzAcc_TimeModule.h"
#include "EzAcc_VisionModule.h"
#include "EzAcc_AudioModule.h"
#include "EzAcc_LanguageModule.h"

static EzAcc_Core* core;

extern "C"
{
	// ---------------- CORE ---------------- //
	/*EzAcc Init
	An instance of the Core module is generated. Initializing the library.
	*/
	DECLDIR bool EzAcc_Init();
	/*EzAcc Awake
	This call starts the serialization system, reading the last saved data from the configuration file. 
	It is also responsible for calling the rest of the modules, passing each of their last saved settings 
	through parameters so that they are initialized correctly.
	*/
	DECLDIR bool EzAcc_Awake();
	/*EzAcc Start
	This function will initialize some extra functions, variables or libraries in some of the modules. 
	P.e. in the input module we will initialize the SDL library.
	*/
	DECLDIR bool EzAcc_Start();
	/*EzAcc PreUpdate
	It is responsible for calling the PreUpdate function of the rest of the modules. 
	For example, user input is collected in the PreUpdate of the Input module. 
	In this way there may be ordered logic later. (In the Update and PostUpdate methods)
	*/
	DECLDIR bool EzAcc_PreUpdate();
	/*EzAcc Update
	*/
	DECLDIR bool EzAcc_Update();
	/*EzAcc AllUpdate
	It is responsible for calling the Update function of the other modules.
	*/
	DECLDIR bool EzAcc_AllUpdate();
	/*EzAcc PostUpdate
	TODOG
	*/
	DECLDIR bool EzAcc_PostUpdate();
	/*EzAcc CleanUp
	TODOG
	*/
	DECLDIR bool EzAcc_CleanUp();


	// ---------------- Motor (Mobility/Control) ---------------- //

	/*
	EzAcc_InputModule GetKey
	EzAcc_KeyState Inaccesible if that ...
	TODOG
	*/
	DECLDIR EzAcc_KeyState EzAcc_GetKey(int id);

	/*
	EzAcc_InputModule SetTimeBetweenInputs
	TODOG
	*/
	DECLDIR void EzAcc_SetTimeBetweenInputs(unsigned int new_time);

	/*
	EzAcc_InputModule GetTimeBetweenInputs
	TODOG
	*/
	DECLDIR unsigned int EzAcc_GetTimeBetweenInputs();

	/*
	EzAcc_InputModule BindKey
	int = SDL_KeyCode
	TODOG
	*/
	DECLDIR int EzAcc_BindKey();

	/*
	EzAcc_InputModule GenerateNewMacro
	int = index macro
	TODOG
	*/
	DECLDIR int EzAcc_GenerateNewMacro(EzAcc_Macro new_macro);

	/*
	EzAcc_InputModule RemoveMacro
	int = index macro
	TODOG
	*/
	DECLDIR bool EzAcc_RemoveMacro(int index);

	/*
	EzAcc_InputModule RemoveMacro
	RumblePlay info :0.0f - 1.0f % of power | time : ms p.e. 500 = 0.5 sec of rumble
	*/
	DECLDIR bool EzAcc_PerformRumble(float rumble_power, int rumble_time);


	// ---------------- Cognitive (Memory/Information Processing) ---------------- //
	/*
	EzAcc_Core SetDTTimeScale
	int = index macro
	TODOG
	*/
	DECLDIR void EzAcc_SetDTTimeScale(float timescale);

	/*
	EzAcc_Core GetDT
	int = index macro
	TODOG
	*/
	DECLDIR float EzAcc_GetDT();

	/*
	EzAcc_Core GetGameTime
	int = index macro
	TODOG
	*/
	DECLDIR float EzAcc_GetGameTime();

	/*
	EzAcc_Core GetGameTimeHourFormat
	int = index macro
	TODOG
	*/
	DECLDIR EzAcc_FormatHour EzAcc_GetGameTimeHourFormat();

	/*
	EzAcc_Core GetRealtime
	int = index macro
	TODOG
	*/
	DECLDIR float EzAcc_GetRealtime();

	/*
	EzAcc_Core GetRealtimeHourFormat
	int = index macro
	TODOG
	*/
	DECLDIR EzAcc_FormatHour EzAcc_GetRealtimeHourFormat();


	// ---------------- Vision (Pixel / texture modification) ---------------- //

	/*
	EzAcc_VisionModule PerformPixelModification
	TODOG
	*/
	DECLDIR void EzAcc_PerformPixelModification(bool pixel_mod_active);

	/*
	EzAcc_VisionModule ModulateTextureColor
	TODOG
	*/
	DECLDIR void EzAcc_ModulateTextureColor(SDL_Texture* texture, SDL_Color color);

	/*
	EzAcc_VisionModule ProcessTexture
	TODOG
	*/
	DECLDIR void EzAcc_ProcessTexture(SDL_Texture* texture, SDL_Surface* surface);

	/*
	EzAcc_VisionModule AddNewPixelMod
	TODOG
	*/
	DECLDIR void EzAcc_AddNewPixelMod(EzAcc_PixelModificator new_pixel_mod);

	/*
	EzAcc_VisionModule ClearPixelMods
	TODOG
	*/
	DECLDIR void EzAcc_ClearPixelMods();

	// ---------------- Audio (Music/Fx and extra info about sounds (Descriptive labels)) ---------------- //
	/*
	EzAcc_AudioModule PlayMusic
	TODOG
	*/
	DECLDIR bool EzAcc_PlayMusic(const char* path, float fade_time = EZACC_DEFAULT_MUSIC_FADE_TIME);

	/*
	EzAcc_AudioModule StopMusic
	TODOG
	*/
	DECLDIR bool EzAcc_StopMusic();

	/*
	EzAcc_AudioModule LoadFx
	TODOG
	*/
	DECLDIR unsigned int EzAcc_LoadFx(const char* path, const char* new_embedded_language);

	/*
	EzAcc_AudioModule PlayFx
	TODOG
	*/
	DECLDIR p2SString EzAcc_PlayFx(unsigned int fx, int repeat = 0);

	/*
	EzAcc_AudioModule SetMusicVolume
	TODOG
	*/
	DECLDIR void EzAcc_SetMusicVolume(int new_volume);

	/*
	EzAcc_AudioModule SetFXVolume
	TODOG
	*/
	DECLDIR void EzAcc_SetFXVolume(int new_volume);

	// -------------------------------- Language -------------------------------- //
	/*
	EzAcc_LanguageModule GetDictionary
	TODOG
	*/
	DECLDIR EzAcc_Dictionary EzAcc_GetDictionary();

	/*
	EzAcc_LanguageModule ChangeLanguage
	TODOG
	*/
	DECLDIR void EzAcc_ChangeLanguage(p2SString new_language);

	/*
	EzAcc_LanguageModule ChangeLanguage
	TODOG
	*/
	DECLDIR p2SString EzAcc_GetWordFromKey(p2SString key);

	
	DECLDIR void Function(void);
}

#endif