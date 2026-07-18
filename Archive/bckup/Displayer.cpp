#include "stdafx.h"
#include "Displayer.h"

// Initialisation du singleton à NULL
CLDisplayer *CLDisplayer::_singleton = NULL;


CLDisplayer::CLDisplayer(void)
{
	p2DText = new CTVScreen2DText();
	clGlobalVar = CLGlobalVar::getInstance();
	clInput = CLInput::getInstance();
	clClock = CLClock::getInstance();

	iScore = 0;
	iLap = 0;
}
CLDisplayer::~CLDisplayer()
{}
//Display 2D Text on the interface
void CLDisplayer::Render(void)
{
	p2DText->Action_BeginText(false);
			
	//  Arranger pour que ca soit plus facile de choisir les ligne d'affichage!  Genre 1 ligne = 13 pixel...

		char BufferTime[256];
		sprintf_s(BufferTime, "%d", ((int)clClock->getRealTime())/1000);
		p2DText->NormalFont_DrawText("   Time : ", 5, 32, WHITE_RGBA, 0);
		p2DText->NormalFont_DrawText(BufferTime, 85, 32, WHITE_RGBA, 0);
		
		char BufferScore[256];
		p2DText->NormalFont_DrawText("Score: ", 5, 45, WHITE_RGBA, 0);
		sprintf_s(BufferScore, "%d", iScore);
		p2DText->NormalFont_DrawText(BufferScore, 85, 45, WHITE_RGBA, 0);

		char BufferLap[256];
		p2DText->NormalFont_DrawText("Lap: ", 5, 58, WHITE_RGBA, 0);
		sprintf_s(BufferLap, "%d", iLap);
		p2DText->NormalFont_DrawText(BufferLap, 85, 58, WHITE_RGBA, 0);
		
		char BufferSpeed[256];
		p2DText->NormalFont_DrawText("Speed: ", 5, 71, WHITE_RGBA, 0);
		sprintf_s(BufferSpeed, "%f", clGlobalVar->fBallSpeed);
		p2DText->NormalFont_DrawText(BufferSpeed, 75, 71, WHITE_RGBA, 0);

		char BufferAlpha[256];
		p2DText->NormalFont_DrawText("Alpha: ", 5, 84, WHITE_RGBA, 0);
		sprintf_s(BufferAlpha, "%f", clGlobalVar->fDayNightAlpha);
		p2DText->NormalFont_DrawText(BufferAlpha, 75, 84, WHITE_RGBA, 0);

	p2DText->Action_EndText();
}