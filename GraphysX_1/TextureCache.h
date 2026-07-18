#pragma once
#include <map>

namespace GraphysX
{

	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();


		int getTexture(std::string texturePath, cCONST_TV_COLORKEY colorKey = cTV_COLORKEY_USE_ALPHA_CHANNEL);
		int getNbrTex();

	private:
		std::map<std::string, int> m_textureMap;

		const std::string texFolder = "..\\StockRoom\\";
	};

}