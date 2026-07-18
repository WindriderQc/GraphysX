#include "StdAfx.h"
#include "Scene.h"

//  A faire : Sortir la config initiale de la camera de la....
CLScene::CLScene(std::string PathToLand, std::string PathToLandTex)
{
	clLand = NULL;
	clWater = NULL;
	
	pTVMaths = new CTVMathLibrary();
    pTVMaterialfactory = new CTVMaterialFactory();
    pTVTexturefactory = new CTVTextureFactory();
    pTVPhysics = new CTVPhysics();       
    pTVScene = new CTVScene();
    pTVCamera = new CTVCamera();
	pTVAtmos = new CTVAtmosphere();
	
	InitTextures(); // Orders are important since textures are needed for landscape, etc...

	//  Create Terrain
	if(PathToLand != "")
	{
		float fWidth = (64 * 8) / 256;
		float fHeight = (64 * 8) / 256;
		clLand = new CLLand(PathToLand, PathToLandTex, fWidth, fHeight, cTV_3DVECTOR(-((fWidth * 256) / 2), -400, -((fHeight * 256) / 2)));
		InitPhysicsMaterials();
	}	

	pTVCamera->SetViewFrustum(45, 1000, 0.1f);
    pTVCamera->SetPosition(0, 300, -700);
    pTVCamera->SetLookAt(0, 300, 0);
}
CLScene::~CLScene(void)
{
	delete(clLand); clLand = NULL;
}
void CLScene::Render(long lTimeOfDay) 
{ /* Virtual Class*/ }
void CLScene::UpdateWater(long lTimeOfDay)
{
if(clWater != NULL)
	{
	clWater->StartReflectRender();
	Render(lTimeOfDay);
	clWater->StopReflectRender();

	clWater->StartRefractRender();
	Render(lTimeOfDay);
	clWater->StopRefractRender();
	}
}
void CLScene::MeshClickedAction(int iMeshClickedIndex)  
{ /* Virtual Class*/ }
void CLScene::InitPhysicsMaterials(void)
{
            //TerrainLandscape
            pmatTerrain = pTVPhysics->CreateMaterialGroup("Terrain");
            pTVPhysics->SetMaterialInteractionFriction(0, pmatTerrain, 0.9f, 1.0f);
            pTVPhysics->SetMaterialInteractionSoftness(0, pmatTerrain, 1.0f);
            pTVPhysics->SetMaterialInteractionBounciness(0, pmatTerrain, 0.1f);	
			pTVPhysics->SetBodyMaterialGroup(clLand->GetPhysicBody(), pmatTerrain);	
}

void CLScene::InitTextures()  // and materials
{
}
void CLScene::ManageCamCtrl(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY, float fTimeElapsed)
{ 	
	/* Virtual Class */
}
void CLScene::SetSkybox(CLSkybox* clSky)
{
clSkybox = clSky;
}
cTV_3DVECTOR CLScene::GenerateCerclePos(int Index, int MaxUnit, float Radius)
{
	float DegAngle = 360/MaxUnit * Index; 
	float RadAngle = pTVMaths->Deg2Rad(DegAngle);
	float x = cos(RadAngle) * Radius;
	float z = sin(RadAngle) * Radius;
	return(cTV_3DVECTOR(x,0,z));
}