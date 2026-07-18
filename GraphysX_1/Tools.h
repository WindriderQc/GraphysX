#pragma once
#include "Fcntl.h" // pour la console
#include "io.h"  // pour la console
#include "GraphysXdef.h"   //#include "TV3D65\tv_Types.h" // pour le cTV_3DVECTOR                        

#include "TV3D65\tvAll.h" //CTVMesh*, ctv_3DVector et Spline   //  TODO: ne serait pas déjà connu a cause de GraphysXdef??
#include "ASCIIFile.h"
#include <random>


namespace Tools
{
	GRAPHYSX_API int RandomDice(int Size);
	GRAPHYSX_API GraphysX::Spline CreateTrianglePile(int NbrEtage, cTV_3DVECTOR pos, float BoxSize);
	GRAPHYSX_API void  DevConsoleCreate(void);
	GRAPHYSX_API std::vector<cTV_3DVECTOR> XmlSplineRead(std::string filename);
	GRAPHYSX_API void XmlSplineAddNode(cTV_3DVECTOR vec, std::string filename);
	GRAPHYSX_API GraphysX::Spline OBJtoSpline(std::string filePath);
	GRAPHYSX_API bool  IsPositionInsideBoundingBox(cTV_3DVECTOR pos, cTV_3DVECTOR BoxMin, cTV_3DVECTOR BoxMax);
	GRAPHYSX_API bool  IsPositionInsideMesh(cTV_3DVECTOR pos, CTVMesh* mesh);
	GRAPHYSX_API std::string floatToStr(float f);
	GRAPHYSX_API std::string getTimeString();
	//GRAPHYSX_API std::string intToString(int value);


	GRAPHYSX_API std::string getexepath();

	//static std::mt19937 randomEngine;
	GRAPHYSX_API int   random_int(int min, int max);
	GRAPHYSX_API float random_float(float min, float max);


	template <typename T>
	std::string to_string(T const& value) {
		stringstream sstr;
		sstr << value;
		return sstr.str();
	}

	void  MsgBox(std::string x, std::string y);
	
}



// Search and remove whitespace from both ends of the string
static std::string TrimEnumString(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isspace(*it)) { it++; }
	std::string::const_reverse_iterator rit = s.rbegin();
	while (rit.base() != it && isspace(*rit)) { rit++; }
	return std::string(it, rit.base());
}
static void SplitEnumArgs(const char* szArgs, std::string Array[], int nMax)
{
	std::stringstream ss(szArgs);
	std::string strSub;
	int nIdx = 0;
	while (ss.good() && (nIdx < nMax)) {
		getline(ss, strSub, ',');
		Array[nIdx] = TrimEnumString(strSub);
		nIdx++;
	}
};
#define DECLARE_ENUM(ename, ...) \
    enum ename { __VA_ARGS__, MAX_NUMBER_OF_##ename }; \
    static std::string ename##Strings[MAX_NUMBER_OF_##ename]; \
    static const char* ename##ToString(ename e) { \
        if (ename##Strings[0].empty()) { SplitEnumArgs(#__VA_ARGS__, ename##Strings, MAX_NUMBER_OF_##ename); } \
        return ename##Strings[e].c_str(); \
	    } \
    static ename StringTo##ename(const char* szEnum) { \
        for (int i = 0; i < MAX_NUMBER_OF_##ename; i++) { if (ename##Strings[i] == szEnum) { return (ename)i; } } \
        return MAX_NUMBER_OF_##ename; \
	    }



/*

#include <iostream>
#include <limits>
#include <stdexcept>
using namespace std;
#include <windows.h>
//----------------------------------------------------------------------------
struct console
{
console(unsigned width, unsigned height)
{
SMALL_RECT r;
COORD      c;
hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
if (!GetConsoleScreenBufferInfo(hConOut, &csbi))
throw runtime_error("You must be attached to a human.");

r.Left = r.Top = 0;
r.Right = width - 1;
r.Bottom = height - 1;
SetConsoleWindowInfo(hConOut, TRUE, &r);

c.X = width;
c.Y = height;
SetConsoleScreenBufferSize(hConOut, c);
}

~console()
{
SetConsoleTextAttribute(hConOut, csbi.wAttributes);
SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
}

void color(WORD color = 0x07)
{
SetConsoleTextAttribute(hConOut, color);
}

HANDLE                     hConOut;
CONSOLE_SCREEN_BUFFER_INFO csbi;
};

//----------------------------------------------------------------------------
}

/*
try this snippet:

Code:
vecLookAt = tvGlobals.Vector3(0, 0, 1)
tvMath.TVVec3TransformCoord(vecLookAt, vecLookAt, Actor2.GetMatrix)


The first line creates a vector that will be 1 unit forward from the Actor2.
The second line transforms the vector by the matrix of the actor moving it to the objects local space.

I have written it into a function.  I havent actually tested this yet, but its should work

Code:

Public Function GetLookAt(ByVal Actor2 As TrueVision3D.TVActor2, ByVal Optional LocalSpace As Boolean)
'Returns the LookAt point 1 unit in front of the Actor2

Dim vecLookAt As DxVBLibA.D3DVECTOR
Dim matTemp As DxVBLibA.D3DMATRIX

If LocalSpace Then
'use only rotation matrix for LookAt Point relative to Actor
Actor2.GetRotationMatrix(matTemp)
Else
'use whole matrix for world space LookAt Point
matTemp = Actor2.GetMatrix
End If

'Calculate point
vecLookAt = tvGlobals.Vector3(0, 0, 1)
tvMath.TVVec3TransformCoord(vecLookAt, vecLookAt, matTemp)

GetLookAt=vecLookAt

End Function


I added a boolean variable to decide wether the returned value gives you the world position the model is actually looking at, or a vector the represents the forward direction of the actor*/
//*/

