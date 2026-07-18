#include "stdafx.h"
#include "PhysMatCache.h"

int PhysmatCache::iPhysmatDefault = -1;
//NewtonWorld* PhysmatCache::World = nullptr;


PhysmatCache::PhysmatCache()
{
}
PhysmatCache::~PhysmatCache()
{
}
int PhysmatCache::createNewtonDefaultMaterial(NewtonWorld* world, bool collidable, float softness, float elasticity, float staticFric, float kineticFric)
{
	int mat = NewtonMaterialGetDefaultGroupID(world);
	// set default material properties  
	NewtonMaterialSetDefaultCollidable(world, mat, mat, collidable);
	NewtonMaterialSetDefaultSoftness(world, mat, mat, softness);
	NewtonMaterialSetDefaultElasticity(world, mat, mat, elasticity);
	NewtonMaterialSetDefaultFriction(world, mat, mat, staticFric, kineticFric);
	return mat;
}
int PhysmatCache::createNewtonMaterial(NewtonWorld* world)
{
	return NewtonMaterialCreateGroupID(world);
}
int PhysmatCache::getNbrMat()
{
	return m_physmatMap.max_size();
}
int PhysmatCache::getPhysmat(ePHYSMAT matType, NewtonWorld* world)
{
	if (iPhysmatDefault == -1)
	{
		iPhysmatDefault = createNewtonDefaultMaterial(world);
		m_physmatMap.insert(std::make_pair(ePHYSMAT::DEF_PHYSMAT, iPhysmatDefault));
	}

	auto mit = m_physmatMap.find(matType);
	if (mit == m_physmatMap.end())
	{
		int matID = createNewtonMaterial(world);
		m_physmatMap.insert(std::make_pair(matType, matID));
		return matID;
	}
	return mit->second;
}

ePHYSMAT PhysmatCache::getPhysmatWithNewtonID(int id, NewtonWorld* world)
{
	if (iPhysmatDefault == -1)
	{
		iPhysmatDefault = createNewtonDefaultMaterial(world);
		m_physmatMap.insert(std::make_pair(ePHYSMAT::DEF_PHYSMAT, iPhysmatDefault));
	}

	for(auto& iter : m_physmatMap)
	{
		if (iter.second == id) return iter.first;
	}

	return ePHYSMAT::DEF_PHYSMAT;
}

