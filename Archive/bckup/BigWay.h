#pragma once
#ifndef _CLBigWay_h__
#define _CLBigway_h__
#pragma once
#include "Scene.h"
#include "SplineFollow.h"


class CLFlyer
{
public:
	CLFlyer(){ iColor = BLUE_RGBA;  PathDive = NULL; };
	CLFlyer(int Color){ iColor = iColor; PathDive = NULL; };
	~CLFlyer();

	vecPosition3D Position, Orientation;
	int iColor;
	int iGroup;
	CTVMesh* Mesh;
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
	void LoadFormation(CLXMLData* data);
	void Simulate(void);	
	void Debug(void);
	
	vector<CLFlyer*> clFlyers;
	CLXMLData* clXMLData;
	

private:
	int iNbrFlyer;
	int iNbrPts;
	bool bLooped;
	CLSceneTools* clSceneTools;

	
	
};


#endif