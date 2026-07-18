#include "stdafx.h"
#include "BigWay.h"


CLFormation::CLFormation(int NbrFlyer, int NbrPts, bool Loop) : iNbrFlyer(NbrFlyer), iNbrPts(NbrPts), bLooped(Loop)
{
	clSceneStaff = new CLSceneStaff();
}
CLFormation::~CLFormation()
{
	delete(clSceneStaff); clSceneStaff = NULL;
}
void CLFormation::SavePoint(void)
{	
	for each (CLFlyer* flyer in clFlyers)
	{
		flyer->Positions.push_back(flyer->Flyer3D->Mesh->GetPosition());
		flyer->Orientations.push_back(flyer->Flyer3D->Mesh->GetRotation());
	}
};
CLXMLData* CLFormation::SaveFormation(void)
{
	clXMLData = new CLXMLData();
	clXMLData->clFlyers = clFlyers;
	return(clXMLData);
	/*
	vec.x = atof(elem->Attribute("x"));
	vec.y = atof(elem->Attribute("y"));
	vec.z = atof(elem->Attribute("z"));
	Spline.push_back(vec);
	*/
};

void CLFormation::Simulate(void)
{
	for each (CLFlyer* flyer in clFlyers)
	{
			
		flyer->PathDive = new CLSplineFollow(flyer->Flyer3D->Mesh, flyer->Positions, bLooped);
		//flyer->PathDive->SetSpeed(1);
		//flyer->Position.push_back(flyer->cl3DObj->Mesh->GetPosition());
	}
};

void CLFormation::Debug(void)
{
	if (clFlyers[0]->PathDive){
		for each (CLFlyer* flyer in clFlyers)
		{
			clSceneStaff->clTV3D->DrawSpline(flyer->PathDive->GetSpline(), WHITE_RGBA);
		}
	}
};
