// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// c++
#include <string>

// reference additional headers your program requires here

//for wtl
#include <atlbase.h>
#include "atlapp.h"
#include "atlframe.h"	//CFrameWindowImpl
#include "atlwinx.h"	// wtl::CWindowEx
#include "atlctrls.h"
#include "atlmisc.h"


#include "resource.h"


extern CAppModule _Module;