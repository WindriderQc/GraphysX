#pragma once
#ifndef _CLBigWay_h__
#define _CLBigway_h__
#pragma once
#include "Scene.h"
#include "SplineFollow.h"


class CLFlyer
{
public:
	CLFlyer(){ iColor = BLUE_RGBA;  PathDive = NULL;	};
	CLFlyer(int Color, CL3DObject* Obj){ iColor = iColor; PathDive = NULL; Flyer3D = Obj; };
	~CLFlyer() {if(Flyer3D) delete(Flyer3D); Flyer3D = NULL; };

	void AddActualPoint(void){
		Positions.push_back(Flyer3D->Mesh->GetPosition()); // 2 pts pour creer une étape
		Positions.push_back(Flyer3D->Mesh->GetPosition()); // 2 pts pour creer une étape
		Orientations.push_back(Flyer3D->Mesh->GetRotation());
		Orientations.push_back(Flyer3D->Mesh->GetRotation());
	};

	vecPosition3D Positions, Orientations;
	int iColor;
	int iGroup;
	CL3DObject* Flyer3D;
	CLSplineFollow* PathDive;
};


class CLXMLData
{
public:
	CLXMLData(){};
	vector<CLFlyer*> clFlyers;
};

class CLFormation
{
	
public:
	CLFormation(int NbrFlyer, int NbrPts, bool Loop);
	~CLFormation();

	void SavePoint(void);
	CLXMLData* SaveFormation(void);
	void Simulate(void);	
	void Debug(void);
	
	vector<CLFlyer*> clFlyers;
	CLXMLData* clXMLData;
	

private:
	int iNbrFlyer;
	int iNbrPts;
	bool bLooped;
	int iMagicienTex;
	CLSceneStaff* clSceneStaff;

	
	
};


#endif