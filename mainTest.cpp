
#include <string>
#include <iostream>

#include "V5SharedLibrary.h"

#include "lib.h"


int main()
{
#ifdef STATIC_LOAD

	// in static loading, shared library with static library is used to link shared library, static library is used as pointers to point into shared library, so that the class declaration is required
	testClass* tt = new testClass;

	// this function statically loaded
	std::cout << hello() << std::endl;
#else

	// in dynamic linking you need to load the file first (*.dll, *.so or *.dylib) or just leave it without extension
	V5SharedLibrary lib("testLib");

	// create pointer to function "string hello()" and cast the pointer
	std::string (*hello)() = (std::string (*)())lib.GetFunction("hello");


	// create pointer to function "testInterface* CreateTestClass()" and cast the pointer
	// notice that interface is used instead of class, you cannot send use class due to platform independent class layouts <padding bytes..>
	testInterface* (*CreateTestInterface)() = (testInterface* (*)())lib.GetFunction("CreateTestClass");
	// call the (loaded) function
	testInterface* tt = CreateTestInterface();

	// this function is dynamically loaded
	std::cout << hello() << std::endl;
#endif

	std::cout << tt->sum(33, 11) << std::endl;


	return 0;
}
