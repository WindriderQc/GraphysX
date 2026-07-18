#include "StdAfx.h"
#include "3DObject.h"
//#include "SpecialEffects.h"

// suppression de warning a la compilation 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4305) // truncation double -> float 


//  ajouter la masse, l'orientation, le light material, 
CL3DObject::CL3DObject(string PathToMesh, float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable)
{
	pTVScene = new CTVScene();
	pTVPhysics = new CTVPhysics;
	pMatFactory = new CTVMaterialFactory();

	// Create the Standard lighting material     //  mettre dans une classe contenant des chose pratique pour toutes les scene
	iStdMaterial = pMatFactory->CreateMaterial("StdMat");
	pMatFactory->SetAmbient(iStdMaterial, 0.1, 0.1, 0.1, 1);
	pMatFactory->SetDiffuse(iStdMaterial, 0.8, 0.8, 0.8, 1);
	pMatFactory->SetSpecular(iStdMaterial, 0.2, 0.2, 0.2, 1);
	pMatFactory->SetPower(iStdMaterial, 20);
	pMatFactory->SetEmissive(iStdMaterial, 0, 0, 0, 1);

	//  Create and set Mesh
	Mesh = pTVScene->CreateMeshBuilder("Mesh");
	bool bResult = Mesh->LoadTVM(PathToMesh.c_str(),true,true);
	if(bResult == false)
		MessageBoxA(NULL, "The 3DObject TVM model hasn't been found...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);

	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	Mesh->SetScale(Size,Size,Size); 
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetMaterial(iStdMaterial, -1);
	Mesh->SetShadowCast(true,true);

	if(Texture)
		Mesh->SetTexture(Texture, -1);
	if(bEnable == false)
		Mesh->Enable(false);
	
	//  Sets physical caracteristics
	fMasse = 0;  // Avec une masse a 0, l'objet n'est pas affecté par la gravité.
	iPhysicBody = pTVPhysics->CreateBody(fMasse);
    pTVPhysics->AddMesh(iPhysicBody, Mesh, cTV_BODY_CONVEXHULL, true); 
    pTVPhysics->SetBodyMovable(iPhysicBody, true);
	pTVPhysics->SetDamping(iPhysicBody,0.00001, new cTV_3DVECTOR(0.00001,0.00001,0.00001));

}

CL3DObject::~CL3DObject(void)
{
	delete(pTVScene); pTVScene = NULL;
	delete(pTVPhysics); pTVPhysics = NULL;
}
//  On devrait pouvoir setter juste soit le mesh ou le physic body et l'autre suivrait,...   a tester.
void CL3DObject::SetPosition(cTV_3DVECTOR position)
{
	Mesh->SetPosition(position.x, position.y, position.z);
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