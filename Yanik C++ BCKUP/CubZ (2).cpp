#include "CubZ.h"
#include <stdio.h>

CLCubZ::CLCubZ(char* PathToMesh, float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable) 
	: CL3DObject(PathToMesh,Size,Texture,Pos, bEnable)
{
	Mesh->Enable(false);  // Désactive le tvm  car on mets les tva à la place!
	iSelectedCube = -1;
	fSize = 8;

	bIsRotating =0;
	bIsBackRotating =0;
	bIsOpening =0;
	bIsClosing =0;
	bIsDisappearing =0;

	CubXSaversActor = pTVScene->CreateActor("CubZActor");
	CubXSaversActor->LoadTVA("media\\CubXScreensaver.tva", true, false);
	CubXSaversActor->SetScale(fSize,fSize,fSize);
	CubXSaversActor->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	AnimationCount = CubXSaversActor->GetAnimationCount();
	CubXSaversActor->StopAnimation();
	CubXSaversActor->Enable(false);
	CubXSaversActor->SetMaterial(iStdMaterial);
	CubXSaversActor->SetTexture(Mesh->GetTexture());
	CubXSaversActor->SetPosition(Pos.x, Pos.y, Pos.z);

	CubeOpenActor = pTVScene->CreateActor("CubeOpenActor");
	CubeOpenActor->LoadTVA("media\\CubeOpen.tva", true, false);
	CubeOpenActor->SetScale(fSize,fSize,fSize);
	CubeOpenActor->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	CubeOpenActor->SetAnimationLoop(false);
	CubeOpenActor->SetMaterial(iStdMaterial);
	CubeOpenActor->Enable(false);
	CubeOpenActor->SetTexture(Mesh->GetTexture());
	CubeOpenActor->SetPosition(Pos.x, Pos.y, Pos.z);

	CubXRotationActor = pTVScene->CreateActor("CubXRotationActor");
	CubXRotationActor->LoadTVA("media\\CubeRot.tva", true, false);
	CubXRotationActor->SetScale(fSize,fSize,fSize);
	CubXRotationActor->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	CubXRotationActor->SetAnimationLoop(false);
	CubXRotationActor->SetMaterial(iStdMaterial);
	CubXRotationActor->SetTexture(Mesh->GetTexture());
	CubXRotationActor->SetPosition(Pos.x, Pos.y, Pos.z);

	int i;
	for(i=0;i<8;i++)
	{
		pCubXBtn[i] = pTVScene->CreateMeshBuilder("CubXBtn");
		pCubXBtn[i]->CreateBox(fSize*58,fSize*58,fSize*58,false);
	}
	
	cTV_3DVECTOR CubXPos = CubXRotationActor->GetPosition();
	pCubXBtn[0]->SetPosition(CubXPos.x - 0*fSize,   CubXPos.y + 25*fSize, CubXPos.z + 0*fSize);
	pCubXBtn[1]->SetPosition(CubXPos.x - 0*fSize, CubXPos.y + 25*fSize, CubXPos.z + 100*fSize);
	pCubXBtn[2]->SetPosition(CubXPos.x - 95*fSize, CubXPos.y + 25*fSize, CubXPos.z + 100*fSize);
	pCubXBtn[3]->SetPosition(CubXPos.x - 95*fSize,   CubXPos.y + 25*fSize, CubXPos.z + 0*fSize);
	pCubXBtn[4]->SetPosition(CubXPos.x - 0*fSize,   CubXPos.y - 70*fSize, CubXPos.z + 0*fSize);
	pCubXBtn[5]->SetPosition(CubXPos.x - 0*fSize, CubXPos.y - 70*fSize, CubXPos.z + 100*fSize);
	pCubXBtn[6]->SetPosition(CubXPos.x - 95*fSize, CubXPos.y - 70*fSize, CubXPos.z + 100*fSize);
	pCubXBtn[7]->SetPosition(CubXPos.x - 95*fSize,   CubXPos.y - 70*fSize, CubXPos.z + 0*fSize);

	char BufferTVMName[256];
	for(i=0;i<32;i++)
	{
	 sprintf(BufferTVMName, "Media\\CubXBtn\\Btn%d.tvm", i+1);
	 clInsideCubXBtn[i] = new CL3DObject(BufferTVMName,fSize,NULL,Pos,false);
	 clInsideCubXBtn[i]->GetMesh()->RotateY(-90,true);
	 clInsideCubXBtn[i]->GetMesh()->SetPosition(Pos.x, Pos.y - 200, Pos.z);
	}
	clInsideCubXExitBtn = new CL3DObject("Media\\CubXBtn\\BtnExit.tvm",fSize,NULL,Pos,false);
	clInsideCubXExitBtn->GetMesh()->RotateY(-90,true);
	clInsideCubXExitBtn->GetMesh()->SetPosition(Pos.x, Pos.y + 0, Pos.z);

	clCubXMenuMgr = new CLMenuManager(); 
	SetCubXMenuButtons();

}

