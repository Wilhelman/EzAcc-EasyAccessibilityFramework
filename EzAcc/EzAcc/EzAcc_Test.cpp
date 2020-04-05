#include "pch.h"
#include <iostream>

#define DLL_EXPORT 

#include "EzAcc_Test.h"

DECLDIR int Add(int a, int b)
{
	int c = a + b;
	return c;
}

DECLDIR void Function(void)
{
	std::cout << "DLL Called!" << std::endl;
}