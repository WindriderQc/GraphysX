#include "stdafx.h"
#include "Entity3D.h"

namespace GraphysX
{
	int CLEntity3D::nbrEntity = 0;
	int CLEntity3D::idCounter = 0;
	//std::vector<CLEntity3D*> CLEntity3D::vecEntity;

	CLEntity3D::CLEntity3D()
	{
		//pScene->vEntity.push_back(this);
		nbrEntity++;
		idCounter++;
		id = idCounter;
	}
	CLEntity3D::~CLEntity3D()
	{
		//pScene->vEntity.erase((std::remove(pScene->vEntity.begin(), pScene->vEntity.end(), this)), pScene->vEntity.end());
		nbrEntity--;
	}
	int CLEntity3D::getNbrEntity()
	{
		return nbrEntity;
	}

}