#include "StdAfx.h"
#include "CLBallZ.h"


void BallZForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
{
	float masse;
	cTV_3DVECTOR inertie;
	cTV_3DVECTOR force;
	dVector Torque, Jump;
	// On récupère en premier lieu la masse ainsi que l'inertie
	NewtonBodyGetMassMatrix(body, &masse, &inertie.x, &inertie.y, &inertie.z);
	force.x = 0.0f;
	force.y = -masse * GRAVITY * SCALE;
	force.z = 0.0f;
	NewtonBodyAddForce(body, &force.x); // On ajoute la force au corps

	bTest = 1;

	//fThrottleStep = 1;

	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	char* test = cl3DObject->Mesh->GetMeshName();

	if (cl3DObject->bJumping)
	{
		if (cl3DObject->bAllowJumps)   // A Arranger pour que le saut soit a 45deg dans la direction du joystick !!!!!!!!!!!!!!!!!!!
		{
			Jump.m_x = 0;
			Jump.m_y = 1000 * SCALE;
			Jump.m_z = 0;
			NewtonBodyAddForce(body, &Jump.m_x); // On ajoute la force au corps
			CLSound::getInstance()->PlaySound(3);
			cl3DObject->bJumping = false;
		}
		cl3DObject->bAllowJumps = false;
	}
	if (cl3DObject->bCTRLBALL)
	{
		dVector Omega;
		Torque.m_x = cl3DObject->vCtrlTorque.x * SCALE;
		Torque.m_y = cl3DObject->vCtrlTorque.y * SCALE;
		Torque.m_z = cl3DObject->vCtrlTorque.z * SCALE;
		NewtonBodyGetOmega(body, &Omega.m_x);
		dVector finaltorque(Torque.Scale(inertie.x *75.0f) - Torque.Scale(2.0f * inertie.x * (Omega % Torque)));
		NewtonBodyAddTorque(body, &Torque.m_x);
	}
}
void BallZTransformationCallback (const NewtonBody* body, const dFloat* matrix, int threadIndex) 
{
	cTV_3DMATRIX* MX = NULL;
	NewtonBodyGetMatrix(body, MX->m[0]);
	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	cl3DObject->Mesh->SetMatrix(MX);
}
CLBallZ::CLBallZ(NewtonWorld* World, cTV_3DVECTOR pos, float Radius, float Masse,int iNewtonMat)
{
	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	fRadius = Radius;
	fMass = Masse;

	// CTRL Section
	int iFIreArrowTex = clTV3D->pTVTexturefactory->LoadTexture("Media\\FireArrow800.jpg", "FireArrow");
	clCtrl = new CL3DObject(SPHERE, "BallZCtrl", NULL, iFIreArrowTex, pos, cTV_3DVECTOR(fRadius*0.85f, fRadius*0.85f, fRadius*0.85f), true, NULL, NULL, iNewtonMat, World);
	clCtrl->Mesh->RotateX(-90);
	//SHELL section
	clShell = new CL3DObject(SPHERE, "BallZShell", NULL, NULL, cTV_3DVECTOR(pos.x, pos.y, pos.z), cTV_3DVECTOR(fRadius, fRadius, fRadius), true, fMass, PHYSICALL, iNewtonMat, World);
	clShell->vCtrlTorque = cTV_3DVECTOR(0, 0, 0);  //WEIRD DANS LE DESIGN...  A PAS RAPPORT DANS 3DOBJECT!!!
	clShell->Mesh->SetMaterial(clTV3D->iGlassMat);
	clShell->Mesh->SetAlphaTest(true, 0, true, -1);
	// On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
	NewtonBodySetForceAndTorqueCallback(clShell->pbody, BallZForceAndTorqueCallback);
	NewtonBodySetTransformCallback(clShell->pbody, BallZTransformationCallback);


	vTorque = cTV_3DVECTOR(0, 0, 0);
	fThrottleStep = 0.05f;
	fThrottle = 0.0f;
	fMaxThrottle = 0.5f;
	clShell->bCTRLBALL = true;     //   DESIGN LOUCHE!!!  A pas rapport dans 3DObject 
	clShell->bJumping = false;     //   DESIGN LOUCHE!!!
	clShell->bAllowJumps = false;  //   DESIGN LOUCHE!!!
	clShell->bIsFinishline = false;//   DESIGN LOUCHE!!!
}
CLBallZ::~CLBallZ(void)
{
}
void CLBallZ::Update(void)
{
	cTV_3DVECTOR vec = clShell->Mesh->GetPosition();
	cTV_3DVECTOR rot = clCtrl->Mesh->GetRotation();
	clCtrl->SetPosition(vec, rot); 
	CalculateSpeed(clShell->Mesh->GetPosition(), clGlobalVar->fTimeElapsed);
	clGlobalVar->fBallSpeed = fSpeed;
}
void CLBallZ::SetPosition(cTV_3DVECTOR Pos, cTV_3DVECTOR Dir)
{
	cTV_3DMATRIX MX;
	clShell->Mesh->SetPosition(Pos.x,Pos.y,Pos.z);
	// Ajouter le SetOrientation ici!!!!!!
	MX = clShell->Mesh->GetMatrix();
	NewtonBodySetMatrix(clShell->pbody, MX.m[0]);
	// On immobilise le corps pour supprimer le momentum précédent
	cTV_3DVECTOR force = cTV_3DVECTOR(0,0,0); 
	NewtonBodySetForce (clShell->pbody, &force.x); 
}