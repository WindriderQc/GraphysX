#include "stdafx.h"
#include "TextureCache.h"
#include "TV3DMoteur.h"


TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

int TextureCache::getTexture(std::string texFilename, cCONST_TV_COLORKEY colorKey)  // TODO: problemo...  si un texture déjà loader, le color key peut pas etre changé...
{
	auto mit = m_textureMap.find(texFilename);

	if (mit == m_textureMap.end())
	{
		CTVTextureFactory pTexFac;
		std::string filePath = texFolder + texFilename;
		int tex = pTexFac.LoadTexture(filePath.c_str(), texFilename.c_str(), -1, -1, colorKey); // cTV_COLORKEY_USE_ALPHA_CHANNEL
		m_textureMap.insert(make_pair(texFilename, tex));
		return tex;
	}
	return mit->second;
}

int TextureCache::getNbrTex()
{
	return m_textureMap.max_size();
}
