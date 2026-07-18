#include "StdAfx.h"
#include "3DObjMgr.h"

//Initialisation du singleton à NULL
CL3DObjMgr *CL3DObjMgr::_singleton = NULL;


void NewtonBodyIteratorCallback(const NewtonBody *_body, void* userData)  // Sert a koi???
{
	NewtonBodySetSleepState(_body, 0);
}


CL3DObjMgr::~CL3DObjMgr() 
{
	for each (CL3DObject *Obj in VecObjects){ 
		delete(Obj); Obj = NULL; 
	} 
	for each (CTVMesh *Obj in VecXMesh){
		delete(Obj); Obj = NULL;
	}
};
void CL3DObjMgr::Render()
{
	for each (CL3DObject *Obj in VecObjects){ 
		Obj->Render(); 
	} 
	for each (CTVMesh *Obj in VecXMesh){
		Obj->Render();
	}

};
CL3DObject* CL3DObjMgr::Find3DObjectWithMesh(CTVMesh* tmpmesh)
{
	bool bFound = NULL;
	for each (CL3DObject *Obj in VecObjects){
		if (Obj->Mesh->iMeshIndex == tmpmesh->iMeshIndex){
			return(Obj);
			break;
		}
	}
	return(NULL);
};