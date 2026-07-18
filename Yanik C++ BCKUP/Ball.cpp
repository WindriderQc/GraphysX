#include "Ball.h"
//#include "SpecialEffects.h"

CLBall::CLBall(void)
{

	pTVScene = new CTVScene();
	//pTVTexFac = new CTVTextureFactory();;

	BallMesh = pTVScene->CreateMeshBuilder("Ball");
	BallMesh->LoadTVM("Media\\Ball.tvm",true,true);

	Position.x = 0; Position.y =  0; Position.z =  0;
	Direction.x = 0; Direction.y = 0; Direction.z = 0;
	iSpeed = 0;
	iAcceleration = 0;
	
	LoadEffects();
}

CLBall::~CLBall(void)
{
	delete(pTVScene); pTVScene = NULL;
	//delete(pTVTexFac); pTVTexFac = NULL;
}

void CLBall::SetSpeed(int speed)
{
	iSpeed = speed;
}
void CLBall::SetDirection(cTV_3DVECTOR direction)
{
	Direction.x = direction.x;
	Direction.y = direction.y;
	Direction.z = direction.z;
}
void CLBall::SetPosition(cTV_3DVECTOR position)
{
	Position.x = position.x;
	Position.y = position.y;
	Position.z = position.z;
	BallMesh->SetPosition(Position.x, Position.y, Position.z);
}
int CLBall::GetSpeed(void)
{
	return(iSpeed);
}
cTV_3DVECTOR CLBall::GetDirection(void)
{
	return(Direction);
}
cTV_3DVECTOR CLBall::GetPosition(void)
{
	return(Position);
}
CTVMesh* CLBall::GetMesh(void)
{
return(BallMesh);
}

void CLBall::Render(void)
{
	BallMesh->Render();
}

void CLBall::LoadEffects(void)
{
//clSpecialEffects[0] = CLExplosion();  //  arranger la classe explosion pour permettre d'Entrer un param au constructeur qui choisira l'effet...
// +++   et checker comment se servir du enum....
}