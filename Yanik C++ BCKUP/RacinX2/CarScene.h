#pragma once
#include "Vehicule.h"
#include "CarsManager.h"
#include "Land.h"
#include "Atmosphere.h"
#include "Anneau.h"


#define NUMOPPONENTCARS 3

class CLCarScene
{
public:
	CLCarScene(void);
	~CLCarScene(void);

	void Render(float TimeElapsed, long lTimeOfDay);
	void InitPhysicsMaterials(void);
	void InitTextures(void);
	void InitSkyBoxes(char* DaySkyPath, char* NightSkyPath);
	void InitAtmosphere(void);

	void SetCarPosition(void);  // A implementer selon les infos venant du fichier réseau
	void UpdateOpponentsCar(void);

	CLVehicule* clPlayerCar;
	CLVehicule* clOpponentCars[NUMOPPONENTCARS];
	STEngineControl stOpponentsEng[NUMOPPONENTCARS];
	CLCarsManager* clCarsManager;
	CLLand* clLand;
	CLAtmosphere* clAtmos;
	CLWater* clWater;
	CLAnneau* clAnneaux;
	cTV_3DVECTOR VecPosAnneaux[100];
	
	
		
			//Atmos
			int texDayUp,  texDayDown, texDayLeft, texDayRight, texDayFront, texDayBack;
			STSkybox stSkyboxDay, stSkyboxNight;
			int pmatTerrain;
			
			CTVScene* pTVScene;   
			CTVCamera* pTVCamera;
			CTVInputEngine* pTVInput;			
			CTVMathLibrary* pTVMaths;                 
			CTVTextureFactory* pTVTexturefactory;     
			CTVMaterialFactory* pTVMaterialfactory;                      
	        CTVPhysics* pTVPhysics;

};
