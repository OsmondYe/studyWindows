#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <combaseapi.h>

#include <Tlhelp32.h>  // for CreateToolhelp32Snapshot

// c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <tchar.h>
// c++
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>



// I always like springf, fsprintf
#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif //PCH_H
