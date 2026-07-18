#pragma once
#include "Agent.h"

class Zombie : public Agent
{
public:
	Zombie();
	virtual ~Zombie();

	void update();

};

