#pragma once

#include <windows.h>
#include <windowsX.h>
#include <tchar.h>
#include <fstream>
#include <commctrl.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _UNICODE
typedef std::wfstream tfstream;
typedef std::wifstream tifstream;
#else
typedef std::fstream tfstream;
typedef std::ifstream tifstream;
#endif

typedef std::basic_string<TCHAR> tstring;