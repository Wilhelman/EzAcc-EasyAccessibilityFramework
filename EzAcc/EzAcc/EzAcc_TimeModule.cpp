#include "pch.h"
#include "EzAcc_TimeModule.h"

#include "EzAcc_Log.h"


EzAcc_TimeModule::EzAcc_TimeModule()
{
	LOG("EzAcc: Generating EzAcc_TimeModule ...");
}

// Destructor
EzAcc_TimeModule::~EzAcc_TimeModule()
{

}

// Called before render is available
bool EzAcc_TimeModule::Awake(pugi::xml_node&)
{
	bool ret = true;
	

	return ret;
}

bool EzAcc_TimeModule::Start()
{
	return true;
}

bool EzAcc_TimeModule::PreUpdate()
{
	

	return true;
}

bool EzAcc_TimeModule::PostUpdate()
{

	frame_count++;

	float avg_fps = float(frame_count) / startup_time.ReadSec();

	float seconds_since_startup = simple_timer.Read();

	uint32 current_ms_frame = perf_timer.ReadMs();
	uint32 last_frame_ms = current_ms_frame;
	uint32 frames_on_last_update = 0;

	double framerate = 1000.0f / perf_timer.ReadMs();
	dt = (float)perf_timer.ReadMs() / 1000.f;

	real_time = last_frame_ms / 1000.0f;
	real_time_secs += real_time;
	// Modify DT with user time scale ...
	game_time = real_time * time_scale;
	game_time_secs += game_time;

	return true;
}

bool EzAcc_TimeModule::CleanUp()
{
	return true;
}

void EzAcc_TimeModule::SetTimeScale(float new_time_scale)
{
	time_scale = new_time_scale;
}

float EzAcc_TimeModule::GetDT()
{
	return dt;
}

float EzAcc_TimeModule::GetRealTime()
{
	return real_time_secs;
}

float EzAcc_TimeModule::GetGameTime()
{
	return game_time_secs;
}

EzAcc_FormatHour EzAcc_TimeModule::GetGameTimeFormatHour()
{
	double tmp_time = game_time_secs;

	EzAcc_FormatHour ret_format;
	ret_format.hours = tmp_time / 3600;
	tmp_time = (uint)tmp_time % 3600;
	ret_format.min = tmp_time / 60.0f;
	tmp_time = (uint)tmp_time % 60;
	ret_format.sec = tmp_time;

	return ret_format;
}

EzAcc_FormatHour EzAcc_TimeModule::GetRealTimeFormatHour()
{
	double tmp_time = real_time_secs;

	EzAcc_FormatHour ret_format;
	ret_format.hours = tmp_time / 3600;
	tmp_time = (uint)tmp_time % 3600;
	ret_format.min = tmp_time / 60.0f;
	tmp_time = (uint)tmp_time % 60;
	ret_format.sec = tmp_time;

	return ret_format;
}
