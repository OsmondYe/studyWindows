// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <combaseapi.h>

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
#include <map>
#include <algorithm>

// reference additional headers your program requires here

//for wtl
#include <atlbase.h>
#include "atlapp.h"
#include "atlwinx.h"	// wtl::CWindowEx
#include "atluser.h"
#include "atlframe.h"	//CFrameWindowImpl
#include "atlctrls.h"
#include "atlmisc.h"
#include "atlcrack.h"


//#include "resource.h"


extern CAppModule _Module;

// TODO: reference additional headers your program requires here
