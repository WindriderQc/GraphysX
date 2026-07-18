#include "CubX.h"
#include <stdio.h>

// suppression de warning a la compilation 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4305) // truncation double -> float 
#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier


CLCubX::CLCubX(float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable) 
{
	iActualState = SLEEPING;
	iSelectedCube = -1;
	clRotationAnim = new CLAnimation(-1,ROTATE,true);
	//  3 axe de rotation(x,y,z) en degrés(valeur relative car elle sera modifié par le speedfactor pour rendre le movement fluide).
	RotForce.x = 20; RotForce.y = 20; RotForce.z = 20;
	SpeedyRotForce.x = 80; SpeedyRotForce.y = 80; SpeedyRotForce.z = 80;

	pTVScene = new CTVScene();
	pTVPhysics = new CTVPhysics;
	pMatFactory = new CTVMaterialFactory();
	pTVCamera = new CTVCamera();
	clCubXMenuMgr = new CLMenuManager(); 

	int i;
	float fSize = 8;
	char String[28];
	char BufferTVAName[256];
	
	
	// Create the Standard lighting material
	iStdMaterial = pMatFactory->CreateMaterial("StdMat");
	pMatFactory->SetAmbient(iStdMaterial, 0.1, 0.1, 0.1, 1);
	pMatFactory->SetDiffuse(iStdMaterial, 0.8, 0.8, 0.8, 1);
	pMatFactory->SetSpecular(iStdMaterial, 0.2, 0.2, 0.2, 1);
	pMatFactory->SetPower(iStdMaterial, 20);
	pMatFactory->SetEmissive(iStdMaterial, 0, 0, 0, 1);

	//  Create and set Mesh
	CubXMesh = pTVScene->CreateMeshBuilder("Mesh");
	bool bResult = CubXMesh->LoadTVM("Media\\CubX\\CubXMesh.tvm",true,true);
	if(bResult == false)
		MessageBoxA(NULL, "The Cubx TVM model hasn't been found...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);

	CubXMesh->SetPosition(Pos.x, Pos.y, Pos.z);//-500,500,0); 
	//CubXMesh->SetScale(Size,Size,Size); // 12
	CubXMesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	CubXMesh->SetMaterial(iStdMaterial, -1);
	CubXMesh->SetShadowCast(true,true);
	InitialRotVec = CubXMesh->GetRotation();


	CubXOpenActor = pTVScene->CreateActor("CubXOpenActor");
	CubXOpenActor->LoadTVA("Media\\CubX\\CubXOpen.tva", true, false);
	CubXOpenActor->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	CubXOpenActor->SetAnimationLoop(false);
	CubXOpenActor->SetMaterial(iStdMaterial);
	CubXOpenActor->SetPosition(Pos.x, Pos.y, Pos.z);
	CubXOpenActor->Enable(false);

	
	for(i=1; i<8;i++)
	{
	sprintf(BufferTVAName, "Media\\CubX\\CubXGet%d.tva", i+1);
	CubXRotActor[i-1] = pTVScene->CreateActor("CubXRotationActor");
	CubXRotActor[i-1]->LoadTVA(BufferTVAName, true, false);
	//CubXRotActor[i-1]->SetScale(fSize,fSize,fSize);
	CubXRotActor[i-1]->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	CubXRotActor[i-1]->SetAnimationLoop(false);
	CubXRotActor[i-1]->SetMaterial(iStdMaterial);
	CubXRotActor[i-1]->SetPosition(Pos.x, Pos.y, Pos.z);
	CubXRotActor[i-1]->Enable(false);
	}

	for(i=0;i<8;i++)
	{
		sprintf(String, "Media\\CubX\\CubXBtn%d.tvm", i+1);
		pCubXBtn[i] = pTVScene->CreateMeshBuilder("CubXBtn");
		pCubXBtn[i]->LoadTVM(String,true,true);// = pTVScene->CreateMeshBuilder("CubXBtn");
		pCubXBtn[i]->SetPosition(Pos.x, Pos.y, Pos.z);
		pCubXBtn[i]->Enable(false);
		//clCubXMenuMgr-
	}
	
	//  Sets physical caracteristics
	fMasse = 0;  // Avec une masse a 0, l'objet n'est pas affecté par la gravité.
	iPhysicBody = pTVPhysics->CreateBody(fMasse);
    pTVPhysics->AddMesh(iPhysicBody, CubXMesh, cTV_BODY_CONVEXHULL, true); 
    pTVPhysics->SetBodyMovable(iPhysicBody, true);
	pTVPhysics->SetDamping(iPhysicBody,0.00001, new cTV_3DVECTOR(0.00001,0.00001,0.00001));


}

CLCubX::~CLCubX(void)
{
	delete(pTVScene);      pTVScene = NULL;
	delete(pTVPhysics);    pTVPhysics = NULL;
	delete(pMatFactory);   pMatFactory = NULL;
	delete(pTVCamera);     pTVCamera = NULL;
	delete(clCubXMenuMgr); clCubXMenuMgr = NULL; 

}

void CLCubX::FadeOff(void)
{
CubXMesh->Enable(false);
}
void CLCubX::FadeOn(void)
{
CubXMesh->Enable(true);
}


void CLCubX::AnimateCubX(float ElapsedTime)
{
//	int i;
	float test;
cTV_3DVECTOR CamPos;


switch(iActualState)
{
case SLEEPING:
			clRotationAnim->Animate(CubXMesh,NULL,RotForce,CubXMesh->GetPosition(),CubXMesh->GetRotation(),ElapsedTime/1000);
			break;
case POSITIONNING:
			if(CubXMesh->GetRotation() != InitialRotVec)
				{
				clRotationAnim->Animate(CubXMesh,NULL,SpeedyRotForce,CubXMesh->GetPosition(),CubXMesh->GetRotation(),ElapsedTime/1000);
				}
			else 
				iActualState = FOCUSING;
			break;
case FOCUSING:
			CamSpeed.x = -10; CamSpeed.y = 5; CamSpeed.z = 10;
			CamPos = pTVCamera->GetPosition();
			if(CamPos.x <= -75) 
				CamSpeed.x = 0; 
			if(CamPos.y >= 725) 
				CamSpeed.y = 0; 
			if(CamPos.z >= -350) 
				CamSpeed.z = 0; 
			pTVCamera->SetPosition(CamPos.x+CamSpeed.x, CamPos.y+CamSpeed.y, CamPos.z+CamSpeed.z);
			pTVCamera->SetLookAt(CubXMesh->GetPosition().x,CubXMesh->GetPosition().y,CubXMesh->GetPosition().z);
			
			if(CamSpeed.x == 0 && CamSpeed.y == 0  &&  CamSpeed.z == 0)
			{
				iActualState = FOCUSED;
				EnableCubXButtons();
			}
			break;
case SELECTING:
			DisableCubXButtons();
			CubXMesh->Enable(false);
			CubXRotActor[iSelectedCube-1]->Enable(true);
			CubXRotActor[iSelectedCube-1]->PlayAnimation(1);
			
			test = CubXRotActor[iSelectedCube-1]->GetKeyFrame();
			//if(CubXRotActor[iSelectedCube-1]->IsAnimationFinished())
			if(CubXRotActor[iSelectedCube-1]->GetKeyFrame()    >  99)    //   PKOI le Is Finished Marche pas???????
			{
			iActualState = OPENING;
			}

			break;
case OPENING:
			//Mesh->Enable(false);
			CubXRotActor[iSelectedCube-1]->Enable(false);
			CubXOpenActor->Enable(true);
			CubXOpenActor->PlayAnimation(1);
			if(CubXOpenActor->IsAnimationFinished())
			{
			iActualState = OPENED;
			}
			break;
case CLOSING:
			
			break;
case DESELECTING:
			
			break;
case FOCUSED:
			
			break;
case OPENED:
			
			break;
case DEFOCUSING:
			
			break;
}



}


void CLCubX::SetCubXMenuButtons(void)
{
	// Niveau 0 = Cube fermé
	// Niveau 1 à 8 = Cube 1 à 8 ouvert
/*
int i,j;
for(j=0;j<8;j++) // 8 niveau
{
for(i=0;i<4;i++) // 4 btn par niveau
  {
	clCubXMenuMgr->CreateBouton(clInsideCubXBtn[(j*4)+i]->GetMesh(),j+1);
  }
}

clCubXMenuMgr->CreateBouton(clInsideCubXExitBtn->GetMesh(),ALLMENU_EXCEPT_MAIN); // Main = Niveau 0
*/
}

void CLCubX::EnableCubXButtons(void)
{
int i;

for(i=0; i<8; i++)
	{
		pCubXBtn[i]->Enable(true);
	}
}

void CLCubX::DisableCubXButtons(void)
{
int i;

for(i=0; i<8; i++)
	{
		pCubXBtn[i]->Enable(true);
	}
}

void CLCubX::Render(void)
{
int i;

if(CubXMesh->IsEnabled())
		CubXMesh->Render();

if(CubXOpenActor->IsEnabled())
		CubXOpenActor->Render();

for(i=0; i<7; i++)
  {
   if(CubXRotActor[i]->IsEnabled())
	CubXRotActor[i]->Render();
  }

/*  
// for trouble shooting....
for(i=0;i<8;i++)
	{
		pCubXBtn[i]->Render();
	}
*/
	
}



CTVMesh* CLCubX::GetMesh(void)
{
return(CubXMesh);
}