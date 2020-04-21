#include "pch.h"

// ----------------------------------------------------
// EzAccTimer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "EzAccTimer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
EzAccTimer::EzAccTimer()
{
}

// ---------------------------------------------
void EzAccTimer::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------

void EzAccTimer::SetStartTime(unsigned __int32 time) {
	started_at = SDL_GetTicks() + time;
}

// ---------------------------------------------
unsigned __int32 EzAccTimer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float EzAccTimer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}