#pragma once
#include "Agent.h"

class Zombie : public GraphysX::Agent
{
public:
	Zombie();
	virtual ~Zombie();

	void update();

};

