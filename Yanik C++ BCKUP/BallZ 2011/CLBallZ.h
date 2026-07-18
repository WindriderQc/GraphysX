#pragma once
#include "StdAfx.h"
#include "3DObject.h"
#include "Global.h"
#include "Input.h"

/*
struct STMoveControl
	{
		float Power;
		bool Accel;
		bool Brake;
		float fSteerAngle;
		bool Handbrake;

		bool Up;
		bool Down;
		bool Left;
		bool Right;
	};
*/

class CLShell
{
protected:
	CTVScene* pTVScene;
	CTVMaterialFactory* pMatFactory; 
	CTVPhysics* pTVPhysics;

public:
	int iPhysicBody;
	CTVMesh* Mesh;
	void Render(void)                                        { Mesh->Render();                                                       };
	void AddTorque(cTV_3DVECTOR vTorque, bool bLocalSpace )  { pTVPhysics->AddTorque(iPhysicBody, &vTorque, bLocalSpace);            };
	void AddForce(cTV_3DVECTOR vForce, bool bLocalSpace )    { pTVPhysics->AddForce(iPhysicBody, &vForce, bLocalSpace);              };
	cTV_3DVECTOR GetTorque(bool bLocalSpace )                { return(pTVPhysics->GetTorque(iPhysicBody, bLocalSpace));              };
	void SetTorque(cTV_3DVECTOR vTorque, bool bLocalSpace )  { pTVPhysics->SetTorque(iPhysicBody, &vTorque, bLocalSpace);            };
	cTV_3DVECTOR GetLinearVelocity(bool bLocalSpace )        { return(pTVPhysics->GetBodyLinearVelocity(iPhysicBody, bLocalSpace));  };
	cTV_3DVECTOR GetAngularVelocity(bool bLocalSpace )       { return(pTVPhysics->GetBodyAngularVelocity(iPhysicBody, bLocalSpace));  };
	void SetAngularVelocity(cTV_3DVECTOR velo, bool bLocalSpace )  { pTVPhysics->SetBodyAngularVelocity(iPhysicBody, &velo, bLocalSpace);  };

	CLShell(cTV_3DVECTOR pos, cTV_3DVECTOR scale)
	{
	pMatFactory = new CTVMaterialFactory();
	pTVScene= new CTVScene();

	// Create the GlassStylish lighting material
	int testmat = pMatFactory->CreateMaterial("BallShellMat");
	pMatFactory->SetAmbient(testmat, 0.1, 0.1, 0.1, 0.4);
	pMatFactory->SetDiffuse(testmat, 0.8, 0.8, 0.8, 0.4);
	pMatFactory->SetSpecular(testmat, 0.2, 0.2, 0.2, 0.4);
	pMatFactory->SetPower(testmat, 20);
	pMatFactory->SetEmissive(testmat, 0, 0, 0, 0.3);
	//  Create and set Mesh
	Mesh = pTVScene->CreateMeshBuilder("Mesh");
	Mesh->CreateSphere(1, 24,24);
	Mesh->SetScale(scale.x,scale.y,scale.z);
	Mesh->SetPosition(pos.x,pos.y, pos.z);
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetMaterial(testmat);
	Mesh->SetAlphaTest(true, 0, true, -1);
	Mesh->SetBlendingMode(cTV_BLEND_ALPHA);
	
	

//  Sets physical caracteristics
	iPhysicBody = pTVPhysics->CreateBody(1250);  // Masse du bois dur, a retester avec masse du plomb mais revoir la gravité avant
	pTVPhysics->AddMesh(iPhysicBody, Mesh, cTV_BODY_SPHERE, true); 
	pTVPhysics->SetBodyMovable(iPhysicBody, true);
	pTVPhysics->SetDamping(iPhysicBody, 0.00001, new cTV_3DVECTOR(0.00001,0.00001,0.00001));
	};
	~CLShell(void);
};


class CLCtrl
{
protected:
	CTVScene* pTVScene;

public:
	CTVMesh* Mesh;
	void Render(void)     { Mesh->Render(); };

	CLCtrl(cTV_3DVECTOR scale)
	{
	pTVScene= new CTVScene();

	//  Create and set Mesh
	Mesh = pTVScene->CreateMeshBuilder("BallZCtrlMesh");
	Mesh->LoadTVM("Media\\Ball\\BallFire.tvm");
	Mesh->SetScale(scale.x,scale.y,scale.z);
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	//mCtrl->SetAlphaTest(true, 0, true, -1);
	//mCtrl->SetBlendingMode(cTV_BLEND_ALPHA);
	};
	~CLCtrl(void);
};


class CLBallZ
{
public:
	CLBallZ(void);
	~CLBallZ(void);

private:
	//CLGlobalVar* clGlobalVar;
	CLInput* clInput;
	CTVMathLibrary* pTVMaths;
	CTVScreen2DImmediate* p2DImmediate;
	
public:
	CLCtrl* clCtrl;
	CLShell* clShell;

	void ManageInput(void);
	void Render(void);
};





