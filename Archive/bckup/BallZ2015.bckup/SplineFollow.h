#pragma once
#include "StdAfx.h"

class CLSplineFollow
{
private:
	CLTV3DMoteur *clTV3D;
	vecPosition3D Spline2Follow;
	CTVPath* path;
	CTVMesh* Mesh;

public:
	CLSplineFollow(CTVMesh* MovingMesh, vecPosition3D Spline, bool bLooping=false) 
		: Spline2Follow(Spline), Mesh(MovingMesh)
	{
		clTV3D = CLTV3DMoteur::getInstance();
		path = new CTVPath();
		path->SetPathType(cTV_PATH_SPLINE);
		path->EnableLooping(bLooping);

		for each(cTV_3DVECTOR vec in Spline2Follow)
			path->AddPathNode(&vec);

		Mesh->SetPath(path);
		Mesh->SetPathSpeed(0, true); // SetPathSpeed(20 * tv.AccurateTimeElapsed(), true);

		
		const char* saved = "";
		path->Save(saved);  //  la fonction ne change pas la valeur de saved...
		cout << saved << endl;
		
	};
	~CLSplineFollow(void){
		if (path) { delete(path); path = NULL; }
	};
	void SetSpeed(float speed){
		Mesh->SetPathSpeed(speed, true);
	};
	vecPosition3D GetSpline(void){
		return(Spline2Follow);
	};
};