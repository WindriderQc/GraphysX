#include "Area.h"
#include <math.h>


// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier


CLArea *CLArea::_singleton = NULL;

CLArea::CLArea(void)
{
	clAtmos = NULL;
	clBallZ = NULL;


	pMath = new CTVMathLibrary();
	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	Init3DEnvironnement();
}

CLArea::~CLArea(void)
{
	clNotesManager->kill();

	if(clAtmos){ delete(clAtmos); clAtmos = NULL; }
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


	default:
				clTV3D->StartRenderingBlock();

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
					clCubXScene->clEffect->Start();     // Démarre l'explosion
				}
			else if(iMeshClickedIndex == clCubXScene->clCubeTools->GetMesh()->iMeshIndex)
				{
					CLTV3DCEGUI::getInstance()->SelectGUI(CARX_GUIMODE);
					SelectScene(NULL);
				}

			if(iMeshClickedIndex == clNotesManager->GetAddNoteBtnMesh()->iMeshIndex)
				{
					CLTV3DCEGUI::getInstance()->SelectGUI(NOTEMGR_GUIMODE);
					clNotesManager->AddNote();
				}

}

void CLArea::SelectScene(int iAreaMode)
{
	
//  IMPLEMENTER UN UNLOAD SCENE


	switch(iAreaMode)
	{
	
	case CAR_MODE:
		CLTV3DCEGUI::getInstance()->SelectGUI(NULL);
		clCarScene	= new CLCarScene();
		break;

	case CUBX_MODE:
		CLTV3DCEGUI::getInstance()->SelectGUI(NULL);
		clCubXScene = new CLCubXScene();
		break;

	case FLIGHTX_MODE:
		clFlightXScene = new CLFlightXScene();
		break;

	case BALLZ_MODE:
		break;

	default:
		break;
	
	}

iSelectedArea = iAreaMode;
}

//  A MODIFIER CAR LA FUNCTION EST FAITE POUR MOVER DE KOI AU DESSUS DU SOL SEULEMENT
void CLArea::UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land)
	{
		// We need a position for the Mesh
        cTV_3DVECTOR MeshPosition = Mesh->GetPosition();
		// We need a destination for the Mesh
        cTV_3DVECTOR MeshDestination = dest;
		// We need a direction for the Mesh to make it point in the right direction.
        cTV_3DVECTOR MeshDirection; 
		// We need angles for the Mesh
		float MeshAngleY;

		//  A checker voir si la function i.ToString()  marche pour tous les int en C++
		//Mesh = Scene.CreateMeshBuilder("Tank" + i.ToString());
        /*
		// Let set the initial position of the Mesh
		MeshPosition.x = (100f * (i+1)) - 300f;
		MeshPosition.z = 200f;
		MeshPosition.y = Land.GetHeight(MeshPosition.x, MeshPosition.z) + 10f;
		Mesh.SetPosition(MeshPosition.x, MeshPosition.y, MeshPosition.z);
		*/
				
		cTV_3DVECTOR dVector;// = new cTV_3DVECTOR();
		pMath->TVVec3Subtract( &dVector, &MeshDestination, &MeshPosition);
		pMath->TVVec3Normalize( &dVector, &dVector);
        MeshDirection = dVector;
		// Update the Mesh's angle
		if (MeshDirection.z > 0 ) 
			MeshAngleY = pMath->Rad2Deg(atan(MeshDirection.x / MeshDirection.z));
		else
			MeshAngleY = pMath->Rad2Deg(atan(MeshDirection.x / MeshDirection.z)) + 180;
				
		// Set the  mesh rotation
		Mesh->SetRotation(0.0f, MeshAngleY, 0.0f);
        
		
		// Check if Mesh has reached destination, if not, update the Mesh
		// position by adding a scale of the vector destination.
		if ((pMath->GetDistance3D(MeshPosition.x, 0, MeshPosition.z, MeshDestination.x, 0, MeshDestination.z)) > 2) 
		{
			// Update all the Mesh's position
			cTV_3DVECTOR dV2;
			pMath->TVVec3Scale( &dV2, &MeshDirection,(clTV3D->GetTimeElapsed() * 0.1f));
            pMath->TVVec3Add( &MeshPosition, &MeshPosition, &dV2);
			MeshPosition.y = Land->GetHeight(MeshPosition.x, MeshPosition.z) + 10;
			// Update the  mesh position
			Mesh->SetPosition(MeshPosition.x, MeshPosition.y, MeshPosition.z);
		}
		
	}