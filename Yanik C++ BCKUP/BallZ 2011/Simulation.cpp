#include "StdAfx.h"
#include "simulation.h"

// Constructeur
Simulation::Simulation(void) 
{
	clGlobalVar = CLGlobalVar::getInstance();
}
// Destructeur
Simulation::~Simulation(void)
{
	//clScreensaver->kill();
//	clCEGUI->kill();
	clGlobalVar->kill();
	clArea->kill();
	clInput->kill();
	clTV3D->kill();  // Last Step, Kill the 3D Engine...
}

// Méthode d'initialisation du simulateur
bool Simulation::initialiser(void)
{
	clTV3D = CLTV3DMoteur::getInstance();    //  Create the TV3D engine singleton.
	clTV3D->Init3DEngine();  // Initialize TV3D 
	
//	clCEGUI = CLTV3DCEGUI::getInstance(); // Initialisation du GUI 
	clInput = CLInput::getInstance();   //  Create the Input singleton.
	clArea = CLArea::getInstance();    //  Create the Area singleton.
//	clScreensaver = CLScreensaver::getInstance(); // Create the Screensaver singleton.

	 
	
	return true;
} 
// Méthode d'animation du simulateur
void Simulation::animer(void)
{
	clArea->Render();  
	////  Section InputControl  ////
	clInput->GetUserInput();
	clInput->CheckGamePadInput();
//	clCEGUI->GetGUISystem()->injectTimePulse(clTV3D->GetTimeElapsed());
	ManageGlobalIOAction();
	clArea->Update(clInput->sngWalk, clInput->sngStrafe, clInput->sngRaise, clInput->stMouse.DeltaPosX, clInput->stMouse.DeltaPosY );
	clInput->ResetFlags();
}
////   METTRE LES FONCTION D'ACTION AILLEUR serait plus propre//////////////////////

void Simulation::ManageGlobalIOAction(void)
{
/*
		if(clInput->bQuit == true)
			{
			 SendMessage(WindowHWND, WM_DESTROY, 0, 0);
			}	
*/
	// Test si le bouton 1 de a souris a été clicker is si oui, test sur quel 
	// objet et agit en consequence..
	if(clInput->stMouse.ButtonDown != NULL)//IsMouseButtonPressed(0))  
	  {
		CheckMeshClicked();
	  }
}

void Simulation::CheckMeshClicked(void)
{
	clInput->pOldMeshClicked = clInput->pMeshClicked;
	clInput->pMeshClicked = NULL;
	pCollision = clTV3D->pTVScene->MousePick(clInput->stMouse.PosX, clInput->stMouse.PosY, 
										cTV_OBJECT_MESH, cTV_TESTTYPE_ACCURATETESTING );
	if(pCollision->IsCollision())
		clInput->pMeshClicked = pCollision->GetCollisionMesh();	
	
	if(clInput->pMeshClicked != NULL)
		clArea->SendMeshClickedToScene(clInput->pMeshClicked->iMeshIndex);
}



/*
void Simulation::ManageBallAction(void)
{
if(clInput->iPushBallZ == 1)
	{
	clArea->clBallZ->PushBall(FRONT);
	clInput->iPushBallZ = 0;
	}
else if(clInput->iPushBallX == -1)
	{
	clArea->clBallZ->PushBall(LEFT);
	clInput->iPushBallX = 0;
	}
else if(clInput->iPushBallX == 1)
	{
	clArea->clBallZ->PushBall(RIGHT);
	clInput->iPushBallX = 0;
	}
else if(clInput->iPushBallZ == -1)
	{
	clArea->clBallZ->PushBall(BACK);
	clInput->iPushBallZ = 0;
	}


if(clInput->bQuit == true)
	{
	SendMessage(WindowHWND, WM_DESTROY, 0, 0);
	}	


}



*/