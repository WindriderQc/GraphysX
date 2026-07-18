#pragma once
#include "GraphysXdef.h"
#include "MeshPrimitive.h"
#include "newton.h"
#include "dMatrix.h"

namespace GraphysX
{

	class CLPhysGeom : public CLMeshPrimitive
	{
	public:
		CLPhysGeom(NewtonWorld* world, STMeshParam param, ePHYSMAT physmat = DEF_PHYSMAT, float masse = 0.0f);
		virtual ~CLPhysGeom();

		virtual void ForceNTorque() = 0;
		virtual void Transform() = 0;

		

		// setters & getters
		//static std::vector<CLPhysGeom*>* getItemVec() { return &vecItem; };
		
		GRAPHYSX_API virtual void setMass(float mass);
		GRAPHYSX_API cTV_3DVECTOR getVelocity();
		GRAPHYSX_API NewtonBody* getBody() { return pbody; }
		GRAPHYSX_API float getMass() { return fMasse; }
		GRAPHYSX_API cTV_3DVECTOR getPos();
		GRAPHYSX_API ePHYSMAT getPhysmat() { return physMat; }
		GRAPHYSX_API void setPhysmat(ePHYSMAT physMat);
		GRAPHYSX_API virtual void matrixTVmeshToBody();
		 
		virtual cTV_3DMATRIX* matrix_bodyToTV();
		virtual dMatrix matrix_TVMeshToNewton();
		virtual void NewtonDebug();

		virtual void reset();


	protected:
		NewtonBody* pbody = NULL;
		
		float fMasse;

		NewtonWorld* m_World;
		virtual void addGravity();
		virtual void addForce(cTV_3DVECTOR force);

		virtual void matrix_setMass();
		
		virtual NewtonCollision* loadCollision();
		virtual NewtonBody* loadBody();
		virtual NewtonCollision* BuildCustomBody(CTVMesh* Mesh, bool isStatic, NewtonWorld* World);
		ePHYSMAT physMat;

		
		static int getNbrPhysGeom();
	private:
		//static std::vector<CLPhysGeom*> vecItem;
		static int nbrPhysGeom;
		NewtonCollision* Collision = NULL;

		cTV_3DMATRIX MX;
		
	};
}