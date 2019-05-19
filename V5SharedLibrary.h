#pragma once

#if defined _WIN32
#include <windows.h>
#include <stdint.h>
#else
#include <dlfcn.h>
#endif

#if defined _WIN32 || defined __CYGWIN__
	#ifdef DYNAMIC_LIB
		#ifdef __GNUC__
			#define EXPORTED __attribute__ ((dllexport))
		#else
			#ifdef __cplusplus
				#define EXPORTED extern "C" __declspec(dllexport)
				#define EXPORTED_MEMBER __declspec(dllexport)

			#else
				#define EXPORTED __declspec(dllexport)
				#define EXPORTED_MEMBER __declspec(dllexport)
			#endif
		#endif
	#else
		#ifdef __GNUC__
			#define EXPORTED __attribute__ ((dllimport))
			#define EXPORTED_MEMBER __attribute__ ((dllimport))
		#else
			#define EXPORTED __declspec(dllimport)
			#define EXPORTED_MEMBER __declspec(dllimport)
		#endif
	#endif
	#define NOT_EXPORTED
#else
	#if __GNUC__ >= 4
        #ifdef __cplusplus
		    #define EXPORTED extern "C" __attribute__ ((visibility ("default")))
		    #define EXPORTED_MEMBER __attribute__ ((visibility ("default")))
        #else
            #define EXPORTED __attribute__ ((visibility ("default")))
            #define EXPORTED_MEMBER __attribute__ ((visibility ("default")))
        #endif
		#define NOT_EXPORTED  __attribute__ ((visibility ("hidden")))
	#else
        #define EXPORTED
        #define EXPORTED_MEMBER
		#define NOT_EXPORTED
	#endif
#endif




#include <iostream>

#if defined _WIN32
class V5SharedLibrary
{
private:
    HINSTANCE handle;
    bool loaded;
    std::string fileName;
    bool loadFailed;
public:
    const static int32_t MODE_LAZY;
    const static int32_t MODE_NOW;
	V5SharedLibrary(const std::string& file, const int32_t& mode = MODE_NOW);
    bool LoadFailed();
    void* GetFunction(const std::string& name);
	virtual ~V5SharedLibrary();
};
#else
class V5SharedLibrary
{
private:
    void* handle;
    bool loadFailed;
public:
    const static int32_t MODE_LAZY;
    const static int32_t MODE_NOW;
	V5SharedLibrary(const std::string& file, const int32_t& mode = MODE_NOW);
    bool LoadFailed();
    void* GetFunction(const std::string& name);
	virtual ~V5SharedLibrary();
};
#endif
