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
	return true; // TODOG
	return core->Awake();
}

DECLDIR bool EzAcc_Update()
{
	return true;// TODOG
}

DECLDIR bool EzAcc_AllUpdate()
{
	return core->Update();
}

DECLDIR bool EzAcc_PostUpdate()
{
	return true; // TODOG
	//return core->PostUp();
}

DECLDIR bool EzAcc_CleanUp()
{
	return core->CleanUp();
}

DECLDIR bool EzAcc_GetKeyIntentoUno()
{
	return false;
}

DECLDIR bool EzAcc_GetKeyIntentoDos(int a)
{
	return true;
}

DECLDIR EzAcc_KeyState EzAcc_GetKey(int id)
{
	return core->input->GetKey(id);
}

DECLDIR void Function(void)
{

}
