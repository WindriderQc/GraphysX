#pragma once
#include <map>

class MaterialCache
{
public:
	MaterialCache();
	~MaterialCache();


	int getMaterial(eMATERIAL matType);
	int getNbrMat();


private:
	std::map<eMATERIAL, int> m_matMap;
	static int iMatDefault;
};

