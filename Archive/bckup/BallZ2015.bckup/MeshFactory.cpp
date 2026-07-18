#include "stdafx.h"
#include "MeshFactory.h"

//Initialisation du singleton à NULL
MeshFactory *MeshFactory::_singleton = NULL;

void PhysicMeshTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{ // transformation callback (called after the body has been simulated)
	cTV_3DMATRIX* MX = new cTV_3DMATRIX();
	NewtonBodyGetMatrix(body, MX->m[0]);
	((CTVMesh*)NewtonBodyGetUserData(body))->SetMatrix(MX);
	char* Name = ((CTVMesh*)NewtonBodyGetUserData(body))->GetMeshName();
	delete(MX); MX = NULL;
}

void MeshControlledTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	char* Name = ((CTVMesh*)NewtonBodyGetUserData(body))->GetMeshName();
	if (!strcmp(Name, "Airplane"))
	{
		int i = 0;
	}
	NewtonBodySetMatrix(body, ((CTVMesh*)NewtonBodyGetUserData(body))->GetMatrix().m[0]);
}



// CLMESH SECTION
CLMesh::CLMesh(eObjet type, char* name, bool bEnable) : TypeMesh(type)
{
	//  Create and set Mesh
	Mesh = CLTV3DMoteur::getInstance()->pTVScene->CreateMeshBuilder(name);
	Mesh->Enable(bEnable);
}
CLMesh::~CLMesh(void)
{
	//if (Mesh) { CLTV3DMoteur::getInstance()->pTVScene->DestroyMesh(Mesh);  Mesh = NULL; }
	if (Mesh) { Mesh->Destroy();  Mesh = NULL; }
}
void CLMesh::Init(void)
{
	//Mesh->SetBlendingMode(cTV_BLEND_ALPHA);
	Mesh->SetShadowCast(true, true);
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	if (!Mesh->GetMaterial(0)) // Applique le Material par défaut si aucun présent dans le mesh loadé
	Mesh->SetMaterial(CLTV3DMoteur::getInstance()->iStdMaterial, -1);
}
void CLMesh::SetPosNRot(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot)
{
	Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
	Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
}
CLMesh* MeshFactory::Find3DObjectWithTVMesh(CTVMesh* tmpmesh)
{
	for each (CLMesh *Obj in vecMesh){
		if (Obj->Mesh->iMeshIndex == tmpmesh->iMeshIndex){
			return(Obj);
			break;
		}
	}
	return(NULL);
};
CLMesh* MeshFactory::Find3DObjectWithName(char* name)
{
	for each (CLMesh *Obj in vecMesh){
		if (strcmp(name, Obj->Mesh->GetMeshName()) == 0){
			return(Obj);
			break;
		}
	}
	return(NULL);
};

MeshFactory::~MeshFactory()
{
	for each (CLMesh *Obj in vecMesh) {
		delete(Obj); Obj = NULL;
	}
}

void MeshFactory::CreateCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshCube(iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshSphere(iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshCylinder(iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshCone(iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateFloor(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshFloor(iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateBillboard(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshBillboard(iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshCustom(PathToMesh, iTexture, Pos, Scale, name, bEnable));
}
void MeshFactory::CreateXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	this->vecMesh.push_back(new CLMeshX(PathToMesh, Pos, Scale, name, bEnable));
}
void  MeshFactory::CreateDuplicate(char* SourceName, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable)
{
	bool bFound = false;
	for each (CLMesh *Obj in vecMesh) {
		if (!strcmp(Obj->Mesh->GetMeshName(), SourceName)){
			this->vecMesh.push_back(new CLDuplicate(Obj, Pos, Scale, name, bEnable));
			bFound = true;
			break;
		}
	}

	if(!bFound) MSGBOX("The Mesh name for Duplicate hasn't been found...", name);
	//MessageBoxA(NULL, , "Warning", MB_ICONEXCLAMATION | MB_SETFOREGROUND);
	
}

void MeshFactory::CreatePhysicMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	this->vecMesh.push_back(new CLPhysicMeshCube(iTexture, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}
void MeshFactory::CreatePhysicMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	this->vecMesh.push_back(new CLPhysicMeshSphere(iTexture, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}
void MeshFactory::CreatePhysicMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	this->vecMesh.push_back(new CLPhysicMeshCylinder(iTexture, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}
void MeshFactory::CreatePhysicMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	this->vecMesh.push_back(new CLPhysicMeshCone(iTexture, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}
void MeshFactory::CreatePhysicMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	vecMesh.push_back(new CLPhysicMeshCustom(PathToMesh, iTexture, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}
void MeshFactory::CreatePhysicXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	this->vecMesh.push_back(new CLPhysicXMesh(PathToMesh, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}
void MeshFactory::CreatePhysicXStatic(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
{
	this->vecMesh.push_back(new CLPhysicXStatic(PathToMesh, Pos, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
}


void MeshFactory::NewtonDebug()
{
	for each(CLMesh* Obj in vecMesh)
	{
		Obj->Debug();
	}
}