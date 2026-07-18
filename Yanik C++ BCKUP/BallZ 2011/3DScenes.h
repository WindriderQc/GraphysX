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

// structure used to hold game play especial effects
/*
struct SpecialEffectStruct
{
	//void *soundHandle;
	//void *particleHandle;

	// create the resources for this materials effect here
	void Init (char* effectName)
	{
	}

	// destroy all resources for this material interaction here
	void Detroy ()
	{
	}

	// play the impact sound
	void PlayImpactSound (float volume)
	{
	}

	// play scratch sound
	void PlayScratchSound (float volume)
	{
	}

	NewtonBody* m_body0;
	NewtonBody* m_body1;
	dVector m_position;
	float m_contactMaxNormalSpeed;
	float m_contactMaxTangentSpeed;
};
*/


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


// FLIGHTX Scene







///////////////////

//  BALLZ SCENE  ///////
#include "Anneau.h"
#include "3DObject.h"
#include "CLBallZ.h"
#include "FlightXScene.h"
#include "Elevator.h"


#define PI 3.141592653
#define ROLL_FACTOR 2
#define PITCH_UP_FACTOR 2
#define PITCH_DOWN_FACTOR 2


class CLBallZScene : public CLScene
{
protected:
	CLAnneau* clAnneaux; cTV_3DVECTOR VecPosAnneaux[100];
	CLBallZ* clBallZ;
	CLElevator* clElevator;
	//CL3DPrimitive* clFloor;
	CL3DPrimitive* clRefBox;
	CL3DObject* clTrack;
	CL3DObject* clFinishStation;
	CTVMesh* pCameraAim;
	cTV_3DVECTOR AimVec;
	CTVMathLibrary* pTVMaths;
	//CLFlightXScene* clFlightX;
	
	
public:
	CLBallZScene(string PathToLand = "", string PathToLandTex = "", string PathToDaySky = "Media\\Sky\\Clearblue\\", string PathToNightSky = "Media\\Sky\\Clearblue\\") : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
	  {};
	virtual ~CLBallZScene(void)
		{};

	void Render(long lTimeOfDay)
		{
		//cTV_3DMATRIX Matrix;
		clAtmos->UpdateAndRender(lTimeOfDay);

		clBallZ->Render();
		
		clTrack->Render();
		clFinishStation->Render();
		//clFloor->Render();
		clRefBox->Render();
		//Render Objets
		clAnneaux->Render();
		//clFlightX->Render();

		//DrawMeshAxis(clBallZ->clShell->mShell);
		DrawMeshAxis(clBallZ->clCtrl->Mesh, 100);
		DrawTV3DAxis();
		clElevator->Render();
		UpdateScene();
		};

