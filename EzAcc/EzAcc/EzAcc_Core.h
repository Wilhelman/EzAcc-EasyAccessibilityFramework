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


#ifndef _EZACC_CORE_H_
#define _EZACC_CORE_H_

#include <list>
#include "EzAcc_Module.h"
#include "pugixml.hpp"
#include "EzAccTimer.h"
#include "EzAccPerfTimer.h"

// Modules
class EzAcc_InputModule;

class EzAcc_Core
{
public:

	// Constructor
	EzAcc_Core();

	// Destructor
	virtual ~EzAcc_Core();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool AllUpdate();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(EzAcc_Module* module);

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool Update();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	void SetTimeScale(float new_time_scale);

	float GetDT();

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	

	

public:

	// Modules
	EzAcc_InputModule* input = nullptr;

private:

	std::list<EzAcc_Module*>	modules;
	bool				all_modules_loaded = false;

	// TODOG : Time dt stuff ...
	/*
	ctPerfTimer			perf_timer;
	ctTimer				simple_timer;
	ctPerfTimer			ptimer;
	uint64				frame_count = 0;
	uint				cap = 0u;
	ctTimer				startup_time;
	float				dt = 0.0f;
	*/
	EzAccPerfTimer			perf_timer;
	EzAccTimer				simple_timer;

	EzAccPerfTimer			ptimer;
	uint64				frame_count = 0;
	uint				cap = 0u;
	EzAccTimer				startup_time;
	float				dt = 0.0f;
	int					capped_ms = -1;
	
	float time_scale = 1.0f;

};

extern EzAcc_Core* Core;

#endif