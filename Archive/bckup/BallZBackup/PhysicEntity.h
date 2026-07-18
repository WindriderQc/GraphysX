#ifndef __PhysicEntity_H__
#define __PhysicEntity_H__
#pragma once
#include "newton.h"
#include "dMatrix.h"
#include "TV3DMoteur.h"  // Pour CLPhysicCustom (CTVMesh)  et DebugShowGeometryCollision (Screen2DImmediate)



void DebugShowGeometryCollision(void* userData, int vertexCount, const dFloat* faceVertec, int id);

class CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicEntity(dMatrix Matrix, float masse, dVector Scale, int newtonMat, NewtonWorld* world, NewtonCollision* coli = NULL);
	virtual ~CLPhysicEntity();

	GRAPHYSX_API NewtonCollision* BuildPhysicMesh(CTVMesh* Mesh, NewtonWorld* World);
	GRAPHYSX_API NewtonCollision* BuildPhysicStaticBody(CTVMesh* Mesh, NewtonWorld* World);
	GRAPHYSX_API void NewtonDebug(void);
	GRAPHYSX_API void setForceNTorqueCallback(NewtonApplyForceAndTorque callback);
	GRAPHYSX_API void setTransformCallback(NewtonSetTransform callback);
	GRAPHYSX_API void setBodyUserData(void* const data);
	GRAPHYSX_API void updateMassMatrix();
	GRAPHYSX_API void getMatrix(float* matrix);
	GRAPHYSX_API cTV_3DVECTOR getVelocity();
	GRAPHYSX_API void addForce(cTV_3DVECTOR force);
	GRAPHYSX_API float getMasse() { return fMasse; };
	GRAPHYSX_API ePHYSMAT getPhysMat() { return physMat; };
	GRAPHYSX_API void setPhysMat(ePHYSMAT mat) { physMat = mat; };

	NewtonBody* pbody = NULL;
	NewtonCollision* Collision = NULL;
	float fMasse;
	int iNewtonMat;

	bool bMeshControlled = false;

	GRAPHYSX_API static void* getBodyUserData(const NewtonBody* body);

private:
	ePHYSMAT physMat;
};

class CLPhysicSphere : public CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicSphere(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateSphere(world, Scale.m_x, 0, NULL)) {};
	virtual ~CLPhysicSphere(){};
	//virtual void setTnFCallback();  // should be pure
};
class CLPhysicCube : public CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicCube(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateBox(world, Scale.m_x, Scale.m_y, Scale.m_z, 0, NULL)) {};
	virtual ~CLPhysicCube(){};
};
class CLPhysicCylinder : public CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicCylinder(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateCylinder(world, Scale.m_x, Scale.m_y, 0, NULL)) {};
	virtual ~CLPhysicCylinder(){};
};
class CLPhysicCone : public CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicCone(dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, NewtonCreateCone(world, Scale.m_x, Scale.m_y, 0, NULL)) {};
	virtual ~CLPhysicCone(){};
};
class CLPhysicCustom : public CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicCustom(CTVMesh* Mesh, dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, BuildPhysicMesh(Mesh, world)) {};
	virtual ~CLPhysicCustom(){};
};
class CLPhysicCustomStatic : public CLPhysicEntity
{
public:
	GRAPHYSX_API CLPhysicCustomStatic(CTVMesh* Mesh, dMatrix Matrix, float masse, dVector Scale, int iNewtonMat, NewtonWorld* world) :
		CLPhysicEntity(Matrix, masse, Scale, iNewtonMat, world, BuildPhysicStaticBody(Mesh, world)) {};
	virtual ~CLPhysicCustomStatic(){};
};
#endif
