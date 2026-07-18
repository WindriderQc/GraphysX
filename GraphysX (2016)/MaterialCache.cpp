#include "stdafx.h"
#include "MaterialCache.h"
#include "TV3DMoteur.h"

int MaterialCache::iMatDefault = -1;

MaterialCache::MaterialCache()
{
}
MaterialCache::~MaterialCache()
{
}

int MaterialCache::getNbrMat()
{
	return m_matMap.max_size();
}
int MaterialCache::getMaterial(eMATERIAL matType)
{	
	CTVMaterialFactory pTVMatFac;
	if (iMatDefault == -1)
	{
		iMatDefault = pTVMatFac.CreateMaterial("defaultMat");
		pTVMatFac.SetAmbient(iMatDefault, 0.1f, 0.1f, 0.1f, 1);
		pTVMatFac.SetDiffuse(iMatDefault, 0.8f, 0.8f, 0.8f, 1);
		pTVMatFac.SetSpecular(iMatDefault, 0.2f, 0.2f, 0.2f, 1);
		pTVMatFac.SetPower(iMatDefault, 20);
		pTVMatFac.SetEmissive(iMatDefault, 0, 0, 0, 1);
		std::cout << "  - Standard lighting material created." << std::endl;
		m_matMap.insert(std::make_pair(DEF_MATERIAL, iMatDefault));


		//Create the translucent Material
		int iTranslucentMaterial = pTVMatFac.CreateMaterial("TranslucentMat");
		pTVMatFac.SetAmbient(iTranslucentMaterial, 0.25f, 0.25f, 0.25f, 1);
		pTVMatFac.SetDiffuse(iTranslucentMaterial, 0.75f, 0.75f, 0.75f, 1);
		pTVMatFac.SetSpecular(iTranslucentMaterial, 1, 1, 1, 1);
		pTVMatFac.SetPower(iTranslucentMaterial, 15);
		m_matMap.insert(std::make_pair(TRANSLUCENT, iTranslucentMaterial));
		std::cout << "  - Translucent lighting material created." << std::endl;
		// Create the GlassStylish lighting material
		int iGlassMaterial = pTVMatFac.CreateMaterial("GlassMat");
		pTVMatFac.SetAmbient(iGlassMaterial, 0.1f, 0.1f, 0.1f, 0.4f);
		pTVMatFac.SetDiffuse(iGlassMaterial, 0.8f, 0.8f, 0.8f, 0.4f);
		pTVMatFac.SetSpecular(iGlassMaterial, 0.2f, 0.2f, 0.2f, 0.4f);
		pTVMatFac.SetPower(iGlassMaterial, 20);
		pTVMatFac.SetEmissive(iGlassMaterial, 0, 0, 0, 0.3f);
		m_matMap.insert(std::make_pair(GLASS, iGlassMaterial));
		std::cout << "  - GlassStyle lighting material created." << std::endl;
	}
	auto mit = m_matMap.find(matType);
	if (mit == m_matMap.end())
	{
		int matID = pTVMatFac.CreateMaterial(eMaterial_names[matType]);
		m_matMap.insert(std::make_pair(matType, matID));
		return matID;
	}
	return mit->second;
}

