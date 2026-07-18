#pragma once
#include "STEntityParam.h"

namespace GraphysX
{
	class CLMeshPrimitive;
	class CLScene;

	class Agent
	{
	public:
		Agent();
		virtual ~Agent();
		virtual void update() = 0;

	protected:
		void create(CLScene* scene);

		STEntityParam stParam;
		CLMeshPrimitive* clPrim;
	};

}