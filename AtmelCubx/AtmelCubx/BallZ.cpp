#include "BallZ.h"

CLBallZ::CLBallZ(void)
{
	pTVPhysics = new CTVPhysics();
	//cTV_3DVECTOR Pos;

	clLevel = new CLLevel();
	clLevel->LoadLevel("media\\slide1.tvm",1);
	
	clBall = new CLBall();
	//Pos.x = 0; Pos.y = 0; Pos.z = 0;
	//clBall->SetPosition(Pos);
	
	
	InitPhysicsMaterials();
}

CLBallZ::~CLBallZ(void)
{
delete(clLevel); clLevel = NULL;
delete(clBall); clBall = NULL;
}

void CLBallZ::Render(void)
{
	clLevel->Render();
	clBall->Render();


	
	

}
void CLBallZ::PushBall(int iDir)
{
clBall->PushBall(iDir); //AddImpulse  (_ballPhysicBody, new TV_3DVECTOR(7f, 7f, 220f));
}


void CLBallZ::InitPhysicsMaterials(void) 
{
    //Slide
    int pmatSlide = pTVPhysics->CreateMaterialGroup("Slide");
    pTVPhysics->SetMaterialInteractionFriction(0, pmatSlide, 1.0f, 0.9f);
    pTVPhysics->SetMaterialInteractionBounciness(0, pmatSlide, 0.01f); 
	pTVPhysics->SetMaterialInteractionSoftness(0, pmatSlide, 1.0f);   // Peu d'impact    entr 0 et 1
	pTVPhysics->SetBodyMaterialGroup(clLevel->GetSlidePhysicBody(), pmatSlide);  

    //Ball 
    int pmatBall = pTVPhysics->CreateMaterialGroup("Ball");
    pTVPhysics->SetMaterialInteractionFriction(pmatBall, pmatSlide, 0.2f, 0.075f);
    pTVPhysics->SetMaterialInteractionBounciness(pmatBall, pmatSlide, 0.05f);
    pTVPhysics->SetMaterialInteractionSoftness(pmatBall, pmatSlide, 1.0f);
	pTVPhysics->SetBodyMaterialGroup(clBall->GetPhysicBody(), pmatBall);
}

CTVMesh* CLBallZ::GetBallMesh(void)
{
	return(clBall->GetMesh());
}
int CLBallZ::GetBallPhysicBody(void)
{
	return(clBall->GetPhysicBody());
}
int CLBallZ::GetSlidePhysicBody(void)
{
	return(clLevel->GetSlidePhysicBody());
}
cTV_3DVECTOR CLBallZ::GetBallPosition(void)
{
	return(clBall->GetPosition());
}
cTV_3DVECTOR CLBallZ::GetBallSpeed(void)
{
	return(clBall->GetSpeed());
}