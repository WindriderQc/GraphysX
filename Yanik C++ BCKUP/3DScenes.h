#pragma once

#ifndef __3DScenes_h__
#define __3DScenes_h__

#include "stdafx.h"
#include "Scene.h"
#include "TV3DMoteur.h"
#include <math.h>


//   CHAQUE SCENE devrait avoir un CollisionAction, etc..   Devrait ptete etre mis en virtuel dans la classe de base.

// Creer une fonction UpdateWater() qui va faire les render sur les surfaces (cette fontion sera caller dans la boucle render de l'Area ) 
// Ajouter le render de water dans la boucle render de la scene.




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
	void MeshClickedAction(int ImeshClickedIndex)
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
#include "CLSkybox.h"
#include "Animation.h"

#define QTY_SKYBOX 5
#define CAM_ROTATION_SPEED 0.5

class CLSkyboxSelectScene : public CLScene
{
protected:
	CLSkybox* clSkyboxes[QTY_SKYBOX];
	CLSkybox* SelectedSkybox;
	CLAnimation* clRotateAnimation;
	CTVMesh* SelectedSkyIcon;
	CTVMesh* mCameraAim;
	CTVMathLibrary* pTVMath;
	CTVAtmosphere* pTVAtmos;   // A mettre dans la classe Scene de base probablement...

	float fRotationAngle;
	float fActualAngle;
	bool isCamZooming;

	void UpdateScene(void);

public:
	CLSkyboxSelectScene(string PathToLand = "", string PathToLandTex = "", string PathToDaySky = "", string PathToNightSky = "") : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
		{};
	 ~CLSkyboxSelectScene(void)
		{};
	void InitScene(void);
	void Render(long lTimeOfDay);
	
	void MeshClickedAction(int iMeshClickedIndex);
	void StartZoomCamEffect(CTVMesh* selectedSkyIcon);
};
///////////////////

/*
void ManageCamCtrl()    //  Pourrait être mis dans une classe caméra...  
{
	cTV_3DVECTOR CamPos, CamLookAt;
	CamPos = clTV3D->pCamera->GetPosition();
	CamLookAt = clTV3D->pCamera->GetLookAt();
	
	float sngWalk = fWalk;      
	float sngStrafe = fStrafe;  
	float sngRaise = fRaise;    
	float sngAngleY = 0;
	float sngAngleX = 0;


	//  Keyboard Ctrl Section

	//Okay, now for the smothing of the movement... We checked above if we were pressing a key. If so, then we updated the
    //movement variable to 1 (positive or negative). Here, we lower this value until it get to 0. This method give us a
    //smoother camera movement. We start by updating the forward and backward (walk) movement
		if(sngWalk > 0)
		{
        sngWalk = sngWalk - 0.05f;
        if(sngWalk < 0) sngWalk = 0;
		}
		if(sngWalk < 0)
		{
        sngWalk = sngWalk + 0.05f;
        if(sngWalk > 0) sngWalk = 0;
		}
	 //Now, we update the left and right (strafe) movement.
		if(sngStrafe > 0)
		{
        sngStrafe = sngStrafe - 0.05f;
        if(sngStrafe < 0) sngStrafe = 0;
		}
		if(sngStrafe < 0)
		{
        sngStrafe = sngStrafe + 0.05f;
        if(sngStrafe > 0) sngStrafe = 0;
		}
	 //Now, we update the up and down (Raise) movement.
		if(sngRaise > 0)
		{
        sngRaise = sngRaise - 0.05f;
        if(sngRaise < 0) sngRaise = 0;
		}
		if(sngRaise < 0)
		{
        sngRaise = sngRaise + 0.05f;
        if(sngRaise > 0) sngRaise = 0;
		}


	// Mouse Ctrl Section
	long tmpMouseX = fMouseDeltaX;
	long tmpMouseY = fMouseDeltaY;
	//From the mouse return values, we update the camera angles
	//by adding or substracting the mouse return value.
	sngAngleX = sngAngleX - ((float)tmpMouseY / 100.0f);
	sngAngleY = sngAngleY - ((float)tmpMouseX / 100.0f);
	//We will add a simple check, so we can't look up at more
	//than 80 degrees nor down than -80 degrees.
	if(sngAngleX > 1.3f) sngAngleX = 1.3f;
	if(sngAngleX < -1.3f) sngAngleX = -1.3f;
		    
	CamLookAt.x = CamPos.x + cosf(sngAngleY);
	CamLookAt.y = CamPos.y + tanf(sngAngleX);
	CamLookAt.z = CamPos.z + sinf(sngAngleY);


	//   A CHECKER CAR SNGAngle est jamais modifier !!?!?!?
	//Update the vectors using the angles and positions.
	float fTimeElapsed = clTV3D->GetTimeElapsed();
	CamPos.x = CamPos.x + (cosf(sngAngleY) * sngWalk * fTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * fTimeElapsed);
	CamPos.y = CamPos.y + (sngRaise * fTimeElapsed);  // A vérifier si l'équation est OK
	CamPos.z = CamPos.z + (sinf(sngAngleY) * sngWalk * fTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * fTimeElapsed);
	
	clTV3D->SetCameraPos(CamPos, CamLookAt);
}

*/


////////////////////////////////////
// Test Scene
///////////////////////////////////

class CLSelectCarScene : public CLScene
{
protected:
	CTVMesh* mCameraAim; // pourrait ptete etre mis dans la classe CLScene de base....
	CTVMesh* SelectedCarIcon;

	void UpdateScene(void);

public:
	CLSelectCarScene(string PathToLand = "", string PathToLandTex = "", string PathToDaySky = "", string PathToNightSky = "") : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
		{};
	 ~CLSelectCarScene(void)
		{};

	void InitScene(void);
	void Render(long lTimeOfDay);
	
	void MeshClickedAction(int iMeshClickedIndex);
};



#endif
