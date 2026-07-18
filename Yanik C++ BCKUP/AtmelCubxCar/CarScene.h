#pragma once
#include "Vehicule.h"
#include "CarsManager.h"
#include "Land.h"


#define NUMOPPONENTCARS 3

class CLCarScene
{
public:
	CLCarScene(void);
	~CLCarScene(void);

	void Render(void);
	void InitLandscape(void);
	void InitPhysicsMaterials(void);
	void InitTextures(void);

	void SetCarPosition(void);  // A implementer selon les infos venant du fichier réseau
	void UpdateOpponentsCar(void);

	CLVehicule* clPlayerCar;
	CLVehicule* clOpponentCars[NUMOPPONENTCARS];
	STEngineControl stOpponentsEng[NUMOPPONENTCARS];
	CLCarsManager* clCarsManager;
	CLLand* clLand;
	
	
		
			//Atmos
			int texDayUp,  texDayDown, texDayLeft, texDayRight, texDayFront, texDayBack;

			int pmatTerrain;
			
			CTVScene* pTVScene;   
			CTVCamera* pTVCamera;
			CTVInputEngine* pTVInput;			
			CTVMathLibrary* pTVMaths;                 
			CTVTextureFactory* pTVTexturefactory;     
			CTVMaterialFactory* pTVMaterialfactory;                      
	        CTVPhysics* pTVPhysics;

};
