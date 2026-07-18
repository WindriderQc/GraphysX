#include "Area.h"
#include <math.h>

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
	pMath = new CTVMathLibrary();
	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	clClock = CLClock::getInstance();

	Init3DEnvironnement();
	Load3DScene(NO_SCENE);
	
	CLTV3DCEGUI::getInstance()->SelectGUI(SETUP_GUIMODE);
	
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
	
	// Mesh qui peux etre mis a la meme position qu'un autre.  En fesant suivre CameraAim par la camera, ceci 
	// permet de faire rotationner l'objet que l'on regarde sans que la camera ne suive la rotation.
	pCameraAim = pScene->CreateMeshBuilder("CameraAim");
	pCameraAim->CreateBox(1,1,1, false);


	p2DText = new CTVScreen2DText();
}
void CLArea::Load3DScene(int iSceneID)  
{
//  Mauvaise technique...  ca va prendre une facon de UnloadScene car sinon en changeant d'une scene a l'autre...   la mem va craquer...

	switch(iSceneID)
	{
	case CAR_SCENE: 
			clCarScene	= new CLCarScene("Media\\Heightmaps\\RecTrack5.dds", "Media\\grass.jpg", "Media\\Sky\\Winter\\", "media\\sky\\clearnight\\");
			clCarScene->InitScene();
			CLTV3DCEGUI::getInstance()->SelectGUI(NULL);  // Devrait pas etre ici...  on load la scene, pas le gui
			iActiveScene = CAR_SCENE;
			break;
	
	//case MODULE_LOADING_SCENE:
	case SKYBOX_SELECT_SCENE:
			clSkySelectScene = new CLSkyboxSelectScene();
			clSkySelectScene->InitScene();
			iActiveScene = SKYBOX_SELECT_SCENE;
			break;
	
	case NO_SCENE:
	default:
			clDefaultScene = new CLDefaultScene("","","","");
			iActiveScene = NO_SCENE;
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
    pTVPhysics->SetGlobalGravity(new cTV_3DVECTOR(0, -9.800908285f, 0));
	pTVPhysics->SetWorldSize(new cTV_3DVECTOR(-10000,-10000,-10000),new cTV_3DVECTOR(10000,10000,10000));
}

void CLArea::Render(void)  
{

cTV_3DVECTOR CameraAimVec;


clTV3D->StartRenderingBlock();

// Modifier pour avoir un pointer 3DScene qui pointera vers la scene active pour éliminer une phase conditionelle
switch(iActiveScene)
{
case CAR_SCENE:
	//Render Objects
	clCarScene->Render(clClock->getTimeOfDay());
	pScene->FinalizeShadows();
	if(clGlobalVar->GetRender3DAxis())
		clTV3D->DrawTV3DAxis();
	Render2DText();
	break;

case MODULE_LOADING_SCENE: 
	
	break;
case SKYBOX_SELECT_SCENE: 
	clSkySelectScene->Render(clClock->getTimeOfDay());

	break;
default:
	clDefaultScene->Render(clClock->getTimeOfDay());
	break;
}


// Render CEGUI 2D Interface
CLTV3DCEGUI::getInstance()->Render();  

clTV3D->StopRenderingBlock();

if(clGlobalVar->GetbUSE_TV_PHYSICS())
		pTVPhysics->Simulate(clTV3D->GetTimeElapsed() * 0.0025f);
}


//	Display anyway 2D Text on the interface
void CLArea::Render2DText(void)    //   Devrait ptete faire partie de la scene??  ou une classe a part pour lk'Affichage 2D
{
char BufferTime[256];

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
		sprintf(BufferTime, "%d", int(clClock->getTimeOfDay()));  //  ptete enlever le int() pour mettre /1000  a la place
		p2DText->NormalFont_DrawText("Time of Day: ",5,32,RGBA(1,1,1,1),0);
		p2DText->NormalFont_DrawText(BufferTime,85,32,RGBA(1,1,1,1),0);	
	}

//////////////////////////////
p2DText->Action_EndText();
}

//Avant yavait ici Mesh CLick action()      
void CLArea::MeshClickedAction(int iMeshClickedIndex)
{

	// Modifier pour avoir un pointer 3DScene qui pointera vers la scene active pour éliminer une phase conditionelle
	switch(iActiveScene)
	{
	case CAR_SCENE:

		break;

	case MODULE_LOADING_SCENE: 
		break;
	case SKYBOX_SELECT_SCENE: 
		clSkySelectScene->MeshClickedAction(iMeshClickedIndex);
		break;
	default:
		break;
	}

}

//  A MODIFIER CAR LA FUNCTION EST FAITE POUR MOVER DE KOI AU DESSUS DU SOL SEULEMENT et mettre dans Scene pas dans area!!!
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