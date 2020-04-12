#include "pch.h"

#define DLL_EXPORT 

#include "EzAcc.h"

DECLDIR bool EzAcc_Init()
{
	bool ret = false;
	core = new EzAcc_Core();
	(core != NULL) ?  ret = true : ret = false;
	return ret;
}

DECLDIR bool EzAcc_Awake()
{
	return core->Awake();
}

DECLDIR bool EzAcc_Start()
{
	return core->Start();
}

DECLDIR bool EzAcc_PreUpdate()
{
	return core->PreUpdate();
}

DECLDIR bool EzAcc_Update()
{
	return core->Update();
}

DECLDIR bool EzAcc_AllUpdate()
{
	return core->AllUpdate();
}

DECLDIR bool EzAcc_PostUpdate()
{
	return core->PostUpdate();
}

DECLDIR bool EzAcc_CleanUp()
{
	return core->CleanUp();
}

DECLDIR EzAcc_KeyState EzAcc_GetKey(int id)
{
	return core->input->GetKey(id);
}

DECLDIR void EzAcc_SetTimeBetweenInputs(unsigned int new_time)
{
	if (core->input && core->input != NULL) {
		core->input->time_between_inputs = new_time;
	}
	else {
		LOG("EzAcc: ERROR using input module, did you used \"EzAcc_Init\" to initialise the module?");
	}
}

DECLDIR unsigned int EzAcc_GetTimeBetweenInputs()
{
	if (core->input && core->input != NULL) {
		return core->input->time_between_inputs;
	}
	else {
		LOG("EzAcc: ERROR using input module, did you used \"EzAcc_Init\" to initialise the module?");
	}
}

DECLDIR int EzAcc_BindKey()
{
	if (core->input && core->input != NULL) {
		return core->input->BindKey();
	}
	else {
		LOG("EzAcc: ERROR using input module, did you used \"EzAcc_Init\" to initialise the module?");
	}
}

DECLDIR void Function(void)
{

}
