#include "V5SharedLibrary.h"

#if defined _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

const int32_t V5SharedLibrary::MODE_LAZY = 0;
const int32_t V5SharedLibrary::MODE_NOW = 1;


V5SharedLibrary::V5SharedLibrary(const std::string& file, const int32_t& mode)
{
#if defined _WIN32
    loadFailed = false;
    loaded = (mode == MODE_NOW);
    fileName = file;
    if (mode == MODE_NOW)
    {
        handle = LoadLibrary(file.c_str());
        loadFailed = (handle == NULL);
    }
    if (loadFailed)
    {
        size_t found = file.find(".dll"); 
        if (found != std::string::npos)
        {
            if (mode == MODE_NOW)
            {
                handle = LoadLibrary((file + ".dll").c_str());
                loadFailed = (handle == NULL);
            }
        }
    }
#else
    //try same path
    handle = dlopen(file.c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
    loadFailed = (handle == NULL);
    if (loadFailed)
    {
        //try same path with extension
#if __APPLE__
        handle = dlopen((file + ".dylib").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
        handle = dlopen((file + ".so").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif

        loadFailed = (handle == NULL);

		if (loadFailed)
		{
			std::string dir = "";
			std::string fileName = "";

			
			std::size_t found = file.find_last_of("/\\");

            if (found != std::string::npos)
			{
				dir = file.substr(0, found + 1);
			}
			fileName = file.substr(found + 1);

            //try same path with prefix lib
#if __APPLE__
			handle = dlopen((dir + "lib" + fileName).c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
			handle = dlopen((dir + "lib" + fileName).c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif

			loadFailed = (handle == NULL);

			if (loadFailed)
			{
                //try same path with prefix lib and extension
#if __APPLE__
				handle = dlopen((dir + "lib" + fileName + ".dylib").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
				handle = dlopen((dir + "lib" + fileName + ".so").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
				loadFailed = (handle == NULL);
			}
            
            // get application start up directory
            std::string appdir = "";
            {
                char buffer[512];
                char *answer = getcwd(buffer, sizeof(buffer));
                if (answer)
                {
                    appdir = answer + std::string("/");
                }
            }
            
            if (loadFailed)
            {
                // try with start up directory
#if __APPLE__
                handle = dlopen((appdir + fileName).c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
                handle = dlopen((appdir + fileName).c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
                loadFailed = (handle == NULL);
            }
            
            if (loadFailed)
            {
                // try with start up directory with prefix lib
#if __APPLE__
                handle = dlopen((appdir + "lib" + fileName).c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
                handle = dlopen((appdir + "lib" + fileName).c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
                loadFailed = (handle == NULL);
            }
            
            if (loadFailed)
            {
                // try with start up directory with extension
#if __APPLE__
                handle = dlopen((appdir + fileName + ".dylib").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
                handle = dlopen((appdir + fileName + ".so").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
                loadFailed = (handle == NULL);
            }
            
            if (loadFailed)
            {
                // try with start up directory with prefix lib and extension
#if __APPLE__
                handle = dlopen((appdir + "lib" + fileName + ".dylib").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
                handle = dlopen((appdir + "lib" + fileName + ".so").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
                loadFailed = (handle == NULL);
            }
            
            if (loadFailed)
            {
                // try without directory with extension
                // for linux environment variable LD_LIBRARY_PATH is used to search
#if __APPLE__
                handle = dlopen((fileName + ".dylib").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
                handle = dlopen((fileName + ".so").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
                loadFailed = (handle == NULL);
            }
            
            if (loadFailed)
            {
                // try without directory, with prefix lib and extension
                // for linux environment variable LD_LIBRARY_PATH is used to search
#if __APPLE__
                handle = dlopen(("lib" + fileName + ".dylib").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#else
                handle = dlopen(("lib" + fileName + ".so").c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
#endif
                loadFailed = (handle == NULL);
            }
		}
    }
#endif
}

bool V5SharedLibrary::LoadFailed()
{
#if defined _WIN32
    if (!loaded)
    {
        handle = LoadLibrary(fileName.c_str());
        loadFailed = (handle == NULL);
        loaded = true;
        if (loadFailed)
        {
            size_t found = fileName.find(".dll"); 
            if (found != std::string::npos)
            {
                handle = LoadLibrary((fileName + ".dll").c_str());
                loadFailed = (handle == NULL);
            }
        }
    }
#endif
    return loadFailed;
}


void* V5SharedLibrary::GetFunction(const std::string& name)
{
#if defined _WIN32
    if (!loaded)
    {
        handle = LoadLibrary(fileName.c_str());
        loadFailed = (handle == NULL);
        loaded = true;
        if (loadFailed)
        {
            size_t found = fileName.find(".dll"); 
            if (found != std::string::npos)
            {
                handle = LoadLibrary((fileName + ".dll").c_str());
                loadFailed = (handle == NULL);
            }
        }
    }

    if (loadFailed)
    {
        return NULL;
    }

    return GetProcAddress(handle, name.c_str());
#else

    if (loadFailed)
    {
        return NULL;
    }

    return dlsym(handle, name.c_str());
#endif
}

V5SharedLibrary::~V5SharedLibrary()
{
#if defined _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}
