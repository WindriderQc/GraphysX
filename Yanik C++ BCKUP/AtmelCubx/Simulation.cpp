#include "simulation.h"



// Constructeur
Simulation::Simulation(void) 

{
	clCEGUI = NULL;
	
	
	clGlobalVar = CLGlobalVar::getInstance();
	clGlobalVar->ConfigBooleans();
	
	IsAtmelConnected = false;
}

// Destructeur
Simulation::~Simulation(void)
{
	
	clScreensaver->kill();
	clCEGUI->kill();
	clGlobalVar->kill();
	clArea->kill();
	clInput->kill();
	// Last Step, Kill the 3D Engine...
	clTV3D->kill();
}

// Méthode d'animation du simulateur
void Simulation::animer(void)
{
bool bIsMouseMoving;

float TimeElapsed = clTV3D->GetTimeElapsed();
	

switch(clArea->iSelectedArea)
{
case CAR_MODE:
			clArea->RenderArea(TimeElapsed);
			// 1- GET OPPONENT LOCATION ON NETWORK COMM
			// 2- UPDATE OPPONENT CAR POSITION IN SCENE
			
			//CLCarsManager::getInstance()-
			clArea->clCarScene->UpdateOpponentsCar();  
			clArea->clCarScene->clPlayerCar->ManageInput(); 

			break;

case CUBX_MODE:
			clGlobalVar->SetScreensaverRunning(clScreensaver->IsSaving());
			clArea->RenderArea(TimeElapsed);

			if(clGlobalVar->IsScreensaverEnabled())
			{
			bIsMouseMoving = clInput->stMouse.DeltaPosX + clInput->stMouse.DeltaPosY;
			clScreensaver->Update(bIsMouseMoving, TimeElapsed, clArea->clCubXScene->clCubX->GetMesh());
			}

			////  Section InputControl  ////
			clInput->GetUserKBInput();	
			clInput->GetUserMouseState();
			clCEGUI->GetGUISystem()->injectTimePulse(TimeElapsed);

	
			if(clScreensaver->IsSaving() == false)
			{		
				if(clGlobalVar->IsCEGUIWindowsOpened() == false)
					ManageGlobalAction(); 
					
				if(clGlobalVar->GetbCAM_CTRL())
					ManageCamAction(TimeElapsed);
			}
			////  fin Section InputControl  //// 


			break;

case BALLZ_MODE:
			clArea->RenderArea(TimeElapsed);
			
			clInput->GetUserKBInput();	
			clInput->GetUserMouseState();
			ManageBallAction();
					
			if(clGlobalVar->GetbCAM_CTRL())
			{
				ManageCamAction(TimeElapsed);

			}
			
			break;

case NOTEMGR_MODE:
			clArea->RenderArea(TimeElapsed);
			
			clInput->GetUserKBInput();	
			clInput->GetUserMouseState();
			ManageBallAction();
					
			if(clGlobalVar->GetbCAM_CTRL())
			{
				ManageCamAction(TimeElapsed);

			}
			
			break;

case FLIGHTX_MODE:
			clArea->RenderArea(TimeElapsed);
			clArea->clFlightXScene->ManagePlaneInput(TimeElapsed);

			//clInput->GetUserKBInput();	
			//clInput->GetUserMouseState();
			/*		
			if(clGlobalVar->GetbCAM_CTRL())
			{
				ManageCamAction(TimeElapsed);

			}
			*/
			break;

default:
			clArea->RenderArea(TimeElapsed);
			////  Section InputControl  ////
			clInput->GetUserKBInput();	
			clInput->GetUserMouseState();
			clCEGUI->GetGUISystem()->injectTimePulse(TimeElapsed);

}

}


// Méthode d'initialisation du simulateur
bool Simulation::initialiser(HWND WindowHandle, char *srchpath)
{
	WindowHWND = WindowHandle;
		
	clTV3D = CLTV3DMoteur::getInstance();
	clTV3D->Init3DEngine(WindowHandle, srchpath);
	
	clInput = CLInput::getInstance();
	
	clArea = CLArea::getInstance();
	clArea->SelectScene(CAR_MODE);
	
	clScreensaver = CLScreensaver::getInstance(); // Initialisation du Screensaver

	if(clGlobalVar->IsCEGUIEnabled())
	{
		clCEGUI = CLTV3DCEGUI::getInstance(); // Initialisation du GUI  
	}

	return true;
} 



