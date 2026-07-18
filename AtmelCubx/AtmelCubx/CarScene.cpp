#include "CarScene.h"

CLCarScene::CLCarScene(void)
{
	int i;
	pTVMaths = new CTVMathLibrary();
    pTVMaterialfactory = new CTVMaterialFactory();
    pTVTexturefactory = new CTVTextureFactory();
    pTVPhysics = new CTVPhysics();       
    pTVScene = new CTVScene();
	pTVInput = new CTVInputEngine();
    pTVCamera = new CTVCamera();
	
	InitTextures(); // Orders are important since textures are needed for landscape, etc...
	
	//  Create Terrain
	float fWidth = (64 * 8) / 256;
	float fHeight = (64 * 8) / 256;
	clLand = new CLLand("Media\\Heightmaps\\CarHeightmap.bmp","Media\\grass.jpg",fWidth, fHeight, cTV_3DVECTOR(-((fWidth * 256) / 2), -400, -((fHeight * 256) / 2)));
	
	InitPhysicsMaterials();
	
	// Create Cars
	clPlayerCar = new CLVehicule(666, pmatTerrain);
	clPlayerCar->SetPosition(cTV_3DVECTOR(0,-300,0));
	clCarsManager->AddCar(clPlayerCar);
	

	for(i=0; i<NUMOPPONENTCARS; i++)
	{
	clOpponentCars[i] = new CLVehicule(i, pmatTerrain);
	clOpponentCars[i]->SetPosition(cTV_3DVECTOR(0,-300,float(10*i)));
	clCarsManager->AddCar(clOpponentCars[i]);
	//SetOriginalLocation(clCars[i], GetCarLocationVector());
	}


	pTVCamera->SetViewFrustum(45, 1000, 0.1f);
    pTVCamera->SetPosition(0, 5, -20);
    pTVCamera->SetLookAt(0, 3, 0);

	clCarsManager = CLCarsManager::getInstance();
	clCarsManager->setNbrCarsValue(NUMOPPONENTCARS);
	clCarsManager->Init(CLIENT);
	

}
CLCarScene::~CLCarScene(void)
{
	clCarsManager->CloseConnection();
	clCarsManager->kill();
}

void CLCarScene::Render(void)
{
	cTV_3DMATRIX Matrix;
	//Matrix = clPlayerCar->GetChassis()->GetMatrix();

	clPlayerCar->Render();

	int i;
	for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->Render();
	}
	
	cTV_3DVECTOR CameraAimVec;
	//Render Objets
	clLand->Render();
	pTVCamera->ChaseCamera(clPlayerCar->GetChassis(),new cTV_3DVECTOR(0,5,-15),new cTV_3DVECTOR(0,0,0),1000,true,-1);

}

void CLCarScene::UpdateOpponentsCar(void)
{
clCarsManager->GetPlayersInput();

int i;
	
for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->SetEngineValues(stOpponentsEng[i]);
	}

}

void CLCarScene::InitPhysicsMaterials(void)
{
            //TerrainLandscape
            pmatTerrain = pTVPhysics->CreateMaterialGroup("Terrain");
            pTVPhysics->SetMaterialInteractionFriction(0, pmatTerrain, 0.9f, 1.0f);
            pTVPhysics->SetMaterialInteractionSoftness(0, pmatTerrain, 1.0f);
            pTVPhysics->SetMaterialInteractionBounciness(0, pmatTerrain, 0.1f);
			pTVPhysics->SetBodyMaterialGroup(clLand->GetPhysicBody(), pmatTerrain);
}

void CLCarScene::InitTextures()  // and materials
{
            //Sky Box
            texDayUp = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\up.jpg", "SkyTop");
            texDayDown = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\down.jpg", "SkyBottom");
            texDayLeft = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\left.jpg", "SkyLeft");
            texDayRight = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\right.jpg", "SkyRight");
            texDayFront = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\front.jpg", "SkyFront");
            texDayBack = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\back.jpg", "SkyBack");
}