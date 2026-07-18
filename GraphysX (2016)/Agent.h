#pragma once
#include "IGraphysX.h"

class Agent
{
public:
	Agent();
	virtual ~Agent();
	virtual void update() = 0;

protected:
	void create(IGraphysX* graphysX);

	STMeshParam stParam;
	CLMeshPrimitive* clPrim;
	float fMasse = 2.0f;
};

