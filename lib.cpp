#include "lib.h"
#include "V5SharedLibrary.h"

std::string hello()
{
#if _WIN32
	return "hello world, this function called from DLL file";
#elif __APPLE__
	return "hello world, this function called from dylib file";
#else
	return "hello world, this function called from so file";
#endif
}



int testClass::sum(int x, int y)
{
	return x + y;
}



testInterface* CreateTestClass()
{
	return new testClass;
}
