#include "Area.h"

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier


CLArea *CLArea::_singleton = NULL;

CLArea::CLArea(void)
{
	clAtmos = NULL;
	
	Snowfall = NULL;
	SmokeFest = NULL;
	clExplosion = NULL;
	clBallZ = NULL;

	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	Init3DEnvironnement();
}

CLArea::~CLArea(void)
{

	clNotesManager->kill();

	if(clAtmos){ delete(clAtmos); clAtmos = NULL; }
	if(Snowfall){ delete(Snowfall); Snowfall = NULL; }
	if(SmokeFest){ delete(SmokeFest); SmokeFest = NULL; }
	
	
	if(clExplosion) { delete (clExplosion); clExplosion = NULL; }
	if(clBallZ) { delete (clBallZ); clBallZ = NULL; }


	 // Clean up the TV scene 
    if( pScene ) 
    { 
      pScene->DestroyAllMeshes(); 
      delete(pScene); pScene = NULL; 
    } 
	
}



void CLArea::Init3DEnvironnement(void)
{
	InitTVScene();
	InitLightAndMaterial();
	
	if(clGlobalVar->GetbUSE_TV_PHYSICS())
		InitPhysics();
	
	if(clGlobalVar->GetbLOAD_ATMOSPHERE()) 
	{
		clAtmos = new CLAtmosphere(); 
		clWater = new CLWater();
	}

	// Mesh qui peux etre mis a la meme position qu'un autre.  En fesant suivre CameraAim par la camera, ceci 
	// permet de faire rotationner l'objet que l'on regarde sans que la camera ne suive la rotation.
	pCameraAim = pScene->CreateMeshBuilder("CameraAim");
	pCameraAim->CreateBox(1,1,1, false);
	
	clExplosion = new CLExplosion();

	// Load all different Scenes
	clCubXScene = new CLCubXScene();
	clFlightXScene = new CLFlightXScene();

	clNotesManager = CLNotesManager::getInstance();	
	p2DText = new CTVScreen2DText();
}

void CLArea::InitTVScene(void)
{
	pScene = new CTVScene();
	pScene->SetRenderMode(cTV_SOLID);
    pScene->SetShadowParameters(RGBA(0.0, 0.0, 0.0, 0.5), false);
}

void CLArea::InitLightAndMaterial(void)
{ 
	pLights = new CTVLightEngine();

	cTV_3DVECTOR VecDir;
	pLights->SetGlobalAmbient(0,0,0);

	VecDir.x = -1; VecDir.y = -1; VecDir.z = 1;
	iMainLight = pLights->CreateDirectionalLight(&VecDir,1,1,1,"GlobalLight",1.0);
	pLights->SetSpecularLighting(true);
	pLights->SetLightProperties(iMainLight,true,true,false);
}
void CLArea::InitPhysics(void)
{
	pTVPhysics = new CTVPhysics();
	pTVPhysics->Initialize();
    pTVPhysics->SetSolverModel(cTV_SOLVER_EXACT);
    pTVPhysics->SetFrictionModel(cTV_FRICTION_EXACT);
    pTVPhysics->SetGlobalGravity(new cTV_3DVECTOR(0, -9.800908285f, 0));
	pTVPhysics->SetWorldSize(new cTV_3DVECTOR(-10000,-10000,-10000),new cTV_3DVECTOR(10000,10000,10000));
}

