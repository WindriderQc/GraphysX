#include "StdAfx.h"
#include "Area.h"


// Un area contient:
// 1 GUI
// Une scene active
// etc....



// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier

//////// CREER UN function ShutDown Default Light...  pour permettre a l'effet de l'umiere de l'atomosphere de la scene de bien fonctionné


CLArea *CLArea::_singleton = NULL;

CLArea::CLArea(void)
{
	clCarScene = NULL;   // Modifier pour ete le pointeur 3DScene qui pointe vers la scene active
	clDefaultScene = NULL;
	clSkySelectScene = NULL;
	iActiveScene = NULL;
	pActiveScene = NULL;
	
	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	clClock = CLClock::getInstance();

	Init3DEnvironnement();
	//Load3DScene(BALLZ_SCENE);  // SKYBOX_SELECT_SCENE
	Load3DScene(MODULE_LOADING_SCENE);  // SKYBOX_SELECT_SCENE
	
	//CLTV3DCEGUI::getInstance()->SelectGUI(NULL);   //  SETUP_GUIMODE
	if(clGlobalVar->GetShowFPS())    //  A mettre dans un event du GUI pour le setter lors de la modif et non tout le temps!!!
		clTV3D->SetShowFPS(true);
	else clTV3D->SetShowFPS(false);
}

CLArea::~CLArea(void)
{
	if(clCarScene) { delete(clCarScene); clCarScene = NULL; }

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
	InitPhysics();
	
	// Init Atmosphere ici avant...   ou maintenant?
	
	p2DText = new CTVScreen2DText();
}
void CLArea::Load3DScene(int iSceneID)  
{
	if(pActiveScene != NULL)
	  {
	  delete(pActiveScene);   // Faudra revoir comme il faut les destructeur des classe scene car memory leak possible...
	  pActiveScene = NULL;
	  }

	switch(iSceneID)
	{
	case CAR_SCENE: 
			clCarScene	= new CLCarScene("Media\\Heightmaps\\RecTrack5.dds", "Media\\grass.jpg", "Media\\Sky\\Winter\\", "media\\sky\\clearnight\\");
			clCarScene->InitScene();
			pActiveScene = clCarScene;
//			CLTV3DCEGUI::getInstance()->SelectGUI(NULL);  // Devrait pas etre ici...  on load la scene, pas le gui
			break;
	
	//case MODULE_LOADING_SCENE:
	case SKYBOX_SELECT_SCENE:
			clSkySelectScene = new CLSkyboxSelectScene();
			clSkySelectScene->InitScene();
			pActiveScene = clSkySelectScene;
			break;

	case BALLZ_SCENE:
			clBallZScene = new CLBallZScene();
			clBallZScene->InitScene();
			pActiveScene = clBallZScene;
			break;

	case NO_SCENE:
	default:
			clDefaultScene = new CLDefaultScene("","","","");
			pActiveScene = clDefaultScene;
			break;
	}
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
    pTVPhysics->SetGlobalGravity(new cTV_3DVECTOR(0, -25,0)); //-9.800908285f, 0));
	pTVPhysics->SetWorldSize(new cTV_3DVECTOR(-10000,-10000,-10000),new cTV_3DVECTOR(10000,10000,10000));
	pTVPhysics->RenderDebugInfo(true);

}

void CLArea::Render(void)  
{
//  Render Water surfaces
// pActiveScene->UpdateWater();
clTV3D->StartRenderingBlock();
pActiveScene->Render(clClock->getTimeOfDay());
pTVPhysics->RenderDebugInfo(true);
// pScene->FinalizeShadows();
 Render2DText();

// Render CEGUI 2D Interface
//CLTV3DCEGUI::getInstance()->Render();  

clTV3D->StopRenderingBlock();
}
// Proceed to all actions on the area's members
void CLArea::Update(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY)
{
if(clGlobalVar->GetbUSE_TV_PHYSICS())
		pTVPhysics->Simulate(clTV3D->GetTimeElapsed() * 0.0025f);

pActiveScene->ManageCamCtrl(fWalk, fStrafe, fRaise, fMouseDeltaX, fMouseDeltaY, clTV3D->GetTimeElapsed());
}

//Display 2D Text on the interface
void CLArea::Render2DText(void)    //   Devrait ptete faire partie de la scene??  ou une classe a part pour lk'Affichage 2D
{
char BufferTime[256];
//char BufferTorqueX[256];
//char BufferTorqueY[256];
//char BufferTorqueZ[256];



p2DText->Action_BeginText(false);
	if(clGlobalVar->GetRender3DAxis())
		{
			p2DText->NormalFont_DrawText("X = White, Y = Green, Z = Red",5,20,RGBA(1,1,1,1),0);
		}
	if(clGlobalVar->GetRenderAtmos())
		{
			sprintf(BufferTime, "%d", int(clClock->getTimeOfDay()));  //  ptete enlever le int() pour mettre /1000  a la place
			p2DText->NormalFont_DrawText("Time of Day: ",5,32,RGBA(1,1,1,1),0);
			p2DText->NormalFont_DrawText(BufferTime,85,32,RGBA(1,1,1,1),0);	
		}
	
/*	vTorqueDisplay = clGlobalVar->GetTorqueValue();
	p2DText->NormalFont_DrawText("Torque: ",5,45,RGBA(1,1,1,1),0);
	sprintf(BufferTorqueX, "%f", vTorqueDisplay.x);
	p2DText->NormalFont_DrawText(BufferTorqueX,75,45,RGBA(1,1,1,1),0);
	sprintf(BufferTorqueY, "%f", vTorqueDisplay.y);
	p2DText->NormalFont_DrawText(BufferTorqueY,100,60,RGBA(1,1,1,1),0);
	sprintf(BufferTorqueZ, "%f", vTorqueDisplay.z);
	p2DText->NormalFont_DrawText(BufferTorqueZ,125,75,RGBA(1,1,1,1),0);
*/
p2DText->Action_EndText();
}
//  If the simulator detected that a mesh was clicked, the mesh ID is sent to the scene to detect if an action is associated.  
void CLArea::SendMeshClickedToScene(int iMeshClickedIndex)
{
	pActiveScene->MeshClickedAction(iMeshClickedIndex);
}