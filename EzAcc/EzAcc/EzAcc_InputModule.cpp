#include "pch.h"
#include "EzAcc_InputModule.h"

#include "EzAcc_Log.h"

#include "SDL/include/SDL.h"

EzAcc_InputModule::EzAcc_InputModule()
{
	LOG("EzAcc: Generating EzAcc_InputModule ...");
	keyboard = new EzAcc_KeyState[EZACC_MAX_KEYS];
	memset(keyboard, EZACC_KEY_IDLE, sizeof(EzAcc_KeyState) * EZACC_MAX_KEYS);
	memset(mouse_buttons, EZACC_KEY_IDLE, sizeof(EzAcc_KeyState) * EZACC_NUM_MOUSE_BUTTONS);
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

	LOG("EzAcc: Init the controller (search and asign)");
	controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			/*SDL_Joystick* gGameController = SDL_JoystickOpen(0);
			//controllerHaptic = SDL_HapticOpenFromJoystick(gGameController);
			if (controller) {
				//controllerHaptic = SDL_HapticOpen(i);
				if (controllerHaptic == NULL)
				{
					LOG("EzAcc: WARNING - Controller does not support haptics! SDL Error: %s\n", SDL_GetError());
				}
				else
				{
					if (SDL_HapticRumbleInit(controllerHaptic) < 0)
					{
						LOG("EzAcc: WARNING - Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
					}
				}
				break;
			}*/
		}
	}

	// test
		//Load joystick
	SDL_Joystick* gGameController = SDL_JoystickOpen(0);
	LOG("EzAcc : 01");
	if (gGameController == NULL)
	{
		LOG("EzAcc - Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		LOG("EzAcc : 02");
		//Get controller haptic device
		controllerHaptic = SDL_HapticOpenFromJoystick(gGameController);
		if (controllerHaptic == NULL)
		{
			LOG("EzAcc - Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get initialize rumble
			int haptic_init_state = SDL_HapticRumbleInit(controllerHaptic);
			if (haptic_init_state < 0)
			{
				LOG("EzAcc - Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
			}
			LOG("EzAcc : haptic_init_state %i", haptic_init_state);
		}
	}
	LOG("EzAcc : 04");

	return ret;
}

 bool EzAcc_InputModule::Start()
 {
	 SDL_StopTextInput();
	 return true;
 }

 bool EzAcc_InputModule::PreUpdate()
 {
	 current_time = SDL_GetTicks();

	 static SDL_Event event;

	 Uint8* keys = (Uint8*)SDL_GetKeyboardState(NULL);

	 for (int i = 0; i < macros.size(); i++)
	 {
		 EzAcc_Macro* tmp_macro = macros.at(i);
		 if (tmp_macro->key == keys[i]) {
			 for (int j = 0; j < tmp_macro->key_effects.size(); j++)
			 {
				 keys[tmp_macro->key_effects[j]] = 1;
			 }
		 }
	 }

	 for (int i = 0; i < EZACC_MAX_KEYS; ++i)
	 {
		 if (keys[i] == 1)
		 {
			 
			 if (keyboard[i] == EZACC_KEY_IDLE)
				 keyboard[i] = EZACC_KEY_DOWN;
			 else
				 keyboard[i] = EZACC_KEY_REPEAT;
		 }
		 else
		 {
			 if (keyboard[i] == EZACC_KEY_REPEAT || keyboard[i] == EZACC_KEY_DOWN)
				 keyboard[i] = EZACC_KEY_UP;
			 else
				 keyboard[i] = EZACC_KEY_IDLE;
		 }
	 }

	 for (int i = 0; i < EZACC_NUM_MOUSE_BUTTONS; ++i)
	 {
		 if (mouse_buttons[i] == EZACC_KEY_DOWN)
			 mouse_buttons[i] = EZACC_KEY_REPEAT;

		 if (mouse_buttons[i] == EZACC_KEY_UP)
			 mouse_buttons[i] = EZACC_KEY_IDLE;
	 }

	 buttonForGamepad();

	 while (SDL_PollEvent(&event) != 0)
	 {
		 switch (event.type)
		 {
		 case SDL_QUIT:
			 windowEvents[EZACC_WE_QUIT] = true;
			 break;

		 case SDL_WINDOWEVENT:
			 switch (event.window.event)
			 {
				 //case SDL_WINDOWEVENT_LEAVE:
			 case SDL_WINDOWEVENT_HIDDEN:
			 case SDL_WINDOWEVENT_MINIMIZED:
			 case SDL_WINDOWEVENT_FOCUS_LOST:
				 windowEvents[EZACC_WE_HIDE] = true;
				 break;

				 //case SDL_WINDOWEVENT_ENTER:
			 case SDL_WINDOWEVENT_SHOWN:
			 case SDL_WINDOWEVENT_FOCUS_GAINED:
			 case SDL_WINDOWEVENT_MAXIMIZED:
			 case SDL_WINDOWEVENT_RESTORED:
				 windowEvents[EZACC_WE_SHOW] = true;
				 break;
			 }
			 break;

		 case SDL_MOUSEBUTTONDOWN:
			 mouse_buttons[event.button.button - 1] = EZACC_KEY_DOWN;
			 //LOG("Mouse button %d down", event.button.button-1);
			 break;

		 case SDL_MOUSEBUTTONUP:
			 mouse_buttons[event.button.button - 1] = EZACC_KEY_UP;
			 //LOG("Mouse button %d up", event.button.button-1);
			 break;

		 /*case SDL_MOUSEMOTION: // TODOG cannot access to the window for now
			 int scale = App->win->GetScale();
			 mouse_motion_x = event.motion.xrel / scale;
			 mouse_motion_y = event.motion.yrel / scale;
			 mouse_x = event.motion.x / scale;
			 mouse_y = event.motion.y / scale;
			 //LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			 break;*/
		 }
	 }

	 return true;
 }

 bool EzAcc_InputModule::CleanUp()
 {
	 LOG("EzAcc: Quitting SDL event subsystem");
	 SDL_HapticClose(controllerHaptic);
	 SDL_QuitSubSystem(SDL_INIT_EVENTS);
	 return true;
 }

 bool EzAcc_InputModule::GetWindowEvent(EzAcc_EventWindow ev)
 {
	 return windowEvents[ev];
 }

 int KeyBind(SDL_Event& Event)
 {
	 while (1) // keep running till user taps a button ...
	 {
		 while (SDL_PollEvent(&Event))
		 {
			 switch (Event.type)
			 {
			 case SDL_KEYDOWN:
				 LOG("Keybound to num: %i", Event.key.keysym.sym);
				 return Event.key.keysym.sym;
				 break;
			 };
		 }
	 }
	 return 0;
 }

 int EzAcc_InputModule::BindKey()
 {
	 SDL_Event Event;
	 return KeyBind(Event);
 }

 bool EzAcc_InputModule::PerformRumble(float rumble_power, int rumble_time)
 {
	 /*
	 RumblePlay info:
	 0.0f-1.0f % of power
	 time: ms p.e. 500 = 0.5 sec of rumble
	 */
	 int haptic_state = SDL_HapticRumblePlay(controllerHaptic, rumble_power, rumble_time);
	 
	 if (haptic_state != 0)
	 {
		 LOG("EzAcc Warning: Unable to play rumble! %s\n", SDL_GetError());
		 return false;
	 }
	 return true;
 }

 void EzAcc_InputModule::GetMousePosition(int& x, int& y)
 {
	 x = mouse_x;
	 y = mouse_y;
 }

 void EzAcc_InputModule::GetWorldMousePosition(int& x, int& y)
 {
	 x = mouse_motion_x;
	 y = mouse_motion_y;
 }

 void EzAcc_InputModule::GetMouseMotion(int& x, int& y)
 {
	 // TODOG I can't do this for now ...
	 /*x = App->render->ScreenToWorld(mouse_x, mouse_y).x;
	 y = App->render->ScreenToWorld(mouse_x, mouse_y).y;*/
 }
 /*
 int haptic_state = SDL_HapticRumblePlay(controllerHaptic, 0.75, 500);
			 //Play rumble at 75% strenght for 500 milliseconds
			 if (haptic_state != 0)
			 {
				 printf("Warning: Unable to play rumble! %s\n", SDL_GetError());
			 }
			 LOG("EzAcc : Despues de clicar esta es la cosa: %i", haptic_state);
 */
 void EzAcc_InputModule::buttonForGamepad()
 {
	 //BUTTON A
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == 1) {

		 if (gamepad.A == EZACC_PAD_BUTTON_IDLE) {
			 
			 gamepad.A = EZACC_PAD_BUTTON_DOWN;
		 }
		 else
			 gamepad.A = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.A == EZACC_PAD_BUTTON_REPEAT || (gamepad.A == EZACC_PAD_BUTTON_DOWN))
			 gamepad.A = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.A = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON B
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) == 1) {
		 if (gamepad.B == EZACC_PAD_BUTTON_IDLE)
			 gamepad.B = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.B = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.B == EZACC_PAD_BUTTON_REPEAT || (gamepad.B == EZACC_PAD_BUTTON_DOWN))
			 gamepad.B = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.B = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON X
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) == 1) {
		 if (gamepad.X == EZACC_PAD_BUTTON_IDLE)
			 gamepad.X = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.X = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.X == EZACC_PAD_BUTTON_REPEAT || (gamepad.X == EZACC_PAD_BUTTON_DOWN))
			 gamepad.X = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.X = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON Y
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y) == 1) {
		 if (gamepad.Y == EZACC_PAD_BUTTON_IDLE)
			 gamepad.Y = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.Y = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.Y == EZACC_PAD_BUTTON_REPEAT || (gamepad.Y == EZACC_PAD_BUTTON_DOWN))
			 gamepad.Y = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.Y = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON DPAD UP
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
		 if (gamepad.CROSS_UP == EZACC_PAD_BUTTON_IDLE)
			 gamepad.CROSS_UP = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.CROSS_UP = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.CROSS_UP == EZACC_PAD_BUTTON_REPEAT || (gamepad.CROSS_UP == EZACC_PAD_BUTTON_DOWN))
			 gamepad.CROSS_UP = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.CROSS_UP = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON DPAD DOWN
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
		 if (gamepad.CROSS_DOWN == EZACC_PAD_BUTTON_IDLE)
			 gamepad.CROSS_DOWN = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.CROSS_DOWN = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.CROSS_DOWN == EZACC_PAD_BUTTON_REPEAT || (gamepad.CROSS_DOWN == EZACC_PAD_BUTTON_DOWN))
			 gamepad.CROSS_DOWN = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.CROSS_DOWN = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON DPAD LEFT
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
		 if (gamepad.CROSS_LEFT == EZACC_PAD_BUTTON_IDLE)
			 gamepad.CROSS_LEFT = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.CROSS_LEFT = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.CROSS_LEFT == EZACC_PAD_BUTTON_REPEAT || (gamepad.CROSS_LEFT == EZACC_PAD_BUTTON_DOWN))
			 gamepad.CROSS_LEFT = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.CROSS_LEFT = EZACC_PAD_BUTTON_IDLE;
	 }

	 //BUTTON DPAD UP
	 if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
		 if (gamepad.CROSS_RIGHT == EZACC_PAD_BUTTON_IDLE)
			 gamepad.CROSS_RIGHT = EZACC_PAD_BUTTON_DOWN;
		 else
			 gamepad.CROSS_RIGHT = EZACC_PAD_BUTTON_REPEAT;
	 }
	 else
	 {
		 if (gamepad.CROSS_RIGHT == EZACC_PAD_BUTTON_REPEAT || (gamepad.CROSS_RIGHT == EZACC_PAD_BUTTON_DOWN))
			 gamepad.CROSS_RIGHT = EZACC_PAD_BUTTON_KEY_UP;
		 else
			 gamepad.CROSS_RIGHT = EZACC_PAD_BUTTON_IDLE;
	 }
 }

 int EzAcc_InputModule::SetMacroForKey(EzAcc_Macro new_macro)
 {
	 EzAcc_Macro* tmp_macro = new EzAcc_Macro();
	 tmp_macro->key = new_macro.key;
	 tmp_macro->key_effects = new_macro.key_effects;
	 
	 macros.push_back(tmp_macro);
	 int index_to_return = macros.size() - 1;
	 LOG("EzAcc: SetMacroForKey - Returning index: %i", index_to_return);
	 return index_to_return;
 }

 bool EzAcc_InputModule::RemoveMacro(int index_to_remove)
 {
	 if (macros.size() > index_to_remove) {
		 macros.erase(macros.begin() + index_to_remove);
		 return true;
	 }
	 return false;
 }
