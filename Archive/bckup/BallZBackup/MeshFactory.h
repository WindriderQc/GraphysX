#pragma once
#include "Mesh.h"
#include "Scene.h"
#include <algorithm> // vector.erase
#include "MeshController.h"
#include "Joint.h"

namespace GraphysX
{
	void PhysicMeshTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);
	void MeshControlledTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);

	class CLMeshSprite : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshSprite(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, int iTexture = NULL);
		virtual ~CLMeshSprite(){};
		void Debug(void){ };
	};
	class CLMeshSphere : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::SPHERE, name, bEnable) {
			Mesh->CreateSphere(Scale.x, 24, 24);
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			if (iTexture)	Mesh->SetTexture(iTexture);
			Init(1);
		};
		virtual ~CLMeshSphere() {};
		void Debug(void){ };
	};
	class CLMeshCube : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshCube(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, bool bEnable) : CLMesh(eObjet::CUBE, "Cube", bEnable)
		{
			int i = Mesh->CreateBox(Scale.x, Scale.y, Scale.z);
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			Init(1);
		};
		GRAPHYSX_API CLMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::CUBE, name, bEnable)
		{
			Mesh->CreateBox(Scale.x, Scale.y, Scale.z);
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			if (iTexture)	Mesh->SetTexture(iTexture, -1);
			Init(1);
		};
		virtual ~CLMeshCube(){};
		void Debug(void){ };
	};
	class CLMeshText3D : public CLMesh
	{
	public: GRAPHYSX_API CLMeshText3D(char* text, char* fontname, float fontsize, int color, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, char* name, bool bEnable, float extrude = 0.2f) : CLMesh(eObjet::TEXT, name, bEnable) {

		Mesh->Create3DText(text, fontname, fontsize, extrude);
		Mesh->SetColor(color);
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
		//if (iTexture)	Mesh->SetTexture(iTexture, -1);
		Init(1);
	};
			virtual ~CLMeshText3D() {};
			void Debug(void){ };
	};
	class CLMeshCylinder : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::CYLINDER, name, bEnable) {
			Mesh->CreateCylinder(Scale.x, Scale.y, 12, true);  // radius, height, Model precision = 12, capped = true.  A mettre en param..
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			if (iTexture)	Mesh->SetTexture(iTexture, -1);
			Init(1);
		};
		virtual ~CLMeshCylinder() {};
		void Debug(void){ };
	};
	class CLMeshCone : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::CONE, name, bEnable) {
			Mesh->CreateCone(Scale.x, Scale.y, 12, true);  // radius, height, Model precision = 12, capped = true.  A mettre en param..
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			if (iTexture)	Mesh->SetTexture(iTexture, -1);
			Init(1);
		};
		virtual ~CLMeshCone() {};
		void Debug(void){ };
	};
	class CLMeshFloor : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshFloor(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, float tileW, float tileH, char* name, bool bEnable) : CLMesh(eObjet::FLOOR, name, bEnable) {
			Mesh->AddFloor(iTexture, -(Scale.x) + Pos.x, -(Scale.z) + Pos.z, (Scale.x) + Pos.x, (Scale.z) + Pos.z, Pos.y, tileW, tileH);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			Mesh->Enable(bEnable);
			Init(1);
		};
		~CLMeshFloor() {};
		void Debug(void){ };
	};
	class CLMeshBillboard : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshBillboard(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::BILLBOARD, name, bEnable) {
			Mesh = GraphysX::CLTV3D::pTVScene.CreateBillboard(iTexture, Pos.x, Pos.y, Pos.z, Scale.x, Scale.y, name, false);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			Init(1);
		};
		~CLMeshBillboard() {};
		void Debug(void){ };
	};
	class CLMeshCustom : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::CUSTOM, name, bEnable) {

			bool bResult = Mesh->LoadTVM(PathToMesh, true, true);
			if (Mesh->LoadTVM(PathToMesh, true, true)) {
				Mesh->SetScale(Scale.x, Scale.y, Scale.z); //fait crasher un bout de la physics
				Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
				Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
				cTV_3DMATRIX after = Mesh->GetMatrix();
				if (iTexture)	Mesh->SetTexture(iTexture, -1);
				Init(1);
			}
			else
				MSGBOX("The 3DObject TVM model hasn't been found...", "Warning");
		};
		virtual ~CLMeshCustom() {};
		void Debug(void){ };
	};
	class CLMeshX : public CLMesh
	{
	public:
		GRAPHYSX_API CLMeshX(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::XMESH, name, bEnable) {

			bool bResult = Mesh->LoadXFile(PathToMesh);
			if (bResult == false)
				MSGBOX("The X Mesh model " << PathToMesh << " hasn't been found...", "Warning");
			//MessageBoxA(NULL, "The X Mesh model hasn't been found...", "Warning", MB_ICONEXCLAMATION | MB_SETFOREGROUND);
			Mesh->SetAlphaTest(true);
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			Mesh->SetScale(Scale.x, Scale.y, Scale.z);  //fait crasher un bout de la physics   ?? still valid?

			Init(1);
		};
		virtual ~CLMeshX() {};
		void Debug(void){ };
	};
	class CLDuplicate : public CLMesh
	{
	public:
		GRAPHYSX_API CLDuplicate(CLMesh* Obj, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable) : CLMesh(eObjet::DUPLICATEMESH, name, bEnable) {

			Mesh = Obj->Mesh->Duplicate(name);
			Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
			Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
			//Mesh->SetScale(Scale.x, Scale.y, Scale.z); Not implemented...   le duplicate est par défaut en shared Memory...  
			//voir si on peux scaler un element en mettant shared Mem a false dans le duplicate.
		};
		virtual ~CLDuplicate() {};
		void Debug(void){ };
	};

	class CLPhysicMeshCube : public CLMeshCube
	{
	public:
		CLPhysicMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) : CLMeshCube(iTexture, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicCube(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);

			if (MeshControlled) {
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
				clPhysicEntity->bMeshControlled = true;
			}
			else
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, PhysicMeshTransformationCallback);
			NewtonBodySetUserData(clPhysicEntity->pbody, this);
			TypeMesh = eObjet::PHYSICCUBE;
		};
		virtual ~CLPhysicMeshCube() { int i = 0; };
		void Debug(void){ clPhysicEntity->NewtonDebug(); };
	};
	class CLPhysicMeshSphere : public CLMeshSphere
	{
	public:
		CLPhysicMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) : CLMeshSphere(iTexture, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicSphere(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);
			if (MeshControlled) {
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
				clPhysicEntity->bMeshControlled = true;
			}
			else
				clPhysicEntity->setTransformCallback(PhysicMeshTransformationCallback);
			clPhysicEntity->setBodyUserData(this);
			TypeMesh = eObjet::PHYSICSPHERE;
		};
		virtual ~CLPhysicMeshSphere(){};
		void Debug(void) { clPhysicEntity->NewtonDebug(); };
		//virtual void setTnFCallback() = 0;
	};
	class CLPhysicMeshCylinder : public CLMeshCylinder
	{
	public:
		CLPhysicMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
			CLMeshCylinder(iTexture, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicCylinder(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);
			Mesh->RotateZ(-90);// because newton aligns primitive on the X axis and TV3D on the Y
			if (MeshControlled) {
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
				clPhysicEntity->bMeshControlled = true;
			}
			else
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, PhysicMeshTransformationCallback);
			NewtonBodySetUserData(clPhysicEntity->pbody, this);
			TypeMesh = eObjet::PHYSICCYLINDER;
		};
		virtual ~CLPhysicMeshCylinder(){};
		void Debug(void){ clPhysicEntity->NewtonDebug(); };
	};
	class CLPhysicMeshCone : public CLMeshCone
	{
	public:
		CLPhysicMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
			CLMeshCone(iTexture, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicCone(Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);
			Mesh->RotateZ(-90);// because newton aligns primitive on the X axis and TV3D on the Y
			if (MeshControlled) {
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
				clPhysicEntity->bMeshControlled = true;
			}
			else
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, PhysicMeshTransformationCallback);
			NewtonBodySetUserData(clPhysicEntity->pbody, this);
			TypeMesh = eObjet::PHYSICCONE;
		};
		virtual ~CLPhysicMeshCone(){};
		void Debug(void){ clPhysicEntity->NewtonDebug(); };
	};
	class CLPhysicMeshCustom : public CLMeshCustom
	{
	public:
		CLPhysicMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
			CLMeshCustom(PathToMesh, iTexture, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicCustom(Mesh, Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);
			if (MeshControlled) {
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
				clPhysicEntity->bMeshControlled = true;
			}
			else
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, PhysicMeshTransformationCallback);
			NewtonBodySetUserData(clPhysicEntity->pbody, this);
			TypeMesh = eObjet::PHYSICCUSTOM;
		};
		virtual ~CLPhysicMeshCustom(){};
		void Debug(void){ clPhysicEntity->NewtonDebug(); };
	};
	class CLPhysicXMesh : public CLMeshX
	{
	public:
		CLPhysicXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
			CLMeshX(PathToMesh, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicCustom(Mesh, Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);
			if (MeshControlled) {
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
				clPhysicEntity->bMeshControlled = true;
			}
			else
				NewtonBodySetTransformCallback(clPhysicEntity->pbody, PhysicMeshTransformationCallback);
			NewtonBodySetUserData(clPhysicEntity->pbody, this);
			TypeMesh = eObjet::PHYSICXMESH;
		};
		virtual ~CLPhysicXMesh(){};
		void Debug(void){ clPhysicEntity->NewtonDebug(); };
	};
	class CLPhysicXStatic : public CLMeshX
	{
	public:
		CLPhysicXStatic(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled) :
			CLMeshX(PathToMesh, Pos, Rot, Scale, name, bEnable)
		{
			clPhysicEntity = new CLPhysicCustomStatic(Mesh, Mesh->GetMatrix().m[0], masse, dVector(Scale.x, Scale.y, Scale.z), iNewtonMat, world);
			NewtonBodySetTransformCallback(clPhysicEntity->pbody, MeshControlledTransformationCallback);
			NewtonBodySetUserData(clPhysicEntity->pbody, this);
			TypeMesh = eObjet::PHYSICXSTATIC;
		};
		virtual ~CLPhysicXStatic(){};
		void Debug(void){ clPhysicEntity->NewtonDebug(); };
	};

	// FACTORY SECTION
	// create all TV3D type of Mesh

	class CLMeshFactory
	{
	public:
		GRAPHYSX_API CLMeshFactory();
		GRAPHYSX_API ~CLMeshFactory();
		GRAPHYSX_API void init(CLScene &clScene);

		GRAPHYSX_API CLMesh* Find3DObjectWithTVMesh(CTVMesh* tmpmesh);
		GRAPHYSX_API CLMesh* Find3DObjectWithName(char* name);
		GRAPHYSX_API void NewtonDebug(void);
		GRAPHYSX_API void ClearScene(void);
		GRAPHYSX_API int CheckNewtonCollisionCollide(CLMesh* Obj1, CLMesh* Obj2); // return the number of Hits of the collision if so.
		GRAPHYSX_API void DestroyNewtonCollision(NewtonCollision* col);
		GRAPHYSX_API void DestroyMesh(CLMesh* mesh);
		GRAPHYSX_API CLHingeJoint* CreateHingeJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float Friction, bool LimitsEnabled, float LimitsMinAngle, float LimitsMaxAngle);
		GRAPHYSX_API CL6DOFJoint* Create6DOFJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, dVector LimitsMinAngle, dVector LimitsMaxAngle);
		GRAPHYSX_API CLSlidingJoint* CreateSlidingJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit);


		GRAPHYSX_API void UpdateCtrl(cTV_3DVECTOR Mouse3DPos);
		MeshController* BallZCtrler;
		MeshController* PistonCtrler;
		GRAPHYSX_API void CreateBallZController();
		GRAPHYSX_API MeshController* CreatePistonController(CLMesh* Piston);


		std::vector<CLMesh*> vecMesh;
		std::vector<CLPhysicEntity*> vecPhysEntity;
		std::vector<MeshController*> vecCtrl;

		GRAPHYSX_API CLMesh* CreateSprite(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, int iTexture = NULL);
		GRAPHYSX_API CLMesh* CreateText3D(char* text, char* fontname, float fontsize, int color, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, char* name, bool bEnable, float extrude = 0.2f);
		GRAPHYSX_API CLMesh* CreateCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateFloor(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, float tileW, float tileH, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateBillboard(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateXMesh(char* PathToMesh, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);
		GRAPHYSX_API CLMesh* CreateDuplicate(char* SourceName, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable);

		GRAPHYSX_API CLMesh* CreatePhysicMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
		GRAPHYSX_API CLMesh* CreatePhysicMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
		GRAPHYSX_API CLMesh* CreatePhysicMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
		GRAPHYSX_API CLMesh* CreatePhysicMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
		GRAPHYSX_API CLMesh* CreatePhysicMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
		GRAPHYSX_API CLMesh* CreatePhysicXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);
		GRAPHYSX_API CLMesh* CreatePhysicXStatic(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled);

	private:
		NewtonWorld* World;
	};
}