#ifndef __3DObjMgr_H__
#define __3DObjMgr_H__
#pragma once
#include "3DObject.h"

typedef std::vector<CL3DObject*> vec3DObjects;
typedef std::vector<CTVMesh*> vecXMesh;

class CL3DObjMgr
{ 
	friend void NewtonBodyIteratorCallback(const NewtonBody *_body, void* userData);   // Sert a koi???
public:
	CL3DObjMgr(){};
	~CL3DObjMgr();

	void Render();
	CL3DObject* Find3DObjectWithMesh(CTVMesh*tmpmesh);
	vec3DObjects VecObjects;
	vecXMesh VecXMesh;


	// Fonctions de création et destruction du singleton
	static CL3DObjMgr *getInstance()
	{
		if (NULL == _singleton)
		{
			// std::cout << "creating singleton." << std::endl;
			_singleton = new CL3DObjMgr();
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
	static CL3DObjMgr *_singleton;
};
#endif
	

