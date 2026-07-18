#include "3DObject.h"
//#include "SpecialEffects.h"

// suppression de warning a la compilation 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4305) // truncation double -> float 

CL3DObject::CL3DObject(char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float fMasse, bool bStatic)
{
	pTVScene = new CTVScene();
	pTVPhysics = new CTVPhysics;
	pMatFactory = new CTVMaterialFactory();

	// Create the Standard lighting material
	iStdMaterial = pMatFactory->CreateMaterial("StdMat");
	pMatFactory->SetAmbient(iStdMaterial, 0.1, 0.1, 0.1, 1);
	pMatFactory->SetDiffuse(iStdMaterial, 0.8, 0.8, 0.8, 1);
	pMatFactory->SetSpecular(iStdMaterial, 0.2, 0.2, 0.2, 1);
	pMatFactory->SetPower(iStdMaterial, 20);
	pMatFactory->SetEmissive(iStdMaterial, 0, 0, 0, 1);

	//  Create and set Mesh
	Mesh = pTVScene->CreateMeshBuilder("Mesh");
	bool bResult = Mesh->LoadTVM(PathToMesh,true,true);
	if(bResult == false)
		MessageBoxA(NULL, "The 3DObject TVM model hasn't been found...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);

	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	Mesh->SetScale(Scale.x, Scale.y, Scale.z); // 12
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetMaterial(iStdMaterial, -1);
	Mesh->SetShadowCast(true,true);

	if(Texture)
		Mesh->SetTexture(Texture, -1);
	if(bEnable == false)
		Mesh->Enable(false);
	
	//  Sets physical caracteristics
	// Avec une masse a 0, l'objet n'est pas affecté par la gravité.
	iPhysicBody = pTVPhysics->CreateBody(fMasse);
	if(bStatic)
	{
		pTVPhysics->CreateStaticMeshBody(Mesh, false, false);
	}else{
		pTVPhysics->AddMesh(iPhysicBody, Mesh, cTV_BODY_CONVEXHULL, true); 
	}
	pTVPhysics->SetBodyMovable(iPhysicBody, true);
	pTVPhysics->SetDamping(iPhysicBody,0.00001, new cTV_3DVECTOR(0.00001,0.00001,0.00001));
	
}

CL3DObject::~CL3DObject(void)
{
	delete(pTVScene); pTVScene = NULL;
	delete(pTVPhysics); pTVPhysics = NULL;
}

void CL3DObject::SetOpacity(float fOpacity)
{
	int iMat = Mesh->GetMaterial(-1);
	pMatFactory->SetOpacity(iMat,fOpacity);
	Mesh->SetMaterial(iMat,-1);
}

void CL3DObject::AddTorque(cTV_3DVECTOR vTorque , bool bLocalSpace )
{
	pTVPhysics->AddTorque(iPhysicBody,&vTorque, bLocalSpace);
}
void CL3DObject::AddForce(cTV_3DVECTOR vForce , bool bLocalSpace )
{
	pTVPhysics->AddForce(iPhysicBody,&vForce, bLocalSpace);
}
void CL3DObject::SetTorque(cTV_3DVECTOR vTorque , bool bLocalSpace )
{
		pTVPhysics->SetTorque(iPhysicBody,&vTorque, bLocalSpace);
}
cTV_3DVECTOR CL3DObject::GetTorque(bool bLocalSpace )
{
		return(pTVPhysics->GetTorque(iPhysicBody, bLocalSpace));
}
void CL3DObject::SetForce(cTV_3DVECTOR vForce , bool bLocalSpace )
{
		pTVPhysics->SetForce(iPhysicBody,&vForce, bLocalSpace);
}
cTV_3DVECTOR CL3DObject::GetForce( bool bLocalSpace )
{
		return(pTVPhysics->GetForce(iPhysicBody, bLocalSpace));
}
/*
void CL3DObject::SetMass(float fMass)
{
	
}*/
void CL3DObject::SetPosition(cTV_3DVECTOR position)
{
	//Mesh->SetPosition(position.x, position.y, position.z);
	pTVPhysics->SetBodyPosition(iPhysicBody, position.x, position.y, position.z);
}

cTV_3DVECTOR CL3DObject::GetPosition(void)
{
	return(Mesh->GetPosition());
}
CTVMesh* CL3DObject::GetMesh(void)
{
return(Mesh);
}
int CL3DObject::GetPhysicBody(void)
{
return(iPhysicBody);
}
void CL3DObject::Render(void)
{
	if(Mesh->IsEnabled())
		Mesh->Render();
}







/*////////////////////////////////////

Primitives Section

*////////////////////////////////////

CL3DPrimitive::CL3DPrimitive(int iType, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float fMasse, bool bStatic)
{
	pTVScene = new CTVScene();
	pTVPhysics = new CTVPhysics;
	pMatFactory = new CTVMaterialFactory();

	// Create the Standard lighting material
	iStdMaterial = pMatFactory->CreateMaterial("StdMat");
	pMatFactory->SetAmbient(iStdMaterial, 0.1, 0.1, 0.1, 1);
	pMatFactory->SetDiffuse(iStdMaterial, 0.8, 0.8, 0.8, 1);
	pMatFactory->SetSpecular(iStdMaterial, 0.2, 0.2, 0.2, 1);
	pMatFactory->SetPower(iStdMaterial, 20);
	pMatFactory->SetEmissive(iStdMaterial, 0, 0, 0, 1);

	//  Create and set Mesh
	Mesh = pTVScene->CreateMeshBuilder("Mesh");
	switch(iType)
		{
			case 1:	Mesh->CreateSphere(1); 
				break;
			case 2: Mesh->CreateBox(1,1,1);
				break;
		}
	
	
	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	Mesh->SetScale(Scale.x, Scale.y, Scale.z);
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetMaterial(iStdMaterial, -1);
	Mesh->SetShadowCast(true,true);

	if(Texture)
		Mesh->SetTexture(Texture, -1);
	if(bEnable == false)
		Mesh->Enable(false);
	
	//  Sets physical caracteristics
	// Avec une masse a 0, l'objet n'est pas affecté par la gravité.
	iPhysicBody = pTVPhysics->CreateBody(fMasse);
	if(bStatic)
	{
		pTVPhysics->CreateStaticMeshBody(Mesh, false, false);
	}else{
		switch(iType)
		{
			case 1:	pTVPhysics->AddMesh(iPhysicBody, Mesh, cTV_BODY_SPHERE, true); 
				break;
			case 2:pTVPhysics->AddMesh(iPhysicBody, Mesh, cTV_BODY_BOX, true); 
				break;
		}
	}
	pTVPhysics->SetBodyMovable(iPhysicBody, true);
	pTVPhysics->SetDamping(iPhysicBody,0.00001, new cTV_3DVECTOR(0.00001,0.00001,0.00001));
	
}

CL3DPrimitive::~CL3DPrimitive(void)
{
	delete(pTVScene); pTVScene = NULL;
	delete(pTVPhysics); pTVPhysics = NULL;
}

void CL3DPrimitive::SetOpacity(float fOpacity)
{
	int iMat = Mesh->GetMaterial(-1);
	pMatFactory->SetOpacity(iMat,fOpacity);
	Mesh->SetMaterial(iMat,-1);
}

void CL3DPrimitive::AddTorque(cTV_3DVECTOR vTorque , bool bLocalSpace )
{
	pTVPhysics->AddTorque(iPhysicBody,&vTorque, bLocalSpace);
}
void CL3DPrimitive::AddForce(cTV_3DVECTOR vForce , bool bLocalSpace )
{
	pTVPhysics->AddForce(iPhysicBody,&vForce, bLocalSpace);
}
void CL3DPrimitive::SetTorque(cTV_3DVECTOR vTorque , bool bLocalSpace )
{
		pTVPhysics->SetTorque(iPhysicBody,&vTorque, bLocalSpace);
}
cTV_3DVECTOR CL3DPrimitive::GetTorque(int iPhysicBody , bool bLocalSpace )
{
		return(pTVPhysics->GetTorque(iPhysicBody, bLocalSpace));
}
void CL3DPrimitive::SetForce(cTV_3DVECTOR vForce , bool bLocalSpace )
{
		pTVPhysics->SetForce(iPhysicBody,&vForce, bLocalSpace);
}
cTV_3DVECTOR CL3DPrimitive::GetForce(int iPhysicBody , bool bLocalSpace )
{
		return(pTVPhysics->GetForce(iPhysicBody, bLocalSpace));
}
void CL3DPrimitive::SetPosition(cTV_3DVECTOR position)
{
	//Mesh->SetPosition(position.x, position.y, position.z);
	pTVPhysics->SetBodyPosition(iPhysicBody, position.x, position.y, position.z);
}

cTV_3DVECTOR CL3DPrimitive::GetPosition(void)
{
	return(Mesh->GetPosition());
}
CTVMesh* CL3DPrimitive::GetMesh(void)
{
return(Mesh);
}
int CL3DPrimitive::GetPhysicBody(void)
{
return(iPhysicBody);
}
void CL3DPrimitive::Render(void)
{
	if(Mesh->IsEnabled())
		Mesh->Render();
}








/*

simple Mesh section

*/




CL3DMesh::CL3DMesh(char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable)
{
	pTVScene = new CTVScene();
	pMatFactory = new CTVMaterialFactory();

	// Create the Standard lighting material
	iStdMaterial = pMatFactory->CreateMaterial("StdMat");
	pMatFactory->SetAmbient(iStdMaterial, 0.1, 0.1, 0.1, 1);
	pMatFactory->SetDiffuse(iStdMaterial, 0.8, 0.8, 0.8, 1);
	pMatFactory->SetSpecular(iStdMaterial, 0.2, 0.2, 0.2, 1);
	pMatFactory->SetPower(iStdMaterial, 20);
	pMatFactory->SetEmissive(iStdMaterial, 0, 0, 0, 1);

	//  Create and set Mesh
	Mesh = pTVScene->CreateMeshBuilder("Mesh");
	bool bResult = Mesh->LoadTVM(PathToMesh,true,true);
	if(bResult == false)
		MessageBoxA(NULL, "The 3DObject TVM model hasn't been found...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);

	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	Mesh->SetScale(Scale.x, Scale.y, Scale.z); // 12
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetMaterial(iStdMaterial, -1);
	Mesh->SetShadowCast(true,true);

	if(Texture)
		Mesh->SetTexture(Texture, -1);
	if(bEnable == false)
		Mesh->Enable(false);
}

CL3DMesh::~CL3DMesh(void)
{
	delete(pTVScene); pTVScene = NULL;
}

void CL3DMesh::SetOpacity(float fOpacity)
{
	int iMat = Mesh->GetMaterial(-1);
	pMatFactory->SetOpacity(iMat,fOpacity);
	Mesh->SetMaterial(iMat,-1);
}

void CL3DMesh::SetPosition(cTV_3DVECTOR position)
{
	Mesh->SetPosition(position.x, position.y, position.z);
}

cTV_3DVECTOR CL3DMesh::GetPosition(void)
{
	return(Mesh->GetPosition());
}
CTVMesh* CL3DMesh::GetMesh(void)
{
return(Mesh);
}
void CL3DMesh::Render(void)
{
	if(Mesh->IsEnabled())
		Mesh->Render();
}









