#include "pch.h"
#include "EzAcc_VisionModule.h"

#include "EzAcc_Log.h"

#include "SDL/include/SDL.h"


EzAcc_VisionModule::EzAcc_VisionModule()
{
	EZACC_LOG("EzAcc: Generating EzAcc_VisionModule ...");
}

// Destructor
EzAcc_VisionModule::~EzAcc_VisionModule()
{

}

// Called before render is available
bool EzAcc_VisionModule::Awake(pugi::xml_node&)
{
	bool ret = true;


	return ret;
}

bool EzAcc_VisionModule::Start()
{
	return true;
}

bool EzAcc_VisionModule::PreUpdate()
{

	return true;
}

bool EzAcc_VisionModule::PostUpdate()
{

	return true;
}

bool EzAcc_VisionModule::CleanUp()
{
	return true;
}
