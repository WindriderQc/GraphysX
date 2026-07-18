#include "BallZ.h"

CLBallZ::CLBallZ(void)
{
	cTV_3DVECTOR Pos;
	//  TEST POUR BallZ
	clLevel = new CLLevel();
	clLevel->LoadLevel("media\\slide1a.tvm",1);
	
	clBall = new CLBall();
	//Pos.x = 0; Pos.y = 0; Pos.z = 0;
	//clBall->SetPosition(Pos);
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

CTVMesh* CLBallZ::GetBallMesh(void)
{
	return(clBall->GetMesh());
}
