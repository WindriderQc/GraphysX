#include "stdafx.h"
#include "BigWay.h"


CLFormation::CLFormation(int NbrFlyer, int NbrPts, bool Loop) : iNbrFlyer(NbrFlyer), iNbrPts(NbrPts), bLooped(Loop)
{
	clSceneTools = new CLSceneTools();
	for (int i = 0; i < iNbrFlyer; i++){
		clFlyers.push_back(new CLFlyer(BLUE_RGBA));
	}

}
CLFormation::~CLFormation()
{}
void CLFormation::SavePoint(void)
{	
	for each (CLFlyer* flyer in clFlyers)
	{
		flyer->Position.push_back(flyer->Mesh->GetPosition());
		flyer->Orientation.push_back(flyer->Mesh->GetRotation());
	}
	/*
	vec.x = atof(elem->Attribute("x"));
	vec.y = atof(elem->Attribute("y"));
	vec.z = atof(elem->Attribute("z"));
	Spline.push_back(vec);
	*/
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
void CLFormation::LoadFormation(CLXMLData* data)
{
	clXMLData = data;
	clFlyers.clear();
	for (int i = 0; i < data->clFlyers.size(); i++){
		clFlyers.push_back(new CLFlyer());
		for (int j = 0; j < data->clFlyers[i]->Position.size(); j++){
			clFlyers.back()->Position[j] = data->clFlyers[i]->Position[j];
		}
	}
	clFlyers = clXMLData->clFlyers;

};
void CLFormation::Simulate(void)
{
	for each (CLFlyer* flyer in clFlyers)
	{
			
		flyer->PathDive = new CLSplineFollow(flyer->Mesh, flyer->Position, bLooped);
		flyer->PathDive->SetSpeed(1);
		//flyer->Position.push_back(flyer->cl3DObj->Mesh->GetPosition());
	}
};

void CLFormation::Debug(void)
{
	if (clFlyers[0]->PathDive){
		for each (CLFlyer* flyer in clFlyers)
		{
			clSceneTools->clTV3D->DrawSpline(flyer->PathDive->GetSpline(), WHITE_RGBA);
		}
	}
};
