#pragma once
#include "StdAfx.h"


//  Improver pour pouvoir creer des primitive gerer par la physic et lighting de base...  ( box, sphere, cylinder)
// permettre aussi de creer des mesh non physics

class CL3DObject
{
public:
	CL3DObject(char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float fMasse, bool bStatic);
	~CL3DObject(void);

	void SetOpacity(float fOpacity);
	void AddTorque(cTV_3DVECTOR vTorque , bool bLocalSpace );
	void AddForce(cTV_3DVECTOR vForce , bool bLocalSpace );
	void SetTorque(cTV_3DVECTOR vTorque , bool bLocalSpace );
	void SetForce(cTV_3DVECTOR vForce , bool bLocalSpace );
	cTV_3DVECTOR GetTorque(bool bLocalSpace );
	cTV_3DVECTOR GetForce(bool bLocalSpace );
	CTVMesh* GetMesh(void);
	int GetPhysicBody(void);
	cTV_3DVECTOR GetPosition(void);
	void SetPosition(cTV_3DVECTOR);
	void Render(void);
	

	CTVMesh* Mesh;
	int iStdMaterial;
	int iPhysicBody;
	float fMasse;
	CTVScene* pTVScene;
	CTVPhysics* pTVPhysics;
	CTVMaterialFactory* pMatFactory; 
	
};

class CL3DPrimitive
{
	// iType =1  : Sphere  //  Changer tout ca pour avoir une classe de base qui coprends le tout et des classe mere pour les primitives
	// iType =2  : Box
public:
	CL3DPrimitive(int iType, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float fMasse, bool bStatic);
	~CL3DPrimitive(void);

	void SetOpacity(float fOpacity);
	void AddTorque(cTV_3DVECTOR vTorque , bool bLocalSpace );
	void AddForce(cTV_3DVECTOR vForce , bool bLocalSpace );
	void SetTorque(cTV_3DVECTOR vTorque , bool bLocalSpace );
	void SetForce(cTV_3DVECTOR vForce , bool bLocalSpace );
	cTV_3DVECTOR GetTorque(int iPhysicBody , bool bLocalSpace );
	cTV_3DVECTOR GetForce(int iPhysicBody , bool bLocalSpace );
	CTVMesh* GetMesh(void);
	int GetPhysicBody(void);
	cTV_3DVECTOR GetPosition(void);
	void SetPosition(cTV_3DVECTOR);
	void Render(void);
	
protected:
	CTVMesh* Mesh;
	int iStdMaterial;
	int iPhysicBody;
	float fMasse;
	CTVScene* pTVScene;
	CTVPhysics* pTVPhysics;
	CTVMaterialFactory* pMatFactory; 
	
};



class CL3DMesh
{
public:
	CL3DMesh(char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable);
	~CL3DMesh(void);

	void SetOpacity(float fOpacity);
	CTVMesh* GetMesh(void);
	cTV_3DVECTOR GetPosition(void);
	void SetPosition(cTV_3DVECTOR);
	void Render(void);
	
protected:
	CTVMesh* Mesh;
	int iStdMaterial;
	CTVScene* pTVScene;
	CTVMaterialFactory* pMatFactory; 
	
};

