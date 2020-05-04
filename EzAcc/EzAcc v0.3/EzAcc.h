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
#include "EzAcc_Input.h"
#include "EzAcc_Log.h"
#include "EzAcc_Core.h"

// Modules
#include "EzAcc_Module.h"
#include "EzAcc_InputModule.h"


static EzAcc_Core* core;

extern "C"
{
	// ---------------- CORE ---------------- //
	/*EzAcc Init
	Initialise TODOG .....
	*/
	DECLDIR bool EzAcc_Init();
	/*EzAcc Awake
	TODOG
	*/
	DECLDIR bool EzAcc_Awake();
	/*EzAcc Start
	TODOG
	*/
	DECLDIR bool EzAcc_Start();
	/*EzAcc PreUpdate
	TODOG
	*/
	DECLDIR bool EzAcc_PreUpdate();
	/*EzAcc Update
	TODOG
	*/
	DECLDIR bool EzAcc_Update();
	/*EzAcc AllUpdate
	TODOG
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


	// ---------------- INPUT MODULE ---------------- //

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

	DECLDIR void Function(void);
}

#endif