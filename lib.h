#pragma once

#include "V5SharedLibrary.h"

// Is dll project uses this as header file ?
// or is it executable project loading this library statically
#if defined(DYNAMIC_LIB) || defined(STATIC_LOAD)
EXPORTED std::string hello();
#endif



class testInterface
{
public:
	EXPORTED_MEMBER virtual int sum(int x, int y) = 0;
};



// Is dll project uses this as header file ?
// Or is it executable project loading this library statically
#if defined(DYNAMIC_LIB) || defined(STATIC_LOAD)
class testClass : public testInterface
{
public:
	EXPORTED_MEMBER virtual int sum(int x, int y);
};

#endif


EXPORTED testInterface* CreateTestClass();