void CLArea::RenderArea(float TimeElapsed)
{
cTV_3DVECTOR CameraAimVec;
float SpeedFactor = TimeElapsed / 1000; // pour avoir un speedfactor en seconde  

switch(iSelectedArea)
{
	case CAR_MODE:
				clTV3D->StartRenderingBlock();
				//Render Atmosphere
				if(clGlobalVar->GetRenderAtmos())   // A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
				{
				clAtmos->UpdateAndRender(TimeElapsed);
				clWater->Render();
				}
				//Render Objects
				clCarScene->Render();
				break;

	case CUBX_MODE:
				//  Update some infos before rendering
				clCubXScene->Refresh3DClock();
				clCubXScene->clMenuManager->SetActiveMenu();
		
		
				if(clGlobalVar->GetRenderAtmos()) 
				{
				// Pre Render everything reflecting in the water
				clWater->StartReflectRender();
				clAtmos->pTVAtmos->Atmosphere_Render();
				clCubXScene->Render();
				clWater->StopReflectRender();
				clWater->StartRefractRender();
				clAtmos->pTVAtmos->Atmosphere_Render();
				clCubXScene->Render();
				clWater->StopRefractRender();

				//////// Start rendering block
				clTV3D->StartRenderingBlock();
			
				clAtmos->UpdateAndRender(TimeElapsed);
				clWater->Render();
				}
				else
				{
				//////// Start rendering block
				clTV3D->StartRenderingBlock();
				}

				clCubXScene->Render();			

				clNotesManager->Render();		
				if(clExplosion->IsFinished() == false)
					clExplosion->Render(TimeElapsed); 

				break;

	case BALLZ_MODE:
				clTV3D->StartRenderingBlock();
				clBallZ->Render();
				CameraAimVec = clBallZ->GetBallPosition();  // Donne une cible a la camera pour etre indépendant des moves de la balle
				pCameraAim->SetPosition(CameraAimVec.x, CameraAimVec.y, CameraAimVec.z);
				clTV3D->pCamera->ChaseCamera(pCameraAim,new cTV_3DVECTOR(0,350,300),new cTV_3DVECTOR(0,0,0),1000,true,-1);
				break;

	case FLIGHTX_MODE:
				clTV3D->StartRenderingBlock();
				//Render Atmosphere
				if(clGlobalVar->GetRenderAtmos())   // A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
				{
				clAtmos->UpdateAndRender(TimeElapsed);
				//clWater->Render();
				}
				
				clFlightXScene->Render();
				CameraAimVec = clFlightXScene->GetPlanePos();  // Donne une cible a la camera pour etre indépendant des moves de la balle
				pCameraAim->SetPosition(CameraAimVec.x, CameraAimVec.y, CameraAimVec.z);
				clTV3D->pCamera->ChaseCamera(clFlightXScene->GetPlaneMesh(),new cTV_3DVECTOR(0,250,-800),new cTV_3DVECTOR(0,0,0),1000,true,-1);
				break;


}

pScene->FinalizeShadows();

if(clGlobalVar->GetRender3DAxis())
	clTV3D->DrawTV3DAxis();

Render2DText();


// Render CEGUI 2D Interface
if(!clGlobalVar->IsScreensaverRunning())
		  {
			  CLTV3DCEGUI::getInstance()->Render();  
		  }


clTV3D->StopRenderingBlock();

if(clGlobalVar->GetbUSE_TV_PHYSICS())
		pTVPhysics->Simulate(TimeElapsed * 0.0025f);
}


//	Display anyway 2D Text on the interface
void CLArea::Render2DText(void)
{
char BufferTime[256];
char BufferBallSpeed[256];
cTV_3DVECTOR BallSpeedVec;

if(clGlobalVar->GetShowFPS())
		clTV3D->SetShowFPS(true);
	else clTV3D->SetShowFPS(false);

p2DText->Action_BeginText(false);
////////////////////////////////

if(clGlobalVar->GetRender3DAxis())
	{
		p2DText->NormalFont_DrawText("X = White, Y = Green, Z = Red",5,20,RGBA(1,1,1,1),0);
	}
if(clGlobalVar->GetRenderAtmos())
	{
		sprintf(BufferTime, "%d", int(clAtmos->lTimeOfDay));  //  ptete enlever le int() pour mettre /1000  a la place
		p2DText->NormalFont_DrawText("Time of Day: ",5,32,RGBA(1,1,1,1),0);
		p2DText->NormalFont_DrawText(BufferTime,85,32,RGBA(1,1,1,1),0);	
	}
	if(iSelectedArea == BALLZ_MODE)
	{
		BallSpeedVec = clBallZ->GetBallSpeed();
		sprintf(BufferBallSpeed, "%d, %d, %d", int(BallSpeedVec.x), int(BallSpeedVec.y), int(BallSpeedVec.z));
		p2DText->NormalFont_DrawText("Ball Speed: ",5,44,RGBA(1,1,1,1),0);
		p2DText->NormalFont_DrawText(BufferBallSpeed,85,44,RGBA(1,1,1,1),0);	
	}

//////////////////////////////
p2DText->Action_EndText();
}



void CLArea::MeshClickedAction(int iMeshClickedIndex)
{
	clCubXScene->CubXClickedAction(iMeshClickedIndex);
	clCubXScene->DomoticClickedAction(iMeshClickedIndex);
			
	if(iMeshClickedIndex == clCubXScene->pSoleil->iMeshIndex)
				{
				/*	iSelectedArea = BALLZ_MODE;
					clBallZ = new CLBallZ();
					//clGlobalVar->SetbCAM_CTRL(false);
				*/
					iSelectedArea = FLIGHTX_MODE;
				}
			
			if(iMeshClickedIndex == clCubXScene->clCubeSys->GetMesh()->iMeshIndex)  //  Menu 2
				{
					//clMenuManager->iAncienNiveau = clMenuManager->iNiveauActif;
					//clMenuManager->iNiveauActif = 2; // info sys
					clExplosion->Start();     // Démarre l'explosion
				}
			else if(iMeshClickedIndex == clCubXScene->clCubeTools->GetMesh()->iMeshIndex)
				{
					clCarScene	= new CLCarScene();
					iSelectedArea = CAR_MODE;
				}

			if(iMeshClickedIndex == clNotesManager->GetAddNoteBtnMesh()->iMeshIndex)
				{
					CLTV3DCEGUI::getInstance()->EnableNoteBloc();
					clNotesManager->AddNote();
				}

}