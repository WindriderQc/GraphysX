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
		cTV_3DVECTOR Pos;
		isCamZooming = NULL;
		SelectedSkyIcon = NULL;
		SelectedSkybox = NULL;
		fRotationAngle = CAM_ROTATION_SPEED;
		fActualAngle = 0;

		string SkyboxesPath = "Media\\Sky\\";
		string SkyboxNames[QTY_SKYBOX] = {"ClearBlue", "SkyX", "ClearNight" , "LostValley" , "Winter" };
		string NightSkyboxName = "ClearNight";

		for(i=0; i< QTY_SKYBOX; i++)
			{
			clSkyboxes[i] = new CLSkybox(SkyboxesPath,SkyboxNames[i], NightSkyboxName);	
			clSkyboxes[i]->mSkyboxCubeIcon->Enable(true);
			Pos = GenerateCerclePos(i, QTY_SKYBOX, QTY_SKYBOX * 25); 
			clSkyboxes[i]->mSkyboxCubeIcon->SetPosition(Pos.x,0,Pos.z);
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
						SelectedSkybox->EnableDay(1.0);
						pActiveSkybox = SelectedSkybox;
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

///////////////////////////////////


//////////////////////////////////////
//  CarSelect Scene
/////////////////////////////////////
void CLCarSelectScene::InitScene(void)
{
int i;
cTV_3DVECTOR Pos;
clLand = new CLLand("Media\\Heightmaps\\height.jpg", "Media\\ground.jpg", 100,100,cTV_3DVECTOR(-50,0,-50));  // Modifier pour permettre la sélection dela précision du terrain.
clWater = new CLWater(2, "Media\\distortiontexture.dds");	

string sCarNames[QTY_CARS] = {"Impreza"};


for(i=0;i<QTY_CARS;i++)
{
	Pos = GenerateCerclePos(i, QTY_CARS, QTY_CARS * 30); 
	clCars[i] = new CLVehicule(i,pmatTerrain);                 //  A arranger car toujours le meme models de car
	Pos.y = 100;
	clCars[i]->SetPosition(Pos);
}

pTVCamera->SetViewFrustum(45, 1000, 0.1f);
pTVCamera->SetPosition(75, 100, 50);
pTVCamera->SetLookAt(0,0,0);	
}
void CLCarSelectScene::Render(long lTimeOfDay)
{
int i;

if(pTVAtmos->SkyBox_IsEnabled())
			pTVAtmos->SkyBox_Render();

clLand->Render();
clWater->Render();

for(i=0;i<QTY_CARS;i++)
{
	clCars[i]->Render();
}


}
void CLCarSelectScene::UpdateScene(void)
{
	//cTV_3DVECTOR carpos = clCars[0]->GetChassis()->GetPosition();
	//pTVCamera->SetLookAt(carpos.x, carpos.y, carpos.z);
	//pTVCamera->ChaseCamera(clCars[0]->GetChassis(),&cTV_3DVECTOR(0,0,0),&cTV_3DVECTOR(0,0,0),2);
}
void CLCarSelectScene::MeshClickedAction(int iMeshClickedIndex)
{}

void CLCarSelectScene::ManageCamCtrl(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY, float fTimeElapsed)
{
	try{

cTV_3DVECTOR CamPos, CamLookAt;
	
	CamPos = pTVScene->GetCamera()->GetPosition();
	CamLookAt = pTVScene->GetCamera()->GetLookAt();
	
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
	CamPos.x = CamPos.x + (cosf(sngAngleY) * sngWalk * fTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * fTimeElapsed);
	CamPos.y = CamPos.y + (sngRaise * fTimeElapsed);  // A vérifier si l'équation est OK
	CamPos.z = CamPos.z + (sinf(sngAngleY) * sngWalk * fTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * fTimeElapsed);
	
	pTVScene->GetCamera()->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);

	}catch(return(0););
}