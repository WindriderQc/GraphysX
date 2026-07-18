#pragma once

#ifndef __3DScenes_h__
#define __3DScenes_h__

#include "stdafx.h"
#include "Scene.h"


// CAR SCEBNE ////////
#include "Anneau.h"
#include "Vehicule.h"
#include "CarsManager.h"
#include <string>


#define NUMOPPONENTCARS 3


class CLCarScene : public CLScene
{
protected:
	CLVehicule* clPlayerCar;
	CLVehicule* clOpponentCars[NUMOPPONENTCARS];
	STEngineControl stOpponentsEng[NUMOPPONENTCARS];
	CLCarsManager* clCarsManager;
	CLAnneau* clAnneaux;
	cTV_3DVECTOR VecPosAnneaux[100];

public:
	CLCarScene(string PathToLand, string PathToLandTex, string PathToDaySky, string PathToNightSky) : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
	  {};
	virtual ~CLCarScene(void)
		{
		 clCarsManager->kill();  
		};
	void Render(float TimeElapsed, long lTimeOfDay)
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

		};

	void UpdateScene(void)
		{
		clPlayerCar->ManageInput(); 
		};
	void InitScene(void)
		{
			int i;
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
		 
			pTVCamera->SetViewFrustum(45, 1000, 0.1f);
			pTVCamera->SetPosition(0, 5, -20);
			pTVCamera->SetLookAt(0, 3, 0);

		};
	
//	void SetCarPosition(void);  // A implementer selon les infos venant du fichier réseau
	void UpdateOpponentsCar(void);

};

///////////////////



#endif
