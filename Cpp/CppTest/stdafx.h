// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#define _CRT_SECURE_NO_WARNINGS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma once

#include "targetver.h"

// windows
#include <winsock2.h>   // must be placed ahead of <windows.h>
#include <windows.h>
#include <shellapi.h>
#include <wininet.h>
#include <dbghelp.h>
#include <TlHelp32.h>
#include <combaseapi.h>
// gdi +
#include <gdiplus.h>

#pragma comment(lib,"gdiplus.lib")

#include <stdio.h>
#include <tchar.h>
#include <process.h>

// c++
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <set>
#include <cctype>

using namespace std;

// gtest
#include <gtest/gtest.h>