	void UpdateScene(void)
		{
		 clBallZ->ManageInput();  
		
		};
	void ManageCamCtrl(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY,float ElapsedTime)
	{
		AimVec = clBallZ->clShell->Mesh->GetPosition();
		pCameraAim->SetPosition(AimVec.x, AimVec.y,AimVec.z);
		pTVCamera->ChaseCamera(pCameraAim,new cTV_3DVECTOR(0,25,-50),new cTV_3DVECTOR(0,0,0),100,true,-1);
		//clFlightX->ManagePlaneInput(ElapsedTime);
	};
	void MeshClickedAction(int ImeshClickedIndex)
		{};
	void InitScene(void)
		{
			int i, iGroundTex, iTestTex;
			int pmatGround;
			int pmatBallZ,pmatDefault, pmatTest;
			CTVPhysics* pTVPhysics;   //  pas propre ici // a mettre dans la classe de base Scene
		

			//clFlightX = new CLFlightXScene();

			// Create BallZ
			clBallZ = new CLBallZ();
			clElevator = new CLElevator();


			iGroundTex = pTVTexturefactory->LoadTexture("Media\\marble04.jpg", "Test");
			iTestTex = pTVTexturefactory->LoadTexture("Media\\yellowtwowaysign.jpg","Ground");
			
			//clFloor = new CL3DPrimitive(2, iGroundTex, cTV_3DVECTOR(0,-1,0), cTV_3DVECTOR(150,1,150),true, 1250, true);
			clTrack = new CL3DObject("Media\\Level0.tvm",NULL,cTV_3DVECTOR(0,0,0), cTV_3DVECTOR(0.5,0.5,0.5),true, 0, true);
			clFinishStation = new CL3DObject("Media\\FinishStation.tvm",NULL,cTV_3DVECTOR(0,0,0), cTV_3DVECTOR(0.5,0.5,0.5),true, 0, true);
			//clBowl = new CL3DObject("Media\\InvertSphere.tvm",iGroundTex,cTV_3DVECTOR(0,10,0), cTV_3DVECTOR(0.2,0.2,0.2),true, 0, true);
			
			clRefBox = new CL3DPrimitive(2, iTestTex, cTV_3DVECTOR(10,10,10), cTV_3DVECTOR(1,1,1),true, 1250, false);

			// Physics Materials
			// get the default material ID
			pmatDefault = pTVPhysics->GetDefaultMaterialGroup();

			// Set default material properties
			pTVPhysics->SetMaterialInteractionSoftness(pmatDefault, pmatDefault, 0.05f);
			//pTVPhysics->NewtonMaterialSetDefaultElasticity (pmatDefault, pmatDefault, 0.4f);
			//pTVPhysics-NewtonMaterialSetDefaultCollidable (pmatDefault, pmatDefault, 1);
			pTVPhysics->SetMaterialInteractionFriction(pmatDefault, pmatDefault, 0.3f, 0.2f);
			
			//TerrainLandscape
			pmatGround = pTVPhysics->CreateMaterialGroup("Terrain");
			pTVPhysics->SetMaterialInteractionFriction(0, pmatGround, 0.9f, 1.0f);
			pTVPhysics->SetMaterialInteractionSoftness(0, pmatGround, 0.05f);
			pTVPhysics->SetMaterialInteractionBounciness(0, pmatGround, 0.01f);
			pTVPhysics->SetBodyMaterialGroup(clTrack->iPhysicBody, pmatGround);
			//Ballz  
			pmatBallZ = pTVPhysics->CreateMaterialGroup("Balle");
			pTVPhysics->SetMaterialInteractionFriction(pmatBallZ, pmatGround, 0.01f, 0.01f);
			pTVPhysics->SetMaterialInteractionSoftness(pmatBallZ, pmatGround, 1.0f);
			pTVPhysics->SetMaterialInteractionBounciness(pmatBallZ, pmatGround, 0.01f);
			pTVPhysics->SetBodyMaterialGroup(clBallZ->clShell->iPhysicBody, pmatBallZ);
	


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
			pTVCamera->SetPosition(0,100, -150);
			pTVCamera->SetLookAt(0,5,0);

			// Mesh qui peux etre mis a la meme position qu'un autre.  En fesant suivre CameraAim par la camera, ceci 
			// permet de faire rotationner l'objet que l'on regarde sans que la camera ne suive la rotation.
			pCameraAim = pTVScene->CreateMeshBuilder("CameraAim");
			pCameraAim->CreateBox(1,1,1, false);
			
		};

};






// CAR SCENE ////////
//#include "Anneau.h"
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
			clCarsManager->Init(0);  //#define CLIENT 0 dans tcpcomm

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

	float fRotationAngle;
	float fActualAngle;
	bool isCamZooming;

