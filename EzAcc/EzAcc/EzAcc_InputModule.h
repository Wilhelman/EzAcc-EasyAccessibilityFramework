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


#ifndef _EZACC_INPUTMODULE_H_
#define _EZACC_INPUTMODULE_H_

#include "EzAcc_Defines.h"
#include "EzAcc_Module.h"

#include "SDL/include/SDL_gamecontroller.h"

enum EzAcc_EventWindow
{
	EZACC_WE_QUIT = 0,
	EZACC_WE_HIDE = 1,
	EZACC_WE_SHOW = 2,
	EZACC_WE_COUNT
};

enum EzAcc_KeyState
{
	EZACC_KEY_IDLE = 0,
	EZACC_KEY_DOWN,
	EZACC_KEY_REPEAT,
	EZACC_KEY_UP
};

enum EzAcc_GamepadState
{
	EZACC_PAD_BUTTON_IDLE = 0,
	EZACC_PAD_BUTTON_DOWN,
	EZACC_PAD_BUTTON_REPEAT,
	EZACC_PAD_BUTTON_KEY_UP
};

struct EzAcc_Gamepad {

	EzAcc_GamepadState A = EZACC_PAD_BUTTON_IDLE;
	EzAcc_GamepadState B;
	EzAcc_GamepadState Y;
	EzAcc_GamepadState X;

	EzAcc_GamepadState CROSS_UP;
	EzAcc_GamepadState CROSS_DOWN;
	EzAcc_GamepadState CROSS_LEFT;
	EzAcc_GamepadState CROSS_RIGHT;

};

class EzAcc_InputModule : public EzAcc_Module
{

public:

	EzAcc_InputModule();

	// Destructor
	virtual ~EzAcc_InputModule();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(EzAcc_EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	EzAcc_KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	EzAcc_KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Get mouse / axis position
	void GetMousePosition(int& x, int& y);
	void GetWorldMousePosition(int& x, int& y);
	void GetMouseMotion(int& x, int& y);

	EzAcc_Gamepad gamepad;
	void buttonForGamepad();

private:
	bool			windowEvents[EZACC_WE_COUNT];
	EzAcc_KeyState* keyboard = nullptr;
	EzAcc_KeyState	mouse_buttons[EZACC_NUM_MOUSE_BUTTONS];
	int				mouse_motion_x = 0;
	int				mouse_motion_y = 0;
	int				mouse_x = 0;
	int				mouse_y = 0;

	SDL_GameController* controller;

};

#endif