#include "pch.h"
#include "EzAcc_InputModule.h"

#include "EzAcc_Log.h"

#include "SDL/include/SDL.h"

EzAcc_InputModule::EzAcc_InputModule()
{
	keyboard = new EzAcc_KeyState[EZACC_MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(EzAcc_KeyState) * EZACC_MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(EzAcc_KeyState) * EZACC_NUM_MOUSE_BUTTONS);
}

// Destructor
EzAcc_InputModule::~EzAcc_InputModule()
{
	delete[] keyboard;
}

// Called before render is available
 bool EzAcc_InputModule::Awake(pugi::xml_node&)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	LOG("Init the controller (search and asign)");
	controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			if (controller) {
				break;
			}
		}
	}

	return ret;
}

 bool EzAcc_InputModule::Start()
 {
	 return false;
 }

 bool EzAcc_InputModule::PreUpdate()
 {
	 return false;
 }

 bool EzAcc_InputModule::CleanUp()
 {
	 return false;
 }

 bool EzAcc_InputModule::GetWindowEvent(EzAcc_EventWindow ev)
 {
	 return false;
 }

 void EzAcc_InputModule::GetMousePosition(int& x, int& y)
 {
 }

 void EzAcc_InputModule::GetWorldMousePosition(int& x, int& y)
 {
 }

 void EzAcc_InputModule::GetMouseMotion(int& x, int& y)
 {
 }

 void EzAcc_InputModule::buttonForGamepad()
 {
 }
