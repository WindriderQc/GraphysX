#pragma once
//#include "TV3DMoteur.h"  /// ARK!!!!!!!!!!!!  tout crash si ca c'est pas include ici    WTF!!!!!!!!!!!!!!!!!!!!!!!
#include <TV3D65\CTVScreen2DText.h>   //  LAST BUG  !!   tas changer TV3DMoteur  pour ca......

namespace GraphysX
{ 
	class CLDisplayer
	{
	public:
		GRAPHYSX_API CLDisplayer();
		GRAPHYSX_API ~CLDisplayer();
		void Render();
		GRAPHYSX_API void setFont(const char* userFont, const char* fontName, int fontSize);
	
		GRAPHYSX_API void addLap();
		GRAPHYSX_API void resetLap();
		GRAPHYSX_API void addScore();
		GRAPHYSX_API void resetScore();
	

	private:
		CTVScreen2DText p2DText;
		int  IDFont = NULL;
	
		static int iScore;
		static int iLap;
	};
}