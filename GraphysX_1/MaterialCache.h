#pragma once
#include <map>
#include "GfxEnum.h"

namespace GraphysX
{

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

}