#pragma once
#include "MeshPrimitive.h"
#include <Newton.h>
#include <dMatrix.h>

namespace GraphysX
{

	class CLPhysGeom : public CLMeshPrimitive
	{
	public:
		CLPhysGeom(NewtonWorld* world, STEntityParam param);
		virtual ~CLPhysGeom();

		virtual void ForceNTorque() = 0;
		virtual void Transform() = 0;
		
		virtual void setMass(float mass);
		cTV_3DVECTOR getVelocity();
		NewtonBody* getBody() { return pbody; }
		float getMass() { return stParam.masse; }
		cTV_3DVECTOR getPos();
		ePHYSMAT getPhysmat() { return stParam.physMat; }
		void setPhysmat(ePHYSMAT physMat);
		virtual void matrixTVmeshToBody();
		 
		virtual cTV_3DMATRIX* matrix_bodyToTV();
		virtual dMatrix matrix_TVMeshToNewton();
		virtual void NewtonDebug();

		virtual void reset();


	protected:
		NewtonBody* pbody = NULL;
		
		NewtonWorld* m_World;
		virtual void addGravity();
		virtual void addForce(cTV_3DVECTOR force);

		virtual void matrix_setMass();
		
		virtual NewtonCollision* loadCollision();
		virtual NewtonBody* loadBody();
		virtual NewtonCollision* BuildCustomBody(CTVMesh* Mesh, bool isStatic, NewtonWorld* World);
			
		static int getNbrPhysGeom();
	private:
		//static std::vector<CLPhysGeom*> vecItem;
		static int nbrPhysGeom;
		NewtonCollision* Collision = NULL;

		cTV_3DMATRIX MX;
		
	};
}