CLCubZ::~CLCubZ(void)
{
}
void CLCubZ::Animate(float ElapsedTime)
{
GetMesh()->RotateY(ROTSPEED * ElapsedTime,true);

}

void CLCubZ::StartSaversAnimation(void)
{
	CubXRotationActor->Enable(false);
	CubXSaversActor->Enable(true);
	CubXSaversActor->PlayAnimation(1);
}

void CLCubZ::RenderActor(void)
{
	float Start = 0;
	float End = 0; 
	float fKey; 
	int SourceAnimID = 0;

if(bIsRotating)
{
	if(CubXRotationActor->IsAnimationFinished())
	{
	bIsRotating = 0;
	Open();
	Disappear();
	}

}
else if(bIsBackRotating)
{
	SourceAnimID = CubXRotationActor->GetAnimationIDByName("Animation0");
	CubXRotationActor->GetAnimationRangeInfo(CubXRotationActor->GetAnimationID(),&SourceAnimID, &Start,&End);
	fKey = CubXRotationActor->GetKeyFrame();
	if( fKey <= 3 )
	{
		CubXRotationActor->StopAnimation();		
		bIsBackRotating = 0;
		iSelectedCube = -1;
		CubXRotationActor->ResetTime();
		EnableCubXMainBtn(true);
		clCubXMenuMgr->iAncienNiveau = clCubXMenuMgr->iNiveauActif;
		clCubXMenuMgr->iNiveauActif = 0; 
	}
}
else if(bIsClosing)
{
	fKey = CubeOpenActor->GetKeyFrame();
	if( fKey <= 3 )
	{
		CubeOpenActor->StopAnimation();	
		CubeOpenActor->Enable(false);
		CubXRotationActor->Enable(true);
		bIsClosing = 0;
		BackRotate();
	}
}
else if(bIsOpening)
{
	if(CubeOpenActor->IsAnimationFinished())
	{
	bIsOpening = 0;
	CubeOpenActor->ResetTime();
	clCubXMenuMgr->iAncienNiveau = clCubXMenuMgr->iNiveauActif;
	clCubXMenuMgr->iNiveauActif = iSelectedCube + 1; 
	}
}
/*
if(bIsDisappearing)
{
	//CubXRotationActor->SetAlphaTest(true,ALPHAVALUETOSET,true);  //  A FINIR
	CubXRotationActor->Enable(false);
}
*/

	if(CubXSaversActor->IsEnabled())
		CubXSaversActor->Render(true);
	if(CubeOpenActor->IsEnabled())
		CubeOpenActor->Render(true);
	if(CubXRotationActor->IsEnabled())
		CubXRotationActor->Render(true);

	clCubXMenuMgr->SetActiveMenu();
	clCubXMenuMgr->Render();


	//  a enlever plus tard
	/*
	for(int i = 0;i<8;i++)
	{
	pCubXBtn[i]->Render();
	}
*/
}

void CLCubZ::RotateTo(int iCubeIndex)
{
	EnableCubXMainBtn(false);
	if(iCubeIndex == 0)
	{
		Disappear();
		Open();
		
	}else{
		CubXRotationActor->SetAnimationID(iCubeIndex);
		CubXRotationActor->PlayAnimation(1);
		bIsRotating = 1;
		}
iSelectedCube = iCubeIndex;
}

void CLCubZ::BackRotate(void)
{
	if(iSelectedCube ==0)
	{
		bIsBackRotating =0;
	}
	else{
		CubXRotationActor->SetAnimationID(iSelectedCube+1);
		CubXRotationActor->PlayAnimation(-1);
		bIsBackRotating = 1;

		float fKey = CubXRotationActor->GetKeyFrame();
		CubXRotationActor->SetKeyFrame(fKey -1);
		}
}
void CLCubZ::Open(void)
{

	CubeOpenActor->Enable(true);
	CubeOpenActor->PlayAnimation(1);
	bIsOpening = 1;
}
void CLCubZ::Close(void)
{

	CubeOpenActor->PlayAnimation(-1);
	bIsClosing = 1;
}
void CLCubZ::Disappear(void)
{
bIsDisappearing = 1;
CubXRotationActor->Enable(false);

}

void CLCubZ::EnableCubXMainBtn(bool bEnabled)
{
int i;

for(i=0;i<8;i++)
  {
	pCubXBtn[i]->Enable(bEnabled);
  }
}

void CLCubZ::SetCubXMenuButtons(void)
{
	// Niveau 0 = Cube fermé
	// Niveau 1 à 8 = Cube 1 à 8 ouvert
int i,j;
for(j=0;j<8;j++) // 8 niveau
{
for(i=0;i<4;i++) // 4 btn par niveau
  {
	clCubXMenuMgr->CreateBouton(clInsideCubXBtn[(j*4)+i]->GetMesh(),j+1);
  }
}

clCubXMenuMgr->CreateBouton(clInsideCubXExitBtn->GetMesh(),ALLMENU_EXCEPT_MAIN); // Main = Niveau 0
}