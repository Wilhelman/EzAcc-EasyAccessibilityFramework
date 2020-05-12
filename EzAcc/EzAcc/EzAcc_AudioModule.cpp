#include "pch.h"
#include "EzAcc_AudioModule.h"

#include "EzAcc_Log.h"


EzAcc_AudioModule::EzAcc_AudioModule()
{
	LOG("EzAcc: Generating EzAcc_AudioModule ...");
}

// Destructor
EzAcc_AudioModule::~EzAcc_AudioModule()
{

}

// Called before render is available
bool EzAcc_AudioModule::Awake(pugi::xml_node&)
{
	bool ret = true;


	return ret;
}

bool EzAcc_AudioModule::Start()
{
	return true;
}

bool EzAcc_AudioModule::PreUpdate()
{


	return true;
}

bool EzAcc_AudioModule::PostUpdate()
{

	return true;
}

bool EzAcc_AudioModule::CleanUp()
{
	return true;
}
