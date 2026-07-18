#pragma once

#ifndef __3DScenes_h__
#define __3DScenes_h__

#include "stdafx.h"
#include "Scene.h"
#include "TV3DMoteur.h"

#include "Animation.h"
#include "3DObject.h"

#include "Anneau.h"       // CAR SCENE //
#include "Vehicule.h"     // CAR SCENE //
#include "CarsManager.h"  // CAR SCENE //

//   CHAQUE SCENE devrait avoir un CollisionAction, etc..   Devrait ptete etre mis en virtuel dans la classe de base.

// Creer une fonction UpdateWater() qui va faire les render sur les surfaces (cette fontion sera caller dans la boucle render de l'Area ) 
// Ajouter le render de water dans la boucle render de la scene.




// DEFAULT SCENE //

class CLDefaultScene : public CLScene
{
public:
	CLDefaultScene(string PathToLand, string PathToLandTex) : 
	  CLScene(PathToLand, PathToLandTex)
		{};
	virtual ~CLDefaultScene(void) 
		{};
	void Render(long lTimeOfDay)
		{}; 
	void UpdateScene(void)  
		{};
	void MeshClickedAction(int ImeshClickedIndex)
		{};
};

///////////////////

// CAR SCENE ////////
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
	CLCarScene(string PathToLand, string PathToLandTex) : 
	  CLScene(PathToLand, PathToLandTex)
	  {};
	virtual ~CLCarScene(void)
		{
		 clCarsManager->kill();  
		};
	void Render(long lTimeOfDay)
		{
		//Render Atmosphere
		// A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
//		clAtmos->UpdateAndRender(lTimeOfDay);
//		clWater->Render();

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
		cTV_3DVECTOR CameraAimVec;  // sert a rien je crois
		//Render Objets
		clLand->Render();
		clAnneaux->Render();
		pTVCamera->ChaseCamera(clPlayerCar->GetChassis(),new cTV_3DVECTOR(0,5,-15),new cTV_3DVECTOR(0,0,0),1000,true,-1);

		};

	void UpdateScene(void)
		{
		clPlayerCar->ManageInput(); 
		};
	void MeshClickedAction(int ImeshClickedIndex)
		{};
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

// SkyboxSelector SCENE ////////


#define QTY_SKYBOX 5
#define CAM_ROTATION_SPEED 0.1

class CLSkyboxSelectScene : public CLScene
{
protected:
	CLSkybox* clSkyboxes[QTY_SKYBOX];
	CLSkybox* SelectedSkybox;
	CLAnimation* clRotateAnimation;
	CTVMesh* SelectedSkyIcon;

	float fRotationAngle;
	float fActualAngle;
	bool isCamZooming;

public:
	CLSkyboxSelectScene(string PathToLand = "", string PathToLandTex = "") : 
	  CLScene(PathToLand, PathToLandTex)
		{};
	 ~CLSkyboxSelectScene(void)
		{};
	

	CLSkybox* pActiveSkybox;
	void InitScene(void);
	void Render(long lTimeOfDay);
	void UpdateScene(void);
	void MeshClickedAction(int iMeshClickedIndex);
	void StartZoomCamEffect(CTVMesh* selectedSkyIcon);
};
///////////////////


////////////////////////////////////
// Car Select Scene
///////////////////////////////////

#define QTY_CARS 1

class CLCarSelectScene : public CLScene
{
protected:
	CTVMesh* SelectedCarIcon;
	CLLand* clLand;
	CLVehicule* clCars[QTY_CARS];
	CLVehicule* clSelectedCar;

public:
	CLCarSelectScene(string PathToLand = "", string PathToLandTex = "") : 
	  CLScene(PathToLand, PathToLandTex)
		{};
	 ~CLCarSelectScene(void)
		{};
	
	void InitScene(void);
	void Render(long lTimeOfDay);
	void UpdateScene(void);
	void MeshClickedAction(int iMeshClickedIndex);
	void ManageCamCtrl(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY, float fTimeElapsed);
};



#endif
