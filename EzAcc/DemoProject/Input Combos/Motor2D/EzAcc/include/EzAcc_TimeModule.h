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


#ifndef _EZACC_TIMEMODULE_H_
#define _EZACC_TIMEMODULE_H_

#include "EzAcc_Defines.h"
#include "EzAcc_Module.h"
#include "EzAccTimer.h"
#include "EzAccPerfTimer.h"


struct EzAcc_FormatHour
{
	unsigned int hours = 0;
	unsigned int min = 0;
	unsigned int sec = 0;
};

class EzAcc_TimeModule : public EzAcc_Module
{

public:

	EzAcc_TimeModule();

	// Destructor
	virtual ~EzAcc_TimeModule();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetTimeScale(float new_time_scale);

	float GetDT();

	float GetRealTime();

	float GetGameTime();

	EzAcc_FormatHour GetGameTimeFormatHour();

	EzAcc_FormatHour GetRealTimeFormatHour();

private:
	EzAccPerfTimer			perf_timer;
	EzAccTimer				simple_timer;

	EzAccPerfTimer			ptimer;
	uint64					frame_count = 0;
	EzAccTimer				startup_time;
	float					dt = 0.0f;
	float					real_time = 0.0f;
	float					game_time = 0.0f;
	float					real_time_secs = 0.0f;
	float					game_time_secs = 0.0f;

	float time_scale = 1.0f;
};

#endif