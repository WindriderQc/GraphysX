#include "CLBallZ.h"


CLBallZ::CLBallZ(void)
{
	//clGlobalVar = CLGlobalVar::getInstance();
	clInput = CLInput::getInstance();
	pTVMaths = new CTVMathLibrary();
	p2DImmediate = new CTVScreen2DImmediate();

	clShell = new CLShell(cTV_3DVECTOR(0,50,0), cTV_3DVECTOR(1,1,1));	
	clCtrl = new CLCtrl(cTV_3DVECTOR(0.1,0.1,0.1));  //  Ctrl est un TVM qui doit etre refait pour avoir un unit de 1 de base..  sinon le scale est offset

}
CLBallZ::~CLBallZ(void)
{
}

void CLBallZ::Render(void)
{
	cTV_3DVECTOR vec = clShell->Mesh->GetPosition();
	clCtrl->Mesh->SetPosition(vec.x, vec.y, vec.z);
	
	clCtrl->Render();
	clShell->Render();
}


void CLBallZ::ManageInput(void)
{
	float fTORQ = 5000.0f;
	cTV_3DVECTOR vec, vecTorque, NewtonTorque;
	int iSlowFactor = 1;
	float fRotSpeed = 0.2f;
	float Angle;
	float Y;	
	
	//if(clInput->stGamePad.LeftY > 0)
	//	Y = clInput->stGamePad.LeftY * -1;  //  on ne prend pas compte si le joystick pointe vers le bas

	Angle = pTVMaths->Direction2Ang(clInput->stGamePad.LeftX, clInput->stGamePad.LeftY); // Y);
	vec = clCtrl->Mesh->GetRotation();
	clCtrl->Mesh->SetRotation(vec.x, Angle, vec.z);	

	/*vecTorque.x = clInput->stGamePad.LeftX / 100; 
	vecTorque.y = 0;
	vecTorque.z =  clInput->stGamePad.LeftY / 100;*/
	vecTorque.x = 10; 
	vecTorque.y = 0;
	vecTorque.z = 0;

	
	//vecTorque = pTVMaths->VNormalize(&vecTorque);
	
	//////////
	//  Show Torque Section
	/////////////
	cTV_3DVECTOR VecPos;
	long lVecLenght;
	cTV_3DVECTOR VecVelo;
	int iAxisLength = 5;
	VecPos = clShell->Mesh->GetPosition();
	p2DImmediate->Draw_Line3D(  VecPos.x, VecPos.y, VecPos.z, 
								VecPos.x + vecTorque.x*2, 
								VecPos.y,
								VecPos.z + vecTorque.z*2,
								AQUA, -2); 


	if(clInput->stGamePad.b1) //if(clInput->stGamePad.RightY < -90)  // forward
	{	
	/*	VecVelo = clShell->GetVelocity(false);
		vecTorque.x = (vecTorque.x * (1 - VecVelo.x)) * fTORQ;
		vecTorque.y = (vecTorque.y * (1 - VecVelo.y)) * fTORQ;
		vecTorque.z = (vecTorque.z * (1 - VecVelo.z)) * fTORQ;
*/
		vecTorque = vecTorque * fTORQ;
		// Tweak pour setter le torque dans le bon sens... car Newton et TV3D ne semble pas avoir le meme systeme d'Axe
		NewtonTorque.x = vecTorque.z;
		NewtonTorque.y = vecTorque.y;
		NewtonTorque.z = -vecTorque.x;
		
		vec = clCtrl->Mesh->GetRotation();
		clShell->Mesh->SetRotation(vec.x,vec.y,vec.z);
		clShell->SetTorque( NewtonTorque, false); 
		
		clCtrl->Mesh->SetRotation(vec.x +fRotSpeed,vec.y, vec.z );
		//clCtrl->GetMesh()->RotateX(fRotSpeed, true);
	}else{
		vec = clShell->GetAngularVelocity(true);
		/*vec = vec * -1;
		//Angle = pTVMaths->Direction2Ang(vec.x, vec.z);
		vec = pTVMaths->VNormalize(&vec);
		clShell->AddTorque(vec * iSlowFactor, true);*/
		lVecLenght = pTVMaths->TVVec3Length(&vec);
		if(lVecLenght > 3)
			{
				//NewtonBodyAddTorque(body, (MyWantedSpeed-ActualSpeed)/TimeStep);
				clShell->SetAngularVelocity((clShell->GetAngularVelocity(true) * 0.5), true);
			}
			

		}
	if(clInput->stGamePad.b0)  //if(clInput->stGamePad.RightY > 90)  // Backward   //  Arranger pour que la force de rotation soit en fonction de l'analog right
	{
		clShell->AddForce(cTV_3DVECTOR(-10,0,0),true);  // Jump 
	}
	//if(clInput->stGamePad.b0)
	//		clShell->AddTorque(cTV_3DVECTOR(0,0,2),false);

}



