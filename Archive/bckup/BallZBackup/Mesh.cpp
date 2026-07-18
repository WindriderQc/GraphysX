#include "stdafx.h"
#include "Mesh.h"

CLMesh::CLMesh(eObjet type, char* name, bool bEnable) : TypeMesh(type)
{
	//  Create and set Mesh

	Mesh = GraphysX::CLTV3D::pTVScene.CreateMeshBuilder(name);
	Mesh->Enable(bEnable);
	
}
CLMesh::~CLMesh(void)
{
	if (Mesh)GraphysX::CLTV3D::pTVScene.DestroyMesh(Mesh);  Mesh = nullptr; // Mesh->Destroy();
	if (clPhysicEntity) delete(clPhysicEntity); clPhysicEntity = nullptr;
}
void CLMesh::Init(int iStdMaterial)
{
	Mesh->SetBlendingMode(cTV_BLEND_ALPHA);
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetShadowCast(true, false);  //  A tester, sert juste pour les mesh emissive je crois
	if (!Mesh->GetMaterial(0)) // Applique le Material par défaut si aucun présent dans le mesh loadé
		Mesh->SetMaterial(iStdMaterial, -1);
}
void CLMesh::SetPosNRot(cTV_3DVECTOR Pos, cTV_3DVECTOR RotAngles)  //  Devrait etre modifié pour travailler avec les matrices
{
	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	Mesh->SetRotation(RotAngles.x, RotAngles.y, RotAngles.z);
	if (clPhysicEntity)
	{
		cTV_3DMATRIX mat = Mesh->GetMatrix();
		NewtonBodySetMatrix(clPhysicEntity->pbody,&mat.m11);
	}
}
void CLMesh::SetMeshOpacity(float fOpacity) 
{
	int iMat = Mesh->GetMaterial(-1);
	GraphysX::CLTV3D::pTVMaterialfactory.SetOpacity(iMat, fOpacity);
	Mesh->SetMaterial(iMat, -1);
}
/*
void CLMesh::SetFunkyShader(const char* pathNormalTex, cTV_3DVECTOR lightPos, const char* name, float fBumpAmount) {

	CTVTextureFactory pTexFac;
	CTVScene pScene;
	// Shader test
	//int IDDiffuse = pTexFac.LoadTexture("..\\StockRoom\\Zack.jpg", "diffuse texture", -1, -1, cTV_COLORKEY_NO, true);
	int IDNormal = pTexFac.LoadTexture(pathNormalTex, "Bump texture", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true); // 
	//float fBumpAmount = 0.01f;// 001f;
	m_Shader = pScene.CreateShader("Objet_simple");
	m_Shader->CreateFromEffectFile("..\\StockRoom\\shaders\\funky.shade");
	m_Shader->SetEffectParamVector3("LPos", &lightPos); // &Vector3(0, 2, -10));      //Give the shader a 3-vector, the shader will take this as a light position.
	m_Shader->SetEffectParamFloat("BumpAmount", fBumpAmount);
	m_Shader->SetEffectParamTexture("Normal", IDNormal); //Set our bump map so the shader can find it. You can do this via semantics, but I wanted to demonstrate SetEffectParam etc.
	//Apply the shader to the mesh.
	Mesh->SetShader(m_Shader);
	//Tell TV we want tagent information etc.
	Mesh->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE, -1, -1); // cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE
};*/