public:
	CLSkyboxSelectScene(string PathToLand = "", string PathToLandTex = "", string PathToDaySky = "", string PathToNightSky = "") : 
	  CLScene(PathToLand, PathToLandTex, PathToDaySky, PathToNightSky)
		{
		pTVMath = new CTVMathLibrary();
		};
	 ~CLSkyboxSelectScene(void)
		{};
	void InitScene(void)
	{
	int i;
		isCamZooming = NULL;
		SelectedSkyIcon = NULL;
		SelectedSkybox = NULL;
		
		string SkyboxesPath = "Media\\Sky\\";
		string SkyboxNames[QTY_SKYBOX] = {"ClearBlue", "SkyX", "ClearNight" , "LostValley" , "Winter" };
		
		float Radius = 25 * QTY_SKYBOX;  // 25 = Facteur de rayon pour le cercle des skybox Icon
		for(i=0; i< QTY_SKYBOX; i++)
			{
			clSkyboxes[i] = new CLSkybox(SkyboxesPath,SkyboxNames[i]);	
			clSkyboxes[i]->mSkyboxCubeIcon->Enable(true);
			
			//  Mettre ca dans un fonction a part....
			float DegAngle = 360/QTY_SKYBOX * i; 
			float RadAngle = pTVMath->Deg2Rad(DegAngle);
			float x = cos(RadAngle) * Radius;
			float z = sin(RadAngle) * Radius;
			
			clSkyboxes[i]->mSkyboxCubeIcon->SetPosition(x,0,z);   //  mettre un mesh au centre comme cible de la cam
			}
		
		clRotateAnimation = new CLAnimation(-1,true);
		
		mCameraAim = pTVScene->CreateMeshBuilder("CameraAim");
		mCameraAim->CreateBox(10,10,10);
		mCameraAim->SetPosition(10,0,10);
		mCameraAim->Enable(false);

		pTVCamera->SetViewFrustum(45, 1000, 0.1f);
		pTVCamera->SetPosition(150, 200, 500);
		pTVCamera->SetLookAt(0,0,0);	
	};
	void Render(long lTimeOfDay)
	{
	int i;
		
		//Render Objets
		mCameraAim->Render();

		for(i= 0; i< QTY_SKYBOX; i++)
			{
			clSkyboxes[i]->Render();
			}
		UpdateScene();
	};
	
	void UpdateScene(void)
	{
		int i;
		cTV_3DVECTOR OldCamPos;
		cTV_3DVECTOR NewCamPos;
		cTV_COLLISIONRESULT ColRes;

		float SpeedFactor = CLTV3DMoteur::getInstance()->GetTimeElapsed()/1000;  // /1000 pour avoir un speedfactor en seconde
		
		for(i= 0; i< QTY_SKYBOX; i++)
		{
		clRotateAnimation->Rotate(clSkyboxes[i]->mSkyboxCubeIcon, cTV_3DVECTOR(0,50,0), SpeedFactor); 			
		}

		if(SelectedSkyIcon)
			{
				if(isCamZooming)
				{
					OldCamPos = pTVCamera->GetPosition();
					pTVCamera->ChaseCamera(SelectedSkyIcon,&cTV_3DVECTOR(0,0,0),&cTV_3DVECTOR(0,0,0),2);
					NewCamPos = pTVCamera->GetPosition();
					
					// Detect collision between camera and Icon
					if(pTVScene->AdvancedCollision(&OldCamPos, &NewCamPos, &ColRes, cTV_OBJECT_MESH, cTV_TESTTYPE_BOUNDINGBOX))
						{
						SelectedSkybox->Enable(1.0);
						pTVCamera->SetPosition(1000, 5, 00);
						pTVCamera->SetLookAt(0, 3, 0);	
						SelectedSkyIcon = NULL;   //  Stops the zoomin effect
						SelectedSkybox = NULL; // Free the Skybox selected flag to allow a future selection
						isCamZooming = false;
						clRotateAnimation->Start();
						}
				
				}

			}else if(clSkyboxes[0]->IsAtmosSkySet())
					{	
					//  PKOI CHANGER LA VALEUR DE LANGLE N'FFECTE PAS LA VITEsse de rotation????  
						pTVCamera->RotateAroundMesh(mCameraAim,true,25,500);  //clSkyboxes[0]->GetCubeIconMesh()
					}
	};

	void MeshClickedAction(int iMeshClickedIndex)
	{
		int i;
		for(i=0; i< QTY_SKYBOX; i++)
		{
		if(iMeshClickedIndex == clSkyboxes[i]->mSkyboxCubeIcon->iMeshIndex)
			{	
			SelectedSkybox = clSkyboxes[i];
			StartZoomCamEffect(clSkyboxes[i]->mSkyboxCubeIcon);
			}
		}
	};
	void StartZoomCamEffect(CTVMesh* selectedSkyIcon)
	{
	
		clRotateAnimation->Stop();
		SelectedSkyIcon = selectedSkyIcon;
		isCamZooming = true;
	};
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





#endif
