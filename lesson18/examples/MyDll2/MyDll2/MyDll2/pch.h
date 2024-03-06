
#include "framework.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

#define FUNC extern "C" MATHLIBRARY_API 

FUNC void Sum(const unsigned long long a, const unsigned long long b);

FUNC void Mult(const unsigned long long a, const unsigned long long b);

// extern "C" __declspec(dllimport) void Mult(const unsigned long long a, const unsigned long long b);