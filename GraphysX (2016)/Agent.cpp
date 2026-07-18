#include "stdafx.h"
#include "Agent.h"


Agent::Agent()
{
	stParam.name = "Agent";
	stParam.geom = eGeometry::SPHERE;
	stParam.scale = { 0.3f, 0.3f, 0.3f };
}
Agent::~Agent()
{
}

void Agent::create(IGraphysX* graphysX)
{
	clPrim = graphysX->pActiveScene->addDynamicMesh(stParam, DEF_PHYSMAT,"", fMasse);
}

