#include "CubZ.h"

CLCubZ::CLCubZ(char* PathToMesh, float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable) 
	: CL3DObject(PathToMesh,Size,Texture,Pos, bEnable)
{
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

	CubeOpenActor = pTVScene->CreateActor("CubeOpenActor");
	CubeOpenActor->LoadTVA("media\\CubeOpen.tva", true, false);
	CubeOpenActor->SetScale(fSize,fSize,fSize);
	CubeOpenActor->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	CubeOpenActor->SetAnimationLoop(false);
	CubeOpenActor->SetMaterial(iStdMaterial);
	CubeOpenActor->Enable(false);


	CubXRotationActor = pTVScene->CreateActor("CubXRotationActor");
	CubXRotationActor->LoadTVA("media\\CubeRot.tva", true, false);
	CubXRotationActor->SetScale(fSize,fSize,fSize);
	CubXRotationActor->SetLightingMode(cTV_LIGHTING_MANAGED,-1,-1);
	CubXRotationActor->SetAnimationLoop(false);
	CubXRotationActor->SetMaterial(iStdMaterial);

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
	if( fKey <= 0.5 )
	{
		CubXRotationActor->StopAnimation();		
		bIsBackRotating = 0;
		iSelectedCube = -1;
	}
}
else if(bIsClosing)
{
	fKey = CubeOpenActor->GetKeyFrame();
	if( fKey <= (0.5) )
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

	//  a enlever plus tard
/*	for(int i = 0;i<8;i++)
	{
	pCubXBtn[i]->Render();
	}
*/
}

void CLCubZ::RotateTo(int iCubeIndex)
{
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