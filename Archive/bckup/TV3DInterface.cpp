#include "stdafx.h"
#include "TV3DInterface.h"


TV3DInterface::TV3DInterface(HWND TVScreenHWND, string PathString)
{
	// Setup the all the 3D, physics, Input, sounds, etc...
	srand(UINT(time(NULL)));  // Called once so we can generate random number after...  maybe should be with the random generator?
	clGlobalVar = CLGlobalVar::getInstance();
	clGlobalVar->w3DWindowHandle = (TVScreenHWND);

	char * PathToApp = new char[PathString.size() + 1];
	std::copy(PathString.begin(), PathString.end(), PathToApp);
	PathToApp[PathString.size()] = '\0'; // don't forget the terminating 0
	clGlobalVar->SearchPath = PathToApp;


	//Set up la simulation 3D
	clTV3D = CLTV3DMoteur::getInstance(); // Create the TV3D engine singleton & initialize 3D basé sur TrueVision3D
	clInput = CLInput::getInstance();   //  Create the Input singleton.
	clArea = CLArea::getInstance();    //  Create the Area singleton.
}
TV3DInterface::~TV3DInterface()
{
	
	clInput->kill();
	clArea->kill();
	clTV3D->kill();
	clGlobalVar->kill();
}
void TV3DInterface::UpdateLoop()
{
	clGlobalVar->fTimeElapsed = clTV3D->pTV->AccurateTimeElapsed();  // On appelle la fonction de l'engine 1 seule fois par boucle.
	clArea->Render();

	clInput->GetUserInput();
	clArea->Update();
}
void TV3DInterface::Cleanup()
{
	// A faire....  dans toutes les classes gros lache..!
}
bool TV3DInterface::AppStillIdle()
{
	MSG msg;
	return !PeekMessage(&msg, NULL, 0, 0, 0);
}





/****************
*	GET/SET Method
*****************/
CLTV3DMoteur* TV3DInterface::GetTV3D()
{
	clTV3D = CLTV3DMoteur::getInstance();
	return(clTV3D);
}
CLInput* TV3DInterface::GetInput()
{
	clInput = CLInput::getInstance();
	return(clInput);
}
CLArea* TV3DInterface::GetArea()
{
	clArea = CLArea::getInstance();
	return(clArea);
}
CLGlobalVar *TV3DInterface::GetGlobalVar()
{
	clGlobalVar = CLGlobalVar::getInstance();
	return(clGlobalVar);
}


