#include "StdAfx.h"
#include "3DScenes.h"





//////////////////////////////
// CARX Scene 
//////////////////////////////

void CLCarScene::UpdateOpponentsCar(void)
{
clCarsManager->GetPlayersInput();

int i;
	
for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->SetEngineValues(stOpponentsEng[i]);// Des fois ca peut planter car rien d'initialiser pour les PlayersInput si ya rien..
	}

}





///////////////////////


//////////////////////////////
//  SkyboxSelect Scene
//////////////////////////////

void CLSkyboxSelectScene::InitScene(void)
{
		int i;
		
		isCamZooming = NULL;
		SelectedSkyIcon = NULL;
		SelectedSkybox = NULL;
		fRotationAngle = CAM_ROTATION_SPEED;
		fActualAngle = 0;
		pTVMath = new CTVMathLibrary();
		pTVAtmos = new CTVAtmosphere();

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
		
}

void CLSkyboxSelectScene::Render(long lTimeOfDay)
{
		int i;
		
		//Render Objets
		mCameraAim->Render();  //  A enlever, pour test seulement....

		for(i= 0; i< QTY_SKYBOX; i++)
			{
			clSkyboxes[i]->RenderIcon();
			}
		if(pTVAtmos->SkyBox_IsEnabled())
			pTVAtmos->SkyBox_Render();
		
		UpdateScene();
}



void CLSkyboxSelectScene::UpdateScene(void)
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

			}else if(pTVAtmos->SkyBox_IsEnabled())
					{	
				    fActualAngle = fActualAngle + fRotationAngle;
					pTVCamera->RotateAroundMesh(mCameraAim,true,fActualAngle,500); 
					}
}

void CLSkyboxSelectScene::MeshClickedAction(int iMeshClickedIndex)
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
}

void CLSkyboxSelectScene::StartZoomCamEffect(CTVMesh* selectedSkyIcon)
{
		clRotateAnimation->Stop();
		SelectedSkyIcon = selectedSkyIcon;
		isCamZooming = true;
}
///////////////////////////////////