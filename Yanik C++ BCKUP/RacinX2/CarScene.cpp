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
	clLand = new CLLand("Media\\Heightmaps\\RecTrack5.dds","Media\\grass.jpg",fWidth, fHeight, cTV_3DVECTOR(-((fWidth * 256) / 2), -400, -((fHeight * 256) / 2)));
	
	InitPhysicsMaterials();
	InitAtmosphere();
	
	//  Loading and initing the Car Manager
	clCarsManager = CLCarsManager::getInstance();
	clCarsManager->setNbrCarsValue(NUMOPPONENTCARS);
	clCarsManager->Init(CLIENT);

	// Create Cars
	clPlayerCar = new CLVehicule(666, pmatTerrain);
	clPlayerCar->SetPosition(cTV_3DVECTOR(50,-350,0));
	clCarsManager->AddCar(clPlayerCar);
/*
	for(i=0; i<NUMOPPONENTCARS; i++)
	{
	clOpponentCars[i] = new CLVehicule(i, pmatTerrain);
	clOpponentCars[i]->SetPosition(cTV_3DVECTOR(50,-350,float(5*i)));
	clCarsManager->AddCar(clOpponentCars[i]);
	//SetOriginalLocation(clCars[i], GetCarLocationVector());
	}
*/

	/////////
	//  Loading des Rings dans le jeux.....   a paufiner
	////////////

	clAnneaux = new CLAnneau();  //  modifier pour avoir en param le nom de fichier xml a loader.  Ou un tableau prédéfini....

for(i=0;i<100;i++)
  {
	VecPosAnneaux[i].x = 0 + i*10;   // Valeur de offset pour le fun de les voir croche au démarrage :)
	VecPosAnneaux[i].y = 0  -397;
	VecPosAnneaux[i].z = 0 + i*10;
	//  Positionne les anneaux dans le tableau  // implementer pour avoir un XML qui defini les 100 positions
	clAnneaux->SetRingPos(VecPosAnneaux[i],i);
  }
 //////////////

	pTVCamera->SetViewFrustum(45, 1000, 0.1f);
    pTVCamera->SetPosition(0, 5, -20);
    pTVCamera->SetLookAt(0, 3, 0);
}
CLCarScene::~CLCarScene(void)
{
	delete(clAtmos); clAtmos = NULL; 
	delete(clWater); clWater = NULL; 
	//clCarsManager->CloseConnection();
	clCarsManager->kill();
}

void CLCarScene::Render(float TimeElapsed, long lTimeOfDay)
{
	//Render Atmosphere
    // A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
	clAtmos->UpdateAndRender(TimeElapsed, lTimeOfDay);
	clWater->Render();


	cTV_3DMATRIX Matrix;
	//Matrix = clPlayerCar->GetChassis()->GetMatrix();

	clPlayerCar->Render();
/*
	int i;
	for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->Render();
	}
*/
	cTV_3DVECTOR CameraAimVec;
	//Render Objets
	clLand->Render();
	clAnneaux->Render();
	pTVCamera->ChaseCamera(clPlayerCar->GetChassis(),new cTV_3DVECTOR(0,5,-15),new cTV_3DVECTOR(0,0,0),1000,true,-1);

}

void CLCarScene::UpdateOpponentsCar(void)
{
clCarsManager->GetPlayersInput();

int i;
	
for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->SetEngineValues(stOpponentsEng[i]);// Des fois ca peut planter car rien d'initialiser pour les PlayersInput si ya rien..
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
          
			
			
}
void CLCarScene::InitSkyBoxes(char* DaySkyPath, char* NightSkyPath)
{
            //Sky Box
            stSkyboxDay.SkyTop = pTVTexturefactory->LoadTexture(*DaySkyPath + "up.jpg", "SkyTop");
            stSkyboxDay.SkyBottom = pTVTexturefactory->LoadTexture(*DaySkyPath + "down.jpg", "SkyBottom");
            stSkyboxDay.SkyLeft = pTVTexturefactory->LoadTexture(*DaySkyPath + "left.jpg", "SkyLeft");
            stSkyboxDay.SkyRight = pTVTexturefactory->LoadTexture(*DaySkyPath + "right.jpg", "SkyRight");
            stSkyboxDay.SkyFront = pTVTexturefactory->LoadTexture(*DaySkyPath + "front.jpg", "SkyFront");
            stSkyboxDay.SkyBack = pTVTexturefactory->LoadTexture(*DaySkyPath + "back.jpg", "SkyBack");

			stSkyboxNight.SkyTop = pTVTexturefactory->LoadTexture(*NightSkyPath + "up.jpg", "NightSkyTop");
            stSkyboxNight.SkyBottom = pTVTexturefactory->LoadTexture(*NightSkyPath + "down.jpg", "NightSkyBottom");
            stSkyboxNight.SkyLeft = pTVTexturefactory->LoadTexture(*NightSkyPath + "left.jpg", "NightSkyLeft");
            stSkyboxNight.SkyRight = pTVTexturefactory->LoadTexture(*NightSkyPath + "right.jpg", "NightSkyRight");
            stSkyboxNight.SkyFront = pTVTexturefactory->LoadTexture(*NightSkyPath + "front.jpg", "NightSkyFront");
            stSkyboxNight.SkyBack = pTVTexturefactory->LoadTexture(*NightSkyPath + "back.jpg", "NightSkyBack");

}
void CLCarScene::InitAtmosphere()
{
	InitSkyBoxes("Media\\Sky\\Winter\\", "media\\sky\\clearnight\\");
	clAtmos = new CLAtmosphere(stSkyboxDay, stSkyboxNight); 
	clWater = new CLWater();
}