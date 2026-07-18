// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#define DLL_EXPORT

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX  // If not present, redefinition warning avec Newton
// Windows Header Files:
#include <windows.h>
#include <WinSock2.h>

#include "GraphysXdef.h"
#include <iostream>
#include <string>
#include <algorithm> // vector.erase