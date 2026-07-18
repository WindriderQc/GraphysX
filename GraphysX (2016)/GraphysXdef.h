#pragma once
#include <vector>
#include <string>
#include <time.h>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <algorithm>
#include <memory>

#include <TV3D65\tv_types.h>


#if defined DLL_EXPORT
#define GRAPHYSX_API __declspec(dllexport)
#else
#define GRAPHYSX_API __declspec(dllimport)
#endif

enum eObjet { PRIMITIVE, BILLBOARD, FLOOR, CUSTOM, PHYSPRIMITIVE, PHYSICCUSTOM, XMESH, PHYSICXMESH, PHYSICXSTATIC, DUPLICATEMESH, TEXT, VERTEXSTRIP, V1_3 };
static const char* eObjet_names[] = { "PRIMITIVE", "BILLBOARD", "FLOOR", "CUSTOM", "PHYSPRIMITIVE", "PHYSICCUSTOM", "XMESH", "PHYSICXMESH", "PHYSICXSTATIC", "DUPLICATEMESH", "TEXT", "VERTEXSTRIP", "V1_3" };
// Physics Material used to set physical interaction between objects and detect collision.
enum ePHYSMAT  { DEF_PHYSMAT, WALL, FINISH, GROUND, BALL, HUMAN };
static const char* ePhysmat_names[] = { "DEF_PHYSMAT", "WOOD", "FINISH", "LEVEL", "BALL", "ELEVATOR" };

enum eMATERIAL  { DEF_MATERIAL, TRANSLUCENT, MATE, METAL_SHINE, GLASS };
static const char* eMaterial_names[] = { "DEF_MATERIAL","TRANSLUCENT", "GLASS", "MATE", "METAL_SHINE" };

enum eGeometry { CUBE, SPHERE, CYLINDER, CONE, TEAPOT, PLANE, NON_PRIMITIVE, CUSTOM_MESH };
static const char* eGeometry_names[] = { "CUBE", "SPHERE", "CYLINDER", "CONE", "TEAPOT", "PLANE", "NON_PRIMITIVE" };


#define MAX_PHYSICS_FPS		60.0f //120.0f
#define PI 3.14159265358979323846
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

#define MSGBOX(x, y) \
{ \
	std::ostringstream oss1; \
	oss1 << x; \
	std::ostringstream oss2; \
	oss2 << y; \
	MessageBoxA(NULL, oss2.str().c_str(), oss1.str().c_str(), MB_OK | MB_ICONQUESTION); \
}

typedef struct STMeshParam
{
	//cTV_3DMATRIX Matrix = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };   //   Trouver comment la mettre par défaut..  pos a 0,0,0, scale a 1, rot a 0.
	cTV_3DVECTOR pos;
	cTV_3DVECTOR rot;
	cTV_3DVECTOR scale;
	eGeometry geom;
	bool enable;
	std::string name;
	float speed;
	unsigned int color;

	/*STMeshParam()
	{
	pos = { 0, 0, 0 };
	rot = { 0, 0, 0 };
	scale = { 1, 1, 1 };
	geom = eGeometry::CUBE;
	enable = true;
	name = "";
	speed = 0.0f;
	color = RGBA_BLUE;
	}*/
	STMeshParam(std::string sname = "", eGeometry geometry = eGeometry::CUBE, cTV_3DVECTOR vpos = cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR vscale = cTV_3DVECTOR(1, 1, 1), cTV_3DVECTOR vrot = cTV_3DVECTOR(0, 0, 0), bool benable = true, float fspeed = 0.0f, int icolor = -1)
	{
		name = sname;
		pos = vpos;
		rot = vrot;
		scale = vscale;
		geom = geometry;
		enable = benable;
		speed = fspeed;
		color = icolor;
	}

}STMeshParam;