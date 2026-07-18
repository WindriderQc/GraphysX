#include "CarScene.h"

CLCarScene::CLCarScene(void)
{
	pTVMaths = new CTVMathLibrary();
    pTVMaterialfactory = new CTVMaterialFactory();
    pTVTexturefactory = new CTVTextureFactory();
    pTVPhysics = new CTVPhysics();       
    pTVScene = new CTVScene();
	pTVInput = new CTVInputEngine();
    pTVCamera = new CTVCamera();
	
	InitTextures(); // Orders are important since textures are needed for landscape, etc...
	InitLandscape();
	InitPhysicsMaterials();
	

	clPlayerCar = new CLVehicule(666, pmatTerrain);

	int i;
	for(i=0; i<NUMOPPONENTCARS; i++)
	{
	clOpponentCars[i] = new CLVehicule(i, pmatTerrain);
	clOpponentCars[i]->SetPosition(cTV_3DVECTOR(0,0,float(10*i)));
	//SetOriginalLocation(clCars[i], GetCarLocationVector());
	}

	pTVCamera->SetViewFrustum(45, 1000, 0.1f);
    pTVCamera->SetPosition(0, 5, -20);
    pTVCamera->SetLookAt(0, 3, 0);

	clCarsManager = CLCarsManager::getInstance();
	clCarsManager->setNbrCarsValue(NUMOPPONENTCARS);

}
CLCarScene::~CLCarScene(void)
{
	clCarsManager->kill();
}

void CLCarScene::Render(void)
{
	clPlayerCar->Render();

	int i;
	for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->Render();
	}
	
	cTV_3DVECTOR CameraAimVec;
	//Render Objets
	pLand->Render();
	pTVCamera->ChaseCamera(clPlayerCar->GetChassis(),new cTV_3DVECTOR(0,5,-15),new cTV_3DVECTOR(0,0,0),1000,true,-1);

}

void CLCarScene::InitLandscape(void)
{
    pLand = pTVScene->CreateLandscape("Land");
            
    pLand->SetAffineLevel(cTV_AFFINE_LOW);
	float fWidth = (64 * 8) / 256;
	float fHeight = (64 * 8) / 256;


	pLand->GenerateTerrain("Media\\Heightmaps\\CarHeightmap.bmp", cTV_PRECISION_HIGH, int(fWidth), int(fHeight), 0, 0, 0,true);

	pLand->SetTexture(texGrass);
	pLand->SetMaterial(matLand);
	pLand->SetTextureScale(10, 10);
	pLand->SetPosition(-((fWidth * 256) / 2), -200, -((fHeight * 256) / 2));

	pbLand = pTVPhysics->CreateStaticTerrainBody(pLand);
		pTVPhysics->EnableDebugInfo(pbLand,true,-1);
}


void CLCarScene::InitPhysicsMaterials(void)
{
            //TerrainLandscape
            pmatTerrain = pTVPhysics->CreateMaterialGroup("Terrain");
            pTVPhysics->SetMaterialInteractionFriction(0, pmatTerrain, 0.9f, 1.0f);
            pTVPhysics->SetMaterialInteractionSoftness(0, pmatTerrain, 1.0f);
            pTVPhysics->SetMaterialInteractionBounciness(0, pmatTerrain, 0.1f);
            pTVPhysics->SetBodyMaterialGroup(pbLand, pmatTerrain);
}

void CLCarScene::InitTextures()  // and materials
        {
            //Land
            texGrass = pTVTexturefactory->LoadTexture("Media\\grass.jpg", "Grass");

            //Sky Box
            texDayUp = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\up.jpg", "SkyTop");
            texDayDown = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\down.jpg", "SkyBottom");
            texDayLeft = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\left.jpg", "SkyLeft");
            texDayRight = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\right.jpg", "SkyRight");
            texDayFront = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\front.jpg", "SkyFront");
            texDayBack = pTVTexturefactory->LoadTexture("Media\\Sky\\Winter\\back.jpg", "SkyBack");

			
			
			
			//Create Materials
            matLand = pTVMaterialfactory->CreateMaterial("land");
            //Land
            pTVMaterialfactory->SetSpecular(matLand, 0.1f, 0.1f, 0.1f, 1.0f);      


}