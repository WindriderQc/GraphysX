#pragma once
#include "TextureCache.h"
#include "PhysmatCache.h"
#include "MaterialCache.h"
#include "SoundCache.h"

namespace GraphysX
{

	class ResourceManager
	{
	public:
		GRAPHYSX_API static int getTexture(std::string texFilename, cCONST_TV_COLORKEY colorKey = cTV_COLORKEY_USE_ALPHA_CHANNEL);
		GRAPHYSX_API static int getPhysmat(ePHYSMAT physmatType, NewtonWorld* world);
		GRAPHYSX_API static int getMaterial(eMATERIAL materialType);
		GRAPHYSX_API static FMOD::Sound* getSound(std::string, bool loop);
		GRAPHYSX_API static void releaseSounds();
		GRAPHYSX_API static int nbrMaterial();
		GRAPHYSX_API static int nbrTexture();
		GRAPHYSX_API static int nbrPhysmat();

		//static ePHYSMAT ePhysmatFromIndex(int i) { return static_cast<ePHYSMAT>(i); }

	private:
		static TextureCache m_textureCache;
		static PhysmatCache m_physmatCache;
		static MaterialCache m_materialCache;
		static SoundCache m_soundCache;
	};

}