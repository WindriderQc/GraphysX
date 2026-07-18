#include "Ball.h"
//#include "SpecialEffects.h"

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 

CLBall::CLBall(void)
{ 	
	pTVPhysics = new CTVPhysics();

	fSize = 0.5;
	iSpeed = 0;
	iAcceleration = 0;
	Position.x = -50; Position.y =  -4750; Position.z =  -250;
	Direction.x = 0; Direction.y = 0; Direction.z = 0;

	clBallObject = new CL3DObject("Media\\Ball.tvm", fSize, NULL, Position, true);
	
	fMasse = 5000;
	pTVPhysics->SetBodyMass(clBallObject->GetPhysicBody(), fMasse);
	pTVPhysics->EnableDebugInfo(clBallObject->GetPhysicBody(),true,-1);
	RotFront =0;
	RotLeft = 0;
	RotRight = 0;
	RotBack = 0; 
	
	Torque.x =0;
	Torque.y = 0;
	Torque.z = 0;

	LoadEffects();
}

CLBall::~CLBall(void)
{
	delete(clBallObject);  clBallObject = NULL;
	delete(pTVPhysics);	   pTVPhysics = NULL;
}

void CLBall::PushBall(int iDir)
{
	cTV_3DVECTOR tempvec;
	tempvec.x = 0;
	tempvec.y = 0;
	tempvec.z = 0;

	switch(iDir)
	{
	case FRONT:
				//pTVPhysics->AddImpulse(iBallPhysicBody, new cTV_3DVECTOR(7.0, 7.0, 220.0), false);
				RotFront= 10;
				tempvec.z = 100;
				Torque = Torque + tempvec;
				TorqueFront.x = 0.0; TorqueFront.y = 0.0; TorqueFront.z = 100.0;
				break;
	case BACK:
				RotBack= 1;
				tempvec.z = -100;
				Torque = Torque + tempvec;
				TorqueBack.x = 0.0; TorqueBack.y = 0.0; TorqueBack.z = -10.0;
				break;
	case LEFT:
				RotLeft= 10;
				tempvec.x = -100;
				Torque = Torque + tempvec;
				TorqueLeft.x = -100.0; TorqueLeft.y = 0.0; TorqueLeft.z = 0.0;
				break;
	case RIGHT:
				RotRight= 10;
				tempvec.x = 100;
				Torque = Torque + tempvec;
				TorqueRight.x = 100.0; TorqueRight.y = 0.0; TorqueRight.z = 0.0;
				break;
	}
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
	//  Marche pas car pas encore implementer
}
void CLBall::SetPosition(cTV_3DVECTOR position)
{
	Position.x = position.x;
	Position.y = position.y;
	Position.z = position.z;
	clBallObject->GetMesh()->SetPosition(Position.x, Position.y, Position.z);
}
cTV_3DVECTOR CLBall::GetSpeed(void)
{
	return(pTVPhysics->GetBodyLinearVelocity(clBallObject->GetPhysicBody(),false));
}
cTV_3DVECTOR CLBall::GetDirection(void)
{
	return(Direction); // Marche pas car direction est jamais updater.
}
cTV_3DVECTOR CLBall::GetPosition(void)
{
	return(clBallObject->GetMesh()->GetPosition());
}
CTVMesh* CLBall::GetMesh(void)
{
return(clBallObject->GetMesh());
}
int CLBall::GetPhysicBody(void)
{
	return(clBallObject->GetPhysicBody());
}
void CLBall::Render(void)
{
	if(clBallObject->GetMesh()->IsEnabled())
		clBallObject->GetMesh()->Render();
	//pTVPhysics->AddTorque(iBallPhysicBody,&Torque, true);
	if ( RotBack > 0 ) 
	{ 
	pTVPhysics->AddImpulse(clBallObject->GetPhysicBody(), &Torque, false);
	RotBack --; 
    } 

 /*   if ( RotFront > 0 ) 
	{ 
		 pTVPhysics->AddTorque(iBallPhysicBody,&TorqueFront, false);
		 RotFront --; 
    } 
	if ( RotBack > 0 ) 
	{ 
		 pTVPhysics->AddTorque(iBallPhysicBody,&TorqueBack, false);
		 RotBack --; 
    } 
	if ( RotLeft > 0 ) 
	{ 
		 pTVPhysics->AddTorque(iBallPhysicBody,&TorqueLeft, false);
		 RotRight --; 
    } 
	if ( RotRight > 0 ) 
	{ 
		 pTVPhysics->AddTorque(iBallPhysicBody,&TorqueRight, false);
		 RotRight --; 
    } 
	*/
}

void CLBall::LoadEffects(void)
{
//clSpecialEffects[0] = CLExplosion();  //  arranger la classe explosion pour permettre d'Entrer un param au constructeur qui choisira l'effet...
// +++   et checker comment se servir du enum....
}