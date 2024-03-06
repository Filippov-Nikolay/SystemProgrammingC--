#pragma once

#include <windows.h>
#include <windowsX.h>
#include <tchar.h>
#include <fstream>
#include "resource.h"

// Макросы, которые меняются в зависимости от кодировки
#ifdef _UNICODE
typedef std::wfstream tfstream;
typedef std::wifstream tifstream;
#else
typedef std::fstream tfstream;
typedef std::ifstream tifstream;
#endif

typedef std::basic_string<TCHAR> tstring;
