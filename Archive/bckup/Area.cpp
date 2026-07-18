#include "StdAfx.h"
#include "Area.h"

CLArea *CLArea::_singleton = NULL;

CLArea::CLArea(void)
{
	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	clDisplayer = CLDisplayer::getInstance();
	clInput = CLInput::getInstance();
	clClock = CLClock::getInstance();

	clBallZScene = new CLBallZScene();
	pActiveScene = clBallZScene; 
	if (!clInput->bGamePadConnected)
		pActiveScene->clCamera->bKeyboardNMouseCtrl = true;

}
CLArea::~CLArea(void)
{
	if (clBallZScene) { delete(clBallZScene); clBallZScene = NULL; }
}

void CLArea::Render(void)  
{
	//  Render Water surfaces
	//pActiveScene->UpdateWater(clClock->getTimeOfDay());
	clTV3D->pTV->Clear(true); 
	pActiveScene->Render(clClock->getTimeOfDay());
	clDisplayer->Render();
	clTV3D->pTV->RenderToScreen();
}
/////////////////////////////////////////////////
// Proceed to all actions on the area's members//
/////////////////////////////////////////////////
void CLArea::Update(void)
{
clClock->update(clGlobalVar->fTimeElapsed);
pActiveScene->clCamera->ManageCamCtrl(clGlobalVar->fTimeElapsed);
}




	
	
	


