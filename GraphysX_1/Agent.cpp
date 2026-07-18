#include "stdafx.h"
#include "Agent.h"
#include "Scene.h"

namespace GraphysX
{
	Agent::Agent()
	{
		stParam.name = "Agent";
		stParam.geom = eGeometry::SPHERE;
		stParam.scale = { 0.3f, 0.3f, 0.3f };
		stParam.masse = 2.0f;
	}
	Agent::~Agent()
	{
	}

	void Agent::create(CLScene* scene)
	{
		clPrim = scene->addDynamicMesh(stParam);
	}

}