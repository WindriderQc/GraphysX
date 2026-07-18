#include "TV3DMoteur.h"
#include <stdio.h>

#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier


CLTV3DMoteur *CLTV3DMoteur::_singleton = NULL;


/*Constructeur de la classe*/
CLTV3DMoteur::CLTV3DMoteur(void)
{
	clGlobalVar = CLGlobalVar::getInstance();

}

/*Destructeur de la classe*/
CLTV3DMoteur::~CLTV3DMoteur(void)
{
	clGlobalVar->kill();
	
	if(pCamera){ delete(pCamera); pCamera = NULL; }

	pTV->ReleaseAll();
    // Destroy the engine last 
    if( pTV ) { delete(pTV); pTV = NULL; } 		
}

void CLTV3DMoteur::Init3DEngine(void)
{
	char * PathToApp = clGlobalVar->GetSearchPath();

	// Create the pTV Interface first:
	pTV = new CTVEngine();

	pTV->AllowMultithreading(true);
	
	pTV->SetSearchDirectory(PathToApp);
	// Set the debug file/options.
	// Do this before the 3D init so it can log any errors found during init.
	pTV->SetDebugMode(true,true);
	pTV->SetDebugFile(strcat(PathToApp, "\\debugfile.txt"));

	// Its time to init the engine:
	pTV->Init3DWindowed(clGlobalVar->Get3DWindowHandle(), true);

	pTV->SetAntialiasing(true,cTV_MULTISAMPLE_2_SAMPLES);

	// Something good to do is to enable the auto-resize feature:
	// Get the default viewport and set autoresize to true for it:
	pTV->GetViewport()->SetAutoResize(true);


	// Set the prefered angle system:
	pTV->SetAngleSystem(cTV_ANGLE_DEGREE);
	pTV->ShowWinCursor(true);

	//pTV->SetVSync(false);  // tester les 2 pour voir la différence...
	//pTV->EnableProfiler(false, false); // tester les 2 pour voir la différence...

	pTVScene = new CTVScene();

	InitCamera();
	
}



void CLTV3DMoteur::InitCamera(void)
{
	cTV_3DVECTOR CamPos; 
	cTV_3DVECTOR CamLookAt;
	CamPos.x = 400;
	CamPos.y = 250;
	CamPos.z = -2000;		
	CamLookAt.x = 0;
	CamLookAt.y = 200;
	CamLookAt.z = 0;

	pCamera = new CTVCamera();
    pCamera->SetViewFrustum(60, 20000);
	SetCameraPos(CamPos, CamLookAt);
}

void CLTV3DMoteur::ResetCamera(void)
{
	cTV_3DVECTOR CamPos; 
	cTV_3DVECTOR CamLookAt;
	CamPos.x = 400;
	CamPos.y = 250;
	CamPos.z = -2000;		
	CamLookAt.x = 0;
	CamLookAt.y = 200;
	CamLookAt.z = 0;

	SetCameraPos(CamPos, CamLookAt);
}
void CLTV3DMoteur::SetCameraPos(cTV_3DVECTOR campos, cTV_3DVECTOR camlookat)
{
    pCamera->SetCamera(campos.x, campos.y, campos.z, camlookat.x, camlookat.y, camlookat.z);
}


float CLTV3DMoteur::GetTimeElapsed(void)
{
return(pTV->AccurateTimeElapsed());
}

void CLTV3DMoteur::SetShowFPS(bool bEnabled)
{
	pTV->DisplayFPS(bEnabled,0);
}





void CLTV3DMoteur::StartRenderingBlock(void)
{
	pTV->Clear(false);
}
void CLTV3DMoteur::StopRenderingBlock(void)
{
	pTV->RenderToScreen();
}