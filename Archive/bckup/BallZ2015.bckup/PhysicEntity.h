#ifndef __PhysicEntity_H__
#define __PhysicEntity_H__
#pragma once
#include "newton.h"
#include "dMath\dMatrix.h"
#include "TV3DMoteur.h"  // Pour CLPhysicCustom (CTVMesh)

void DebugShowGeometryCollision(void* userData, int vertexCount, const dFloat* faceVertec, int id);

class CLPhysicEntity
{
public:
	CLPhysicEntity(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world, NewtonCollision* coli = NULL);
	virtual ~CLPhysicEntity();

	NewtonCollision* BuildPhysicMesh(CTVMesh* Mesh, NewtonWorld* World);
	NewtonCollision* BuildPhysicStaticBody(CTVMesh* Mesh, NewtonWorld* World);
	void NewtonDebug(void);

	NewtonBody* pbody = NULL;
	NewtonCollision* Collision = NULL;
	float fMasse;
};

class CLPhysicSphere : public CLPhysicEntity
{
public:
	CLPhysicSphere(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateSphere(world, Scale.m_x, 0, NULL)) {};
	virtual ~CLPhysicSphere(){};
};
class CLPhysicCube : public CLPhysicEntity
{
public:
	CLPhysicCube(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) : 
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateBox(world, Scale.m_x, Scale.m_y, Scale.m_z, 0, NULL)) {};
	virtual ~CLPhysicCube(){};
};
class CLPhysicCylinder : public CLPhysicEntity
{
public:
	CLPhysicCylinder(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateCylinder(world, Scale.m_x, Scale.m_y, 0, NULL)) {};
	virtual ~CLPhysicCylinder(){};
};
class CLPhysicCone : public CLPhysicEntity
{
public:
	CLPhysicCone(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateCone(world, Scale.m_x, Scale.m_y, 0, NULL)) {};
	virtual ~CLPhysicCone(){};
};
class CLPhysicCustom : public CLPhysicEntity
{
public:
	CLPhysicCustom(CTVMesh* Mesh, dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) : 
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, BuildPhysicMesh(Mesh, world)) {};
	virtual ~CLPhysicCustom(){};
};
class CLPhysicCustomStatic : public CLPhysicEntity
{
public:
	CLPhysicCustomStatic(CTVMesh* Mesh, dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, BuildPhysicStaticBody(Mesh, world)) {};
	virtual ~CLPhysicCustomStatic(){};
};
#endif
