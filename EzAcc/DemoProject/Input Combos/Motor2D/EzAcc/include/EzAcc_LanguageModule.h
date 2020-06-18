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


#ifndef _EZACC_LANGUAGEMODULE_H_
#define _EZACC_LANGUAGEMODULE_H_

#include "EzAcc_Defines.h"
#include "EzAcc_Module.h"

#include "p2SString.h"
#include "p2DynArray.h"


struct EzAcc_Dictionary
{
	p2SString press_space_btn;
	p2SString new_game_btn;
	p2SString continue_btn;
	p2SString credits_btn;
	p2SString music_volume;
	p2SString fx_volume;
	p2SString cap_to;
	p2SString language_option;

	p2SString quit_lan;
	p2SString save_lan;
	p2SString load_lan;
	p2SString main_menu_lan;
};

class EzAcc_LanguageModule : public EzAcc_Module
{

public:

	EzAcc_LanguageModule();

	// Destructor
	virtual ~EzAcc_LanguageModule();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load languages file
	pugi::xml_node LoadLanguages(pugi::xml_document&) const;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void ChangeCurrentLanguage(p2SString new_language);

	p2SString GetWordFromKey(p2SString key);

	EzAcc_Dictionary GetDictionary()const;

public:
	p2SString current_language;
	p2DynArray<p2SString> posible_languages;

private:
	EzAcc_Dictionary dictionary;
};

#endif