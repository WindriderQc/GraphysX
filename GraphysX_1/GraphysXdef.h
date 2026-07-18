#pragma once
#include <string>
#include <algorithm> // vector.erase  // TODO : check ou il est utilisé et mettre le include only there...?
#include <vector>
#include <time.h>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <memory>
#include <map>             //  
#include <TV3D65\tv_types.h>
#include <TV3D65\HelperFunctions.h>


#ifdef DLL_EXPORT
	#define GRAPHYSX_API __declspec(dllexport)
#else
	#define GRAPHYSX_API __declspec(dllimport)
#endif


#define MSGBOX(x, y) \
{ \
	std::ostringstream oss1; \
	oss1 << x; \
	std::ostringstream oss2; \
	oss2 << y; \
	MessageBoxA(NULL, oss1.str().c_str(), oss2.str().c_str(), MB_OK | MB_ICONQUESTION); \
}


#define MAX_PHYSICS_FPS		60.0f //120.0f
#define PI		3.14159265358979323846
#define M_E     2.71828182845904523536
#define GRAVITY 9.81f  // 9.81 est l'attraction gravitationnelle de la Terre
#define SCALE 1;  // Ratio entre les unit de Newton et TV3D


#define RGBA_BLACK     RGBA256(  0,   0,   0, 255)
#define RGBA_DARKGRAY  RGBA256( 69,  69,  69, 255)
#define	RGBA_BLUE      RGBA256(  0,   0, 255, 255)  
#define RGBA_AQUA      RGBA256(  0, 255, 255, 255)
#define RGBA_WHITE     RGBA256(255, 255, 255, 255)
#define RGBA_YELLOW    RGBA256(255, 255,   0, 255)
#define RGBA_RED	   RGBA256(255,   0,   0, 255)
#define RGBA_GREEN     RGBA256(  0, 255,   0, 255)

namespace GraphysX
{
	struct sVERTEX
	{
		float x; float y; float z;
		sVERTEX() : x(0), y(0), z(0){};
	};

	struct sLine3D{
		cTV_3DVECTOR Start, End;
		int Color;
		sLine3D() : Start(cTV_3DVECTOR(0, 0, 0)), End(cTV_3DVECTOR(0, 0, 0)), Color(RGBA_WHITE) {};
		sLine3D(cTV_3DVECTOR start, cTV_3DVECTOR end, int color) : Start(start), End(end), Color(color) {};
	};

	typedef std::vector<cTV_3DVECTOR> Spline;
}