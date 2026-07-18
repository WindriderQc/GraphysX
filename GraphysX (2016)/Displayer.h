#pragma once
#include "TV3DMoteur.h"


namespace GraphysX
{ 
	class CLDisplayer
	{
	public:
		CLDisplayer();
		~CLDisplayer();
		void Render();
		GRAPHYSX_API void setFont(const char* userFont, const char* fontName, int fontSize);
	
		GRAPHYSX_API void addLap();
		GRAPHYSX_API void resetLap();
		GRAPHYSX_API void addScore();
		GRAPHYSX_API void resetScore();
	

	private:
		CTVScreen2DText p2DText;
		CTVScene pTVScene;
		int  IDFont = NULL;
	
		static int iScore;
		static int iLap;
	};
}