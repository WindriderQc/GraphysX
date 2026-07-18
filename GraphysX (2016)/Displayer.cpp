#include "stdafx.h"
#include "Displayer.h"
#include "CLock.h"

namespace GraphysX
{
	int CLDisplayer::iScore = 0;
	int CLDisplayer::iLap = 0;

	CLDisplayer::CLDisplayer()
	{}
	CLDisplayer::~CLDisplayer()
	{}
	void CLDisplayer::setFont(const char* userFont, const char* fontName, int fontSize)
	{
		IDFont = p2DText.TextureFont_Create(userFont, fontName, fontSize, true, false, false, false, true);
	}
	void CLDisplayer::Render()
	{
		p2DText.Action_BeginText(false);
		//  Arranger pour que ca soit plus facile de choisir les ligne d'affichage!  Genre 1 ligne = 13 pixel...
		p2DText.NormalFont_DrawText("Time : ", 5, 32, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText(std::to_string(GraphysX::CLClock::getRealTime() / 1000).c_str(), 85, 32, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText("Score: ", 5, 44, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText(std::to_string(iScore).c_str(), 85, 44, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText("Lap: ", 5, 56, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText(std::to_string(iLap).c_str(), 85, 56, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText("Speed: ", 5, 68, RGBA_WHITE, IDFont);
		p2DText.NormalFont_DrawText("Globalvar..  come on!", 75, 68, RGBA_WHITE, IDFont);
		/*p2DText->NormalFont_DrawText("Alpha: ", 5, 80, RGBA_WHITE, IDFont);
		p2DText->NormalFont_DrawText(std::to_string(clGlobalVar.getDayNightAlpha()).c_str(), 75, 80, RGBA_WHITE, IDFont);*/
		p2DText.Action_EndText();
	}

	void CLDisplayer::addLap() { iLap++; }
	void CLDisplayer::resetLap(){ iLap = 0; }
	void CLDisplayer::addScore(){ iScore++; }
	void CLDisplayer::resetScore(){ iScore = 0; }
}