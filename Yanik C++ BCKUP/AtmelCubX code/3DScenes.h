#pragma once

#ifndef __3DScenes_h__
#define __3DScenes_h__

#include "stdafx.h"
#include "Scene.h"
#include "TV3DMoteur.h"


//   CHAQUE SCENE devrait avoir un meshclickedAction , un CollisionAction, etc..   Devrait ptete etre mis en virtuel dans la classe de base.






// DEFAULT SCENE //

class CLDefaultScene : public CLScene
{
public:
	CLDefaultScene(string PathToLand, string PathToLandTex, string PathToDaySky, string PathToNightSky) : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
		{};
	virtual ~CLDefaultScene(void) 
		{};
	void Render(long lTimeOfDay)
		{};
};

///////////////////



// CAR SCENE ////////
#include "Anneau.h"
#include "Vehicule.h"
#include "CarsManager.h"


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
	void Render(long lTimeOfDay)
		{
		//Render Atmosphere
		// A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
		clAtmos->UpdateAndRender(lTimeOfDay);
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
#include "CLSkybox.h"
#include "Animation.h"

class CLSkyboxSelectScene : public CLScene
{
protected:
	CLSkybox* clSkybox1;
	CLAnimation* clRotateAnimation;
	CTVMesh* SelectedSkyIcon;


public:
	CLSkyboxSelectScene(string PathToLand = "", string PathToLandTex = "", string PathToDaySky = "", string PathToNightSky = "") : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
	  {};
	virtual ~CLSkyboxSelectScene(void)
		{};

	void InitScene(void)
		{
		SelectedSkyIcon = NULL;
		clSkybox1 = new CLSkybox("Media\\Sky\\","ClearBlue");	
		clSkybox1->mSkyboxCubeIcon->Enable(true);
		clSkybox1->mSkyboxCubeIcon->SetPosition(0,0,0);
		
		clRotateAnimation = new CLAnimation(-1,ROTATE,true);
		//				Load 3 cubemesh
		//creer 3 classe anim - rotation des 3 cubes
		//creer 1 classe anim pour le zoom cam vers l'icone
		 
			pTVCamera->SetViewFrustum(45, 1000, 0.1f);
			pTVCamera->SetPosition(1000, 5, 00);
			pTVCamera->SetLookAt(0, 3, 0);

		};

	void Render(long lTimeOfDay)
		{
		cTV_3DVECTOR CameraAimVec;
		//Render Objets
		clSkybox1->Render();
		
		UpdateScene();
		};

	void UpdateScene(void)
		{
			clRotateAnimation->Animate(clSkybox1->mSkyboxCubeIcon,20, cTV_3DVECTOR(50,50,50),
											CLTV3DMoteur::getInstance()->GetTimeElapsed()/1000); // /1000 pour avoir un speedfactor en seconde   
			
			if(SelectedSkyIcon)
			{
				pTVCamera->ChaseCamera(SelectedSkyIcon,&cTV_3DVECTOR(0,0,0),&cTV_3DVECTOR(0,0,0),2);	
				
				cTV_3DVECTOR cam = pTVCamera->GetPosition();
				cTV_3DVECTOR mesh = SelectedSkyIcon->GetPosition();
				cTV_3DVECTOR tmp = cam - mesh;
				if((tmp.x < 10) && (tmp.y < 10) && (tmp.z < 10))
				{
					clSkybox1->Enable(1.0);
					pTVCamera->SetPosition(1000, 5, 00);
					pTVCamera->SetLookAt(0, 3, 0);	
				}
			}
//		Faire tourner cam 360 autour des icons

			/*
	Si cam contact avec Icon
		setter le skybox
*/

		};



	void MeshClickedAction(int iMeshClickedIndex)
	{
		if(iMeshClickedIndex == clSkybox1->mSkyboxCubeIcon->iMeshIndex)
				StartZoomCamEffect(clSkybox1->mSkyboxCubeIcon);
	
	};

	void StartZoomCamEffect(CTVMesh* selectedSkyIcon)
	{
		clRotateAnimation->Stop();
		SelectedSkyIcon = selectedSkyIcon;
	};
};



///////////////////







#endif