void Simulation::ManageGlobalAction(void)
{

		if(clInput->bQuit == true)
			{
			 SendMessage(WindowHWND, WM_DESTROY, 0, 0);
			}	

	// Test si le bouton 1 de a souris a été clicker is si oui, test sur quel 
	// objet et agit en consequence..
	if(clInput->stMouse.ButtonDown != NULL)//IsMouseButtonPressed(0))  
	  {
		GetMeshClicked();
	  }
}

void Simulation::GetMeshClicked(void)
{
	clInput->pOldMeshClicked = clInput->pMeshClicked;
	clInput->pMeshClicked = NULL;
	pCollision = clTV3D->pTVScene->MousePick(clInput->stMouse.PosX, clInput->stMouse.PosY, 
										cTV_OBJECT_MESH, cTV_TESTTYPE_ACCURATETESTING );
	if(pCollision->IsCollision())
		clInput->pMeshClicked = pCollision->GetCollisionMesh();	
	
	if(clInput->pMeshClicked != NULL)
		clArea->MeshClickedAction(clInput->pMeshClicked->iMeshIndex);
}




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

void Simulation::ManageCamAction(float pTimeElapsed)
{

	cTV_3DVECTOR CamPos, CamLookAt;

	CamPos = clTV3D->pCamera->GetPosition();
	CamLookAt = clTV3D->pCamera->GetLookAt();

	float sngWalk = clInput->sngWalk;      clInput->sngWalk = 0;
	float sngStrafe = clInput->sngStrafe;  clInput->sngStrafe = 0;
	float sngRaise = clInput->sngRaise;    clInput->sngRaise = 0;
	float sngAngleY = 0;
	float sngAngleX = 0;


	//Okay, now for the smothing of the movement... We checked
    //above if we were pressing a key. If so, then we updated the
    //movement variable to 1 (positive or negative). Here, we
    //lower this value until it get to 0. This method give us a
    //smoother camera movement. We start by updating the forward
    //and backward (walk) movement
		if(sngWalk > 0)
		{
        sngWalk = sngWalk - 0.05f;
        if(sngWalk < 0) sngWalk = 0;
		}
		if(sngWalk < 0)
		{
        sngWalk = sngWalk + 0.05f;
        if(sngWalk > 0) sngWalk = 0;
		}
	 //Now, we update the left and right (strafe) movement.
		if(sngStrafe > 0)
		{
        sngStrafe = sngStrafe - 0.05f;
        if(sngStrafe < 0) sngStrafe = 0;
		}
		if(sngStrafe < 0)
		{
        sngStrafe = sngStrafe + 0.05f;
        if(sngStrafe > 0) sngStrafe = 0;
		}
	
	 //Now, we update the up and down (Raise) movement.
		if(sngRaise > 0)
		{
        sngRaise = sngRaise - 0.05f;
        if(sngRaise < 0) sngRaise = 0;
		}
		if(sngRaise < 0)
		{
        sngRaise = sngRaise + 0.05f;
        if(sngRaise > 0) sngRaise = 0;
		}

if(clGlobalVar->GetbMOUSE_CAM_CTRL())   // Changer pour mettre dans Globals et non Area
{
	long tmpMouseX = clInput->stMouse.DeltaPosX;
	long tmpMouseY = clInput->stMouse.DeltaPosY;

    //From the mouse return values, we update the camera angles
    //by adding or substracting the mouse return value.
    sngAngleX = sngAngleX - ((float)tmpMouseY / 100.0f);
    sngAngleY = sngAngleY - ((float)tmpMouseX / 100.0f);

    //We will add a simple check, so we can't look up at more
    //than 80 degrees nor down than -80 degrees.
    if(sngAngleX > 1.3f) sngAngleX = 1.3f;
    if(sngAngleX < -1.3f) sngAngleX = -1.3f;
    
	CamLookAt.x = CamPos.x + cosf(sngAngleY);
	CamLookAt.y = CamPos.y + tanf(sngAngleX);
	CamLookAt.z = CamPos.z + sinf(sngAngleY);
} // fin du if bCTRL_CAM


	//   A CHECKER CAR SNGAngle est jamais modifier !!?!?!?
	//Update the vectors using the angles and positions.
	CamPos.x = CamPos.x + (cosf(sngAngleY) * sngWalk * pTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * pTimeElapsed);
	CamPos.y = CamPos.y + (sngRaise * pTimeElapsed);  // A vérifier si l'équation est OK
	CamPos.z = CamPos.z + (sinf(sngAngleY) * sngWalk * pTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * pTimeElapsed);
	
	clTV3D->SetCameraPos(CamPos, CamLookAt);
	if(clArea->iSelectedArea == BALLZ_MODE)
		clTV3D->SetCameraPos(CamPos, clArea->clBallZ->GetBallMesh()->GetPosition());
}
