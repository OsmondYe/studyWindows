#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>



// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// c++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <mutex>
#include <string>
#include <vector>


// atl & wtl
#include <combaseapi.h>
#include <atlbase.h>
#include "atlapp.h"
#include "atlwinx.h"	// wtl::CWindowEx
#include "atluser.h"
#include "atlframe.h"	//CFrameWindowImpl
#include "atlctrls.h"
#include "atlmisc.h"
#include "atlcrack.h"
#include "atlddx.h"


// gdi+ above win_xp
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "resource.h"
