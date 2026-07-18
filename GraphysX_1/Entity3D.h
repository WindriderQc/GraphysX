#pragma once
#include <string>
#include <vector>
#include <TV3D65\tv_types.h>
#include "STEntityParam.h"

namespace GraphysX
{
	class CLEntity3D
	{
	public:
		CLEntity3D();
		virtual ~CLEntity3D();
		virtual void update() { };
		//virtual void destroy() = 0;
		//std::string name = "entityDefault";

		bool isDead() { return bDead; }
		void kill() { bDead = true; }   //   TODO: sert a koi??
		STEntityParam entityParam;
		//static std::vector<CLEntity3D*> vecEntity;  // TODO: devrait etre private?
		static int getNbrEntity();
		
	private:
		static int nbrEntity;
		static int idCounter;
		bool bDead = false;
		int id = NULL;
	};
}