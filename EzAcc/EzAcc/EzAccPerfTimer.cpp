#include "pch.h"

// ----------------------------------------------------
// EzAccPerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "EzAccPerfTimer.h"
#include "SDL\include\SDL_timer.h"

unsigned __int64 EzAccPerfTimer::frequency = 0;

// ---------------------------------------------
EzAccPerfTimer::EzAccPerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void EzAccPerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double EzAccPerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
unsigned __int64 EzAccPerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}