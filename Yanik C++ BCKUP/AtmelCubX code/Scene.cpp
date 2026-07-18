#include "Scene.h"

//  A faire : Sortir la config initiale de la camera de la....
CLScene::CLScene(std::string PathToLand, std::string PathToLandTex, std::string PathToDaySky, std::string PathToNightSky)
{
	clLand = NULL;
	clAtmos = NULL;
	clWater = NULL;
	
	pTVMaths = new CTVMathLibrary();
    pTVMaterialfactory = new CTVMaterialFactory();
    pTVTexturefactory = new CTVTextureFactory();
    pTVPhysics = new CTVPhysics();       
    pTVScene = new CTVScene();
    pTVCamera = new CTVCamera();
	
	InitTextures(); // Orders are important since textures are needed for landscape, etc...

	//  Create Terrain
	if(PathToLand != "")
	{
	float fWidth = (64 * 8) / 256;
	float fHeight = (64 * 8) / 256;
	clLand = new CLLand(PathToLand, PathToLandTex, fWidth, fHeight, cTV_3DVECTOR(-((fWidth * 256) / 2), -400, -((fHeight * 256) / 2)));
	InitPhysicsMaterials();
	clWater = new CLWater();
	}	

	// Create Atmosphere. 
	DaySkyPath = PathToDaySky; 
	NightSkyPath = PathToNightSky;
	if(DaySkyPath != "")
	{
	InitAtmosphere();
	}

	pTVCamera->SetViewFrustum(45, 1000, 0.1f);
    pTVCamera->SetPosition(0, 300, -700);
    pTVCamera->SetLookAt(0, 300, 0);
}
CLScene::~CLScene(void)
{
	delete(clLand); clLand = NULL;
	delete(clAtmos); clAtmos = NULL; 
	delete(clWater); clWater = NULL; 
}

void CLScene::Render(long lTimeOfDay)
{
	//Render Atmosphere
	if(clAtmos != NULL)
		clAtmos->UpdateAndRender(lTimeOfDay); // A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
	
	//Render Objets
	if(clLand != NULL)
	{
		clLand->Render();
		clWater->Render();
	}
}


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

void CLScene::LoadSkyBoxesTextures(void)
{
	string Path;

			Path = DaySkyPath + "up.jpg";
			stSkyboxDay.SkyTop = pTVTexturefactory->LoadTexture(Path.c_str(), "SkyTop");
			Path = DaySkyPath+ "down.jpg";
            stSkyboxDay.SkyBottom = pTVTexturefactory->LoadTexture(Path.c_str(), "SkyBottom");
            Path = DaySkyPath+ "left.jpg";
			stSkyboxDay.SkyLeft = pTVTexturefactory->LoadTexture(Path.c_str(), "SkyLeft");
            Path = DaySkyPath+ "right.jpg";
			stSkyboxDay.SkyRight = pTVTexturefactory->LoadTexture(Path.c_str(), "SkyRight");
            Path = DaySkyPath+ "front.jpg";
			stSkyboxDay.SkyFront = pTVTexturefactory->LoadTexture(Path.c_str(), "SkyFront");
            Path = DaySkyPath+"back.jpg";
			stSkyboxDay.SkyBack = pTVTexturefactory->LoadTexture(Path.c_str(), "SkyBack");
			
			Path = NightSkyPath+ "up.jpg";
			stSkyboxNight.SkyTop = pTVTexturefactory->LoadTexture(Path.c_str(), "NightSkyTop");
            Path = NightSkyPath+ "down.jpg";
			stSkyboxNight.SkyBottom = pTVTexturefactory->LoadTexture(Path.c_str(), "NightSkyBottom");
            Path = NightSkyPath+ "left.jpg";
			stSkyboxNight.SkyLeft = pTVTexturefactory->LoadTexture(Path.c_str(), "NightSkyLeft");
            Path = NightSkyPath+ "right.jpg";
			stSkyboxNight.SkyRight = pTVTexturefactory->LoadTexture(Path.c_str(), "NightSkyRight");
            Path = NightSkyPath+ "front.jpg";
			stSkyboxNight.SkyFront = pTVTexturefactory->LoadTexture(Path.c_str(), "NightSkyFront");
            Path = NightSkyPath+ "back.jpg";
			stSkyboxNight.SkyBack = pTVTexturefactory->LoadTexture(Path.c_str(), "NightSkyBack");

}
void CLScene::InitAtmosphere(void)
{
	LoadSkyBoxesTextures();
	clAtmos = new CLAtmosphere(stSkyboxDay, stSkyboxNight); 		
}