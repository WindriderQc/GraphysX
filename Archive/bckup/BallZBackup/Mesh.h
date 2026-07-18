#pragma once
#include "TV3DMoteur.h"
#include "PhysicEntity.h"

class CLMesh
{
public:
	CLMesh(eObjet type, char* name, bool bEnable);
	virtual ~CLMesh(void);

	void Init(int iStdMaterial);
	GRAPHYSX_API void SetPosNRot(cTV_3DVECTOR Pos, cTV_3DVECTOR RotAngles);
	GRAPHYSX_API void SetMeshOpacity(float fOpacity);
	//GRAPHYSX_API void SetPPLShader(const char* pathNormalTex,  cTV_3DVECTOR lightPos, const char* name = (const char*)0, float fBumpAmount = 0);
	//GRAPHYSX_API void SetPPLShaderShadow(const char* pathNormalTex, cTV_3DVECTOR lightPos, const char* name = (const char*)0, float fBumpAmount = 0);
	//GRAPHYSX_API void SetFunkyShader(const char* pathNormalTex, cTV_3DVECTOR lightPos, const char* name = (const char*)0, float fBumpAmount = 0);
	//GRAPHYSX_API void SetShader(char* shaderFilename, cTV_3DVECTOR lightPos, const char* name);
	//GRAPHYSX_API void UpdateShadowMap(cTV_3DVECTOR lightpos);

	virtual void Debug(void) = 0 {};
	eObjet TypeMesh;
	CTVMesh* Mesh;
	CLPhysicEntity* clPhysicEntity = nullptr;
	
	

	char* filePath = "";
	char* texturename = "";
};