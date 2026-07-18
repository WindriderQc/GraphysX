#include "StdAfx.h"
#include "Area.h"

CLArea *CLArea::_singleton = NULL;

CLArea::CLArea(void)
{
	

	
	clSceneStaff = new CLSceneStaff();

	clBallZScene = new CLBallZScene();
	pActiveScene = clBallZScene; 
	if (!clSceneStaff->clInput->bGamePadConnected)
		pActiveScene->clSceneStaff->clCamera->bKeyboardNMouseCtrl = true;

}
CLArea::~CLArea(void)
{
	if (clBallZScene) { delete(clBallZScene); clBallZScene = NULL; }
}
/////////////////////////////////////////////////
// Proceed to all actions on the area's members//
/////////////////////////////////////////////////
void CLArea::Update(void)
{

	pActiveScene->Render(clClock->getTimeOfDay());
	
}




	
	
	


