#pragma once
#include <map>
#include "GfxEnum.h"


class NewtonWorld;
namespace GraphysX
{

	class PhysmatCache
	{
	public:
		PhysmatCache();
		~PhysmatCache();

		int getNbrMat();
		int getPhysmat(ePHYSMAT matType, NewtonWorld* world);
		ePHYSMAT getPhysmatWithNewtonID(int id, NewtonWorld* world);

	private:
		std::map<ePHYSMAT, int> m_physmatMap;
		static int iPhysmatDefault;
		//static NewtonWorld* World;

		int createNewtonDefaultMaterial(NewtonWorld* world, bool collidable = true, float softness = 0.05f, float elasticity = 0.4f, float staticFric = 1.0f, float kineticFric = 0.5f);
		int createNewtonMaterial(NewtonWorld* world);
	};
}