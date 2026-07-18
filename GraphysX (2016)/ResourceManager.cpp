#include "stdafx.h"
#include "ResourceManager.h"

TextureCache ResourceManager::m_textureCache;
PhysmatCache ResourceManager::m_physmatCache;
MaterialCache ResourceManager::m_materialCache;
SoundCache ResourceManager::m_soundCache;

int ResourceManager::getTexture(std::string texFilename, cCONST_TV_COLORKEY colorKey)
{
	return m_textureCache.getTexture(texFilename);
}
int ResourceManager::getPhysmat(ePHYSMAT matType, NewtonWorld* world)
{
	return m_physmatCache.getPhysmat(matType, world);
}
int ResourceManager::getMaterial(eMATERIAL materialType)
{
	return m_materialCache.getMaterial(materialType);
}
FMOD::Sound* ResourceManager::getSound(std::string soundFilename, bool loop)
{
	return m_soundCache.getSound(soundFilename,  loop);
}
void ResourceManager::releaseSounds()
{
	m_soundCache.releaseSounds();
}

int ResourceManager::nbrPhysmat()
{
	return m_physmatCache.getNbrMat();
}
int ResourceManager::nbrTexture()
{
	return m_textureCache.getNbrTex();
}
int ResourceManager::nbrMaterial()
{
	return m_materialCache.getNbrMat();
}