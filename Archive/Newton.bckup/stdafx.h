// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés

// add this to here   // PATCH pour Newton...
#if _MSC_VER >= 1400
#define HAVE_STRUCT_TIMESPEC
#endif


#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>




// Windows Header Files:
#include <stdexcept>
#include <windows.h>
#include "targetver.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Shlwapi.h>   //pour le RemoveFileNameSpec

enum  eObjet { CUBE, SPHERE, CYLINDER, CONE, BILLBOARD, FLOOR, CUSTOM, PHYSICCUBE, PHYSICSPHERE, PHYSICCYLINDER, PHYSICCONE, PHYSICCUSTOM, XMESH, PHYSICXMESH, PHYSICXSTATIC, DUPLICATE, V1_0 };
static const char* eObjet_names[] = { "CUBE", "SPHERE", "CYLINDER", "CONE", "BILLBOARD", "FLOOR", "CUSTOM", "PHYSICCUBE", "PHYSICSPHERE", "PHYSICSCYLINDER", "PHYSICCONE", "PHYSICCUSTOM", "XMESH", "PHYSICXMESH", "PHYSICXSTATIC", "DUPLICATE", "V1_0" };
// Physics Material used to set physical interaction between objects and detect collision.
enum ePHYSMAT { WOOD, FINISH, LEVEL, BALL, ELEVATOR };

//using namespace std;

#define MSGBOX(x, y) \
{ \
	std::ostringstream oss1; \
	oss1 << x; \
	std::ostringstream oss2; \
	oss2 << y; \
	MessageBoxA(NULL, oss1.str().c_str(), oss2.str().c_str(), MB_OK | MB_ICONQUESTION); \
}

#define PI 3.141592653
#define GRAVITY 9.81f  // 9.81 est l'attraction gravitationnelle de la Terre
#define SCALE 1;  // Ratio entre les unit de Newton et TV3D

#define BLACK_RGBA   RGBA256(0  , 0  ,   0, 255)
#define	BLUE_RGBA    RGBA256(0  , 0  , 255, 255)  
#define AQUA_RGBA    RGBA256(0  , 255, 255, 255)
#define WHITE_RGBA   RGBA256(255, 255, 255, 255)
#define YELLOW_RGBA  RGBA256(255, 255,   0, 255)
#define RED_RGBA	 RGBA256(255, 0  ,   0, 255)
#define GREEN_RGBA   RGBA256(0  , 255,   0, 255)
