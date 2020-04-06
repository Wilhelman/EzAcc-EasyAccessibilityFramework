#include "pch.h"

#define DLL_EXPORT 

#include "EzAcc.h"

DECLDIR bool EzAcc_Init()
{
	bool ret = false;
	EzAcc_Core* core = new EzAcc_Core();
	(core != NULL) ?  ret = true : ret = false;
	return ret;
}

DECLDIR void Function(void)
{

}
