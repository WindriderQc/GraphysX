#include "stdafx.h"
#include "Displayer.h"

// Initialisation du singleton à NULL
CLDisplayer *CLDisplayer::_singleton = NULL;


CLDisplayer::CLDisplayer(void)
{
	p2DText = new CTVScreen2DText();
	clGlobalVar = CLGlobalVar::getInstance();
	//clInput = CLInput::getInstance();
	clClock = CLClock::getInstance();

	iScore = 0;
	iLap = 0;
	iTime = 0;

	IDFont = p2DText->TextureFont_Create("font", "consolas", 8, true, false, false, false, true);

}
CLDisplayer::~CLDisplayer()
{}


//Display 2D Text on the interface
void CLDisplayer::Render(float fBumpAmount)
{
	p2DText->Action_BeginText(false);
	//  Arranger pour que ca soit plus facile de choisir les ligne d'affichage!  Genre 1 ligne = 13 pixel...
	
	iTime = (int)clClock->getRealTime() / 1000;
	char BufferTime[256];
	sprintf_s(BufferTime, "%d", iTime);
	p2DText->NormalFont_DrawText("Time : ", 5, 32, WHITE_RGBA, IDFont);
	p2DText->NormalFont_DrawText(BufferTime, 85, 32, WHITE_RGBA, IDFont);
	char BufferScore[256];
	p2DText->NormalFont_DrawText("Score: ", 5, 44, WHITE_RGBA, IDFont);
	sprintf_s(BufferScore, "%d", iScore);
	p2DText->NormalFont_DrawText(BufferScore, 85, 44, WHITE_RGBA, IDFont);
	char BufferLap[256];
	p2DText->NormalFont_DrawText("Lap: ", 5, 56, WHITE_RGBA, IDFont);
	sprintf_s(BufferLap, "%d", iLap);
	p2DText->NormalFont_DrawText(BufferLap, 85, 56, WHITE_RGBA, IDFont);
	char BufferSpeed[256];
	p2DText->NormalFont_DrawText("Speed: ", 5, 68, WHITE_RGBA, IDFont);
	sprintf_s(BufferSpeed, "%f", clGlobalVar->fBallSpeed);
	p2DText->NormalFont_DrawText(BufferSpeed, 75, 68, WHITE_RGBA, IDFont);
	char BufferAlpha[256];
	p2DText->NormalFont_DrawText("Alpha: ", 5, 80, WHITE_RGBA, IDFont);
	sprintf_s(BufferAlpha, "%f", clGlobalVar->fDayNightAlpha);
	p2DText->NormalFont_DrawText(BufferAlpha, 75, 80, WHITE_RGBA, IDFont);

	p2DText->NormalFont_DrawText(FloatToStr(fBumpAmount).c_str(), 5, 92, WHITE_RGBA, IDFont);

	p2DText->Action_EndText();
}