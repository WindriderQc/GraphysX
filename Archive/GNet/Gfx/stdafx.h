#pragma once
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

//#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//#define NOMINMAX  // If not present, redefinition warning avec Newton
// Windows Header Files:
#include <windows.h>
//#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>
//#include <algorithm> // vector.erase  // TODO : check ou il est utilisé et mettre le include only there...


#define GFX_API_EXPORT
#include <TV3D65\tv_types.h>