# V5SharedLibrary
Cross-platform Shared Library wrapper/helper

All you need to do is to add both V5SharedLibrary.h and V5SharedLibrary.cpp to your project

# Example writing library
Header:
```cpp

#pragma once

#include "V5SharedLibrary.h"

EXPORTED std::string hello();

```

CPP:
```cpp

#include "lib.h"

std::string hello()
{
	return "hello world, this function called from shared library";
}

```

# Executable Example
```cpp Header:

#include <string>
#include <iostream>

#include "V5SharedLibrary.h"

#include "lib.h"


int main()
{
  // in dynamic linking you need to load the file first (*.dll, *.so or *.dylib) or just leave it without extension
  V5SharedLibrary lib("testLib");
  if (!lib.LoadFailed())
  {
    // create pointer to function "string hello()" and cast the pointer
    std::string (*hello)() = (std::string (*)())lib.GetFunction("hello");

    if (hello != NULL)
    {
      // this function is dynamically loaded
      std::cout << hello() << std::endl;
    }
    else
    {
      std::cout << "Failed to find function" << std::endl;
    }
  }
  else
  {
    std::cout << "Failed to find shared library file, probably you didn't build it or its in other folder" << std::endl;
  }
  return 0;
}
```

# Advance Usage:
testEXE contains a Dynamically loaded library
testEXE2 contains a Statically loaded library
Both with ability to create class
