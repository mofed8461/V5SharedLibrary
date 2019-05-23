#include "V5SharedLibrary.h"

#if defined _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif
#if __APPLE__
#include <mach-o/dyld.h>
#endif

const int32_t V5SharedLibrary::MODE_LAZY = 0;
const int32_t V5SharedLibrary::MODE_NOW = 1;

std::vector<std::string> V5SharedLibrary::SearchPaths;
std::vector<std::string> V5SharedLibrary::FileNamePrefixes;
std::vector<std::string> V5SharedLibrary::FileNameExtensions;

static int32_t initMe()
{
    V5SharedLibrary::AddDefaultSearchPaths();
    V5SharedLibrary::AddDefaultFileNamePrefixes();
    V5SharedLibrary::AddDefaultFileNameExtensions();
    return 0;
}
static int32_t _initMe = initMe();


std::string V5SharedLibrary::GetStartUpPath()
{
    std::string appdir = "";
    {
        char buffer[1024];
#if __APPLE__
        uint32_t buffer_size = 1024;
        _NSGetExecutablePath(buffer, &buffer_size);
        appdir = buffer;
        std::size_t found = appdir.find_last_of("/\\");
        
        if (found != std::string::npos)
        {
            appdir = appdir.substr(0, found + 1);
        }
#else
        appdir = getcwd(buffer, sizeof(buffer)) + std::string("/");
#endif
    }
    return appdir;
}

void V5SharedLibrary::AddDefaultSearchPaths()
{
    SearchPaths.push_back(GetStartUpPath());
}

void V5SharedLibrary::AddDefaultFileNamePrefixes()
{
#if !defined(_WIN32)
    FileNamePrefixes.push_back("lib");
#endif
}

void V5SharedLibrary::AddDefaultFileNameExtensions()
{
#if __APPLE__
    FileNameExtensions.push_back(".dylib");
#elif defined _WIN32
    FileNameExtensions.push_back(".dll");
#else
    FileNameExtensions.push_back(".so");
#endif
}

void V5SharedLibrary::load(const std::string& file, const int32_t& mode)
{
    std::vector<std::string> searchPaths = SearchPaths;
    std::vector<std::string> prefixes = FileNamePrefixes;
    std::vector<std::string> extensions = FileNameExtensions;
    
    searchPaths.insert(searchPaths.begin(), "");
    prefixes.insert(prefixes.begin(), "");
    extensions.insert(extensions.begin(), "");
    
    
    std::string dir = "";
    std::string fileName = "";
    
    searchPaths.insert(searchPaths.begin(), dir);
    
    {
        std::size_t found = file.find_last_of("/\\");
        
        if (found != std::string::npos)
        {
            dir = file.substr(0, found + 1);
        }
        fileName = file.substr(found + 1);
    }
    
    loadFailed = false;
#if defined _WIN32
    loaded = (mode == MODE_NOW);
#endif
    
    for (int32_t i = 0; i < searchPaths.size(); ++i)
    {
        for (int32_t j = 0; j < prefixes.size(); ++j)
        {
            for (int32_t k = 0; k < extensions.size(); ++k)
            {
                std::string fullPath = searchPaths[i] + prefixes[j] + fileName + extensions[k];
#if defined _WIN32
                if (mode == MODE_NOW)
                {
                    
                    handle = LoadLibrary(fullPath.c_str());
                    loadFailed = (handle == NULL);
                }
#else
                handle = dlopen(fullPath.c_str(), mode == MODE_LAZY ? RTLD_LAZY : RTLD_NOW);
                loadFailed = (handle == NULL);
#endif
                if (!loadFailed)
                    break;
            }
            if (!loadFailed)
                break;
        }
        if (!loadFailed)
            break;
    }
}

V5SharedLibrary::V5SharedLibrary(const std::string& file, const int32_t& mode)
{
    load(file, mode);
}

bool V5SharedLibrary::LoadFailed()
{
#if defined _WIN32
    if (!loaded)
    {
        load(fileName, RTLD_NOW);
    }
#endif
    return loadFailed;
}


void* V5SharedLibrary::GetFunction(const std::string& name)
{
#if defined _WIN32
    if (!loaded)
    {
        load(fileName, RTLD_NOW);
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
