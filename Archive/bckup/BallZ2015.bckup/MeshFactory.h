#ifndef __MeshFactory_H__
#define __MeshFactory_H__
#pragma once
#include "TV3DMoteur.h"
#include "PhysicEntity.h"

// CLMESH SECTION

void PhysicMeshTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);
void MeshControlledTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);

class CLMesh
{
public:
	CLMesh(eObjet type, char* name, bool bEnable);
	virtual ~CLMesh(void);

	void Init(void);
	void SetPosNRot(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot);
	virtual void Debug(void) = 0 {};
	eObjet TypeMesh;
	CTVMesh* Mesh;	

};

class CLMeshSphere : public CLMesh
{
public: CLMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(SPHERE, name, bEnable) {
		Mesh->CreateSphere(Scale.x, 24, 24);
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		if (iTexture != -1)	Mesh->SetTexture(iTexture, -1);
		Init();
	};
	virtual ~CLMeshSphere() {};
	void Debug(void){ };
};
class CLMeshCube : public CLMesh
{
public:
	CLMeshCube(cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable) : CLMesh(CUBE,"Cube", bEnable) {
		int i = Mesh->CreateBox(Scale.x, Scale.y, Scale.z);
		Mesh->SetPosition(Pos.x,Pos.y,Pos.z);
		Init();
	};
	CLMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(CUBE, name, bEnable) {
		Mesh->CreateBox(Scale.x, Scale.y, Scale.z);
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		if (iTexture != -1)	Mesh->SetTexture(iTexture, -1);
		Init();
	};
	virtual ~CLMeshCube() {};
	void Debug(void){ };
};
class CLMeshCylinder : public CLMesh
{
public: 
	CLMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(CYLINDER, name, bEnable) {
		Mesh->CreateCylinder(Scale.x, Scale.y, 12, true);  // radius, height, Model precision = 12, capped = true.  A mettre en param..
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		if (iTexture != -1)	Mesh->SetTexture(iTexture, -1);
		Init();
	};
	virtual ~CLMeshCylinder() {};
	void Debug(void){ };
};
class CLMeshCone : public CLMesh
{
public:
	CLMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(CONE, name, bEnable) {
		Mesh->CreateCone(Scale.x, Scale.y, 12, true);  // radius, height, Model precision = 12, capped = true.  A mettre en param..
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		if (iTexture != -1)	Mesh->SetTexture(iTexture, -1);
		Init();
	};
	virtual ~CLMeshCone() {};
	void Debug(void){ };
};
class CLMeshFloor : public CLMesh
{
public:
	CLMeshFloor(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(FLOOR, name, bEnable) {
		Mesh = CLTV3DMoteur::getInstance()->pTVScene->CreateMeshBuilder();
		Mesh->AddFloor(iTexture, -(Scale.x / 2), -(Scale.y / 2), Scale.x / 2, Scale.y / 2, Pos.y);
		Init();
	};
	~CLMeshFloor() {};
	void Debug(void){ };
};
class CLMeshBillboard : public CLMesh
{
public:
	CLMeshBillboard(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(BILLBOARD, name, bEnable) {
		Mesh = CLTV3DMoteur::getInstance()->pTVScene->CreateBillboard(iTexture, Pos.x, Pos.y, Pos.z, Scale.x, Scale.y, name, false);
		Init();
	};
	~CLMeshBillboard() {};
	void Debug(void){ };
};
class CLMeshCustom : public CLMesh
{
public:
	CLMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(CUSTOM, name, bEnable) {

		bool bResult = Mesh->LoadTVM(PathToMesh, true, true);
		if (Mesh->LoadTVM(PathToMesh, true, true)) {
			Mesh->SetScale(Scale.x, Scale.y, Scale.z); //fait crasher un bout de la physics
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			cTV_3DMATRIX after = Mesh->GetMatrix();
			if (iTexture != -1)	Mesh->SetTexture(iTexture, -1);
			Init();
		}else	
			MSGBOX("The 3DObject TVM model hasn't been found...", "Warning");
	};
	virtual ~CLMeshCustom() {};
	void Debug(void){ };
};
class CLMeshX : public CLMesh
{
public:
	CLMeshX(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(XMESH, name, bEnable) {

		bool bResult = Mesh->LoadXFile(PathToMesh);
		if (bResult == false)
			MessageBoxA(NULL, "The X Mesh model hasn't been found...", "Warning", MB_ICONEXCLAMATION | MB_SETFOREGROUND);
		Mesh->SetAlphaTest(true);
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		Mesh->SetScale(Scale.x, Scale.y, Scale.z);  //fait crasher un bout de la physics   ?? still valid?

		Init();
	};
	virtual ~CLMeshX() {};
	void Debug(void){ };
};
class CLDuplicate : public CLMesh
{
public:
	CLDuplicate(CLMesh* Obj, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(DUPLICATE, name, bEnable) {

	Mesh = Obj->Mesh->Duplicate(name);
	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	//Mesh->SetScale(Scale.x, Scale.y, Scale.z); Not implemented...   le duplicate est par défaut en shared Memory...  
	//voir si on peux scaler un element en mettant shared Mem a false dans le duplicate.
	};
	virtual ~CLDuplicate() {};
	void Debug(void){ };
};

class CLPhysicMeshCube : public CLMeshCube, public CLPhysicCube
{
public:
	CLPhysicMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshCube(iTexture,Pos, Scale, name, bEnable), CLPhysicCube(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		if (MeshControlled)
			NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		else
			NewtonBodySetTransformCallback(pbody, PhysicMeshTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICCUBE;
	};
	~CLPhysicMeshCube() {
		int i = 0;
	};
	void Debug(void){ NewtonDebug(); };
};
class CLPhysicMeshSphere : public CLMeshSphere, public CLPhysicSphere
{
public:
	CLPhysicMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshSphere(iTexture, Pos, Scale, name, bEnable), CLPhysicSphere(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		if (MeshControlled)
			NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		else
			NewtonBodySetTransformCallback(pbody, PhysicMeshTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICSPHERE;
	};
	~CLPhysicMeshSphere(){};
	void Debug(void){ NewtonDebug(); };
};
class CLPhysicMeshCylinder : public CLMeshCylinder, public CLPhysicCylinder
{
public:
	CLPhysicMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshCylinder(iTexture, Pos, Scale, name, bEnable), CLPhysicCylinder(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		Mesh->RotateZ(-90);// because newton aligns primitive on the X axis and TV3D on the Y
		if (MeshControlled)
			NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		else
			NewtonBodySetTransformCallback(pbody, PhysicMeshTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICCYLINDER;
	};
	~CLPhysicMeshCylinder(){};
	void Debug(void){ NewtonDebug(); };
};
class CLPhysicMeshCone : public CLMeshCone, public CLPhysicCone
{
public:
	CLPhysicMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshCone(iTexture, Pos, Scale, name, bEnable), CLPhysicCone(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		Mesh->RotateZ(-90);// because newton aligns primitive on the X axis and TV3D on the Y
		if (MeshControlled)
			NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		else
			NewtonBodySetTransformCallback(pbody, PhysicMeshTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICCONE;
	};
	~CLPhysicMeshCone(){};
	void Debug(void){ NewtonDebug(); };
};
class CLPhysicMeshCustom : public CLMeshCustom, public CLPhysicCustom
{
public:
	CLPhysicMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshCustom(PathToMesh, iTexture, Pos, Scale, name, bEnable), CLPhysicCustom(this->Mesh, Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		if (MeshControlled) 
			NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		else 
			NewtonBodySetTransformCallback(pbody, PhysicMeshTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICCUSTOM;
	};
	~CLPhysicMeshCustom(){};
	void Debug(void){ NewtonDebug(); };
};
class CLPhysicXMesh : public CLMeshX, public CLPhysicCustom
{
public:
	CLPhysicXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshX(PathToMesh, Pos, Scale, name, bEnable), CLPhysicCustom(this->Mesh, Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		if (MeshControlled)
			NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		else
			NewtonBodySetTransformCallback(pbody, PhysicMeshTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICXMESH;
	};
	~CLPhysicXMesh(){};
	void Debug(void){ NewtonDebug(); };
};
class CLPhysicXStatic : public CLMeshX, public CLPhysicCustomStatic
{
public:
	CLPhysicXStatic(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
		CLMeshX(PathToMesh, Pos, Scale, name, bEnable), CLPhysicCustomStatic(this->Mesh, Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world) {
		
		NewtonBodySetTransformCallback(pbody, MeshControlledTransformationCallback);
		NewtonBodySetUserData(pbody, this->Mesh);
		TypeMesh = PHYSICXSTATIC;
	};
	~CLPhysicXStatic(){};
	void Debug(void){ NewtonDebug(); };
};

// FACTORY SECTION
// create all TV3D type of Mesh
class MeshFactory
{
public:
	MeshFactory(){};
	~MeshFactory();
	
	vector<CLMesh*> vecMesh; 
	//vector<NewtonBody*> vecBody;
	//vector<CLPhysicEntity*> vecBody;
	
	void CreateCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateFloor(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateBillboard(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	void CreateDuplicate(char* SourceName, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable);
	CLMesh* Find3DObjectWithTVMesh(CTVMesh* tmpmesh);
	CLMesh* Find3DObjectWithName(char* name);
	
	void CreatePhysicMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
	void CreatePhysicMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
	void CreatePhysicMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
	void CreatePhysicMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
	void CreatePhysicMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
	void CreatePhysicXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
	void CreatePhysicXStatic(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);


	void NewtonDebug(void);

	// Fonctions de création et destruction du singleton
	static MeshFactory *getInstance()
	{
		if (NULL == _singleton)
		{
			// std::cout << "creating singleton." << std::endl;
			_singleton = new MeshFactory();
		}
		else
		{
			// std::cout << "singleton already created!" << std::endl;
		}

		return _singleton;
	}
	static void kill()
	{
		if (NULL != _singleton)
		{
			delete _singleton;
			_singleton = NULL;
		}
	}
private:
	static MeshFactory *_singleton;
};
#endif