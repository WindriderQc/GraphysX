#include "stdafx.h"
#include "MeshFactory.h"


namespace GraphysX
{
	void PhysicMeshTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{ // transformation callback (called after the body has been simulated)
		cTV_3DMATRIX MX;
		NewtonBodyGetMatrix(body, MX.m[0]);
		CLMesh* clMesh = (CLMesh*)NewtonBodyGetUserData(body);
		clMesh->Mesh->SetMatrix(&MX);
		//char* Name = ((CTVMesh*)NewtonBodyGetUserData(body))->GetMeshName();
	}
	void MeshControlledTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{
		//char* Name = ((CTVMesh*)NewtonBodyGetUserData(body))->GetMeshName();
		//if (!strcmp(Name, "Suzanne1.RotatorCube"))
		//{
		//	int i = 0;
		//}
		NewtonBodySetMatrix(body, ((CLMesh*)NewtonBodyGetUserData(body))->Mesh->GetMatrix().m[0]);
	}

	/*static void ElevatorForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
	{
	// Cette fonction est une fonction Callback. Elle sera appelée à chaque fois
	// qu'une modification aura lieu sur le corps.

	// On récupère en premier lieu la masse ainsi que l'inertie
	//float masse; // Contiendra la masse de l'objet pris en paramètre par la fonction
	cTV_3DVECTOR inertie; // Contiendra l'inertie du corps
	cTV_3DVECTOR force; // Spécifiera la force appliquée sur le corps

	//Pas de gravité pour que l'object reste stable.

	//NewtonBodyGetMassMatrix(body, &masse, &inertie.x, &inertie.y, &inertie.z);
	//force.x = 0.0f;
	//force.y = -masse * 9.81* SCALE; // 9.81 est l'attraction gravitationnelle de la Terre
	//force.z = 0.0f;
	//NewtonBodyAddForce(body, &force.x); // On ajoute la force au corps


	//CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	}
	static void ElevatorTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{
	cTV_3DMATRIX MX;
	NewtonBodyGetMatrix(body, MX.m[0]);
	//CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	//cl3DObject->Mesh->SetMatrix(MX);
	}*/
	/*static void FinishLineContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
	{
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	//CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body0);
	//if (!cl3DObject) {
	//	cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body1);
	//}

	//cl3DObject->bIsFinishline = true;
	//  ICI faire un call de l'Avion qui va chercher la balle.  Devrait etre une creation d'objet avion,
	//  qui sera détruit quand la balle arrive a destination

	}*/
	/*static void ElevatorContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
	{
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	//CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body0);
	//if (cl3DObject->Mesh->GetMeshName() != "BallZShell")
	//	cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body1);

	//cl3DObject->bAllowJumps = true;
	//cl3DObject->bIsOnElevator = true;
	}*/

	CLMeshFactory::CLMeshFactory()
	{}
	CLMeshFactory::~CLMeshFactory()
	{
		vecPhysEntity.clear();
		for each(CLMesh* Obj in vecMesh)
		{
			if (Obj) delete(Obj); Obj = NULL;
		}
		vecMesh.clear();
		for each(MeshController* ctrler in vecCtrl)
		{
			delete(ctrler); ctrler = NULL;
		}
		vecCtrl.clear();

		std::cout << "MeshFactory destructed.     - All CLMesh and MeshController deleted!!" << std::endl;
	}


	void CLMeshFactory::init(CLScene &clScene)
	{

		World = clScene.GetWorld();
		std::cout << "MeshFactory constructed." << std::endl;
	}
	void CLMeshFactory::ClearScene()
	{
		vecPhysEntity.clear();
		for each(CLMesh* Obj in vecMesh)
		{
			if (Obj) delete(Obj); Obj = NULL;
		}
		vecMesh.clear();
		for each(MeshController* ctrler in vecCtrl)
		{
			delete(ctrler); ctrler = NULL;
		}
		vecCtrl.clear();

		std::cout << "XML Scene Cleared" << std::endl;
	}
	int CLMeshFactory::CheckNewtonCollisionCollide(CLMesh* Obj1, CLMesh* Obj2) // return the number of Hits of the collision if so.
	{
		const int nContacts = 2;
		float contacts[3 * nContacts];
		float normals[3 * nContacts];
		float penetration[nContacts];
		long long attributeA[nContacts];
		long long attributeB[nContacts];
		int threadIndex = 0;
		dMatrix mat1, mat2;

		NewtonBodyGetMatrix(Obj1->clPhysicEntity->pbody, (float*)&mat1);
		NewtonBodyGetMatrix(Obj2->clPhysicEntity->pbody, (float*)&mat2);

		int nHits = NewtonCollisionCollide(World, nContacts,
			Obj1->clPhysicEntity->Collision, (float*)&mat1,
			Obj2->clPhysicEntity->Collision, (float*)&mat2,
			contacts, normals, penetration, attributeA, attributeB, threadIndex);
		return(nHits);
	}
	void CLMeshFactory::DestroyNewtonCollision(NewtonCollision* col)
	{
		NewtonDestroyCollision(col);
	}
	void CLMeshFactory::DestroyMesh(CLMesh* mesh)
	{

		vecMesh.erase((std::remove(vecMesh.begin(), vecMesh.end(), mesh)), vecMesh.end());
		if (mesh) delete(mesh); mesh = NULL;
		std::cout << "Mesh destroyed." << std::endl;
	}
	CLHingeJoint* CLMeshFactory::CreateHingeJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float Friction, bool LimitsEnabled, float LimitsMinAngle, float LimitsMaxAngle)
	{
		CLHingeJoint* clHingejoint = new CLHingeJoint(ParentBody, ChildBody, Friction, LimitsEnabled, LimitsMinAngle, LimitsMaxAngle);
		return(clHingejoint);
	}
	CL6DOFJoint* CLMeshFactory::Create6DOFJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, dVector LimitsMinAngle, dVector LimitsMaxAngle)
	{
		CL6DOFJoint* cl6DOFJoint = new CL6DOFJoint(ParentBody, ChildBody, LimitsMinAngle, LimitsMaxAngle);
		return(cl6DOFJoint);
	}
	CLSlidingJoint* CLMeshFactory::CreateSlidingJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit)
	{
		CLSlidingJoint* clSlidingJoint = new CLSlidingJoint(ParentBody, ChildBody, limitsMinDist, limitsMaxDist, MinAngularLimit, MaxAngularLimit);
		return(clSlidingJoint);
	}
	void CLMeshFactory::CreateBallZController(void)
	{
		CLMesh* BallZ;  BallZ = Find3DObjectWithName("BallZ");
		if (BallZ){
			vecCtrl.push_back(new MeshController(BallZ, true, this));
			BallZCtrler = vecCtrl.back();
			NewtonBodySetUserData(BallZ->clPhysicEntity->pbody, BallZCtrler); // sets the controller instead of the CTVMesh as the data
			std::cout << "  - BallZ Controller created." << std::endl;
		}
	}
	MeshController* CLMeshFactory::CreatePistonController(CLMesh* Piston)
	{
		if (Piston){
			vecCtrl.push_back(new MeshController(Piston, true, this));
			PistonCtrler = vecCtrl.back();
			NewtonBodySetUserData(Piston->clPhysicEntity->pbody, PistonCtrler);
			std::cout << "  - Piston Controller created." << std::endl;
			return PistonCtrler;
		}
		else {
			std::cout << "-!!!   Controller Obj not Valid !!!";
			return NULL;
		}
	}
	void CLMeshFactory::NewtonDebug()
	{
		for each(CLMesh* Obj in vecMesh)
		{
			Obj->Debug();
		}
	}
	void CLMeshFactory::UpdateCtrl(cTV_3DVECTOR Mouse3DPos)
	{
		for each(MeshController* ctrler in vecCtrl)
		{
			if (ctrler) ctrler->Update(Mouse3DPos);
		}
	}
	CLMesh* CLMeshFactory::Find3DObjectWithTVMesh(CTVMesh* tmpmesh)
	{
		for each (CLMesh *Obj in vecMesh){
			if (Obj->Mesh->iMeshIndex == tmpmesh->iMeshIndex){
				return(Obj);
				break;
			}
		}
		return(NULL);
	};
	CLMesh* CLMeshFactory::Find3DObjectWithName(char* name)
	{
		for each (CLMesh *Obj in vecMesh){
			if (!strcmp(name, Obj->Mesh->GetMeshName())){
				return(Obj);
				break;
			}
		}
		return(NULL);
	};
	CLMesh* CLMeshFactory::CreateSprite(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, int iTexture)
	{
		vecMesh.push_back(new CLMeshSprite(Pos, Rot, Scale, name, bEnable, iTexture));
		return(vecMesh.back());
	}
	CLMeshSprite::CLMeshSprite(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, int iTexture) : CLMesh(eObjet::VERTEXSTRIP, name, bEnable)
	{
		std::cout << "triangle create mesh";
		Mesh->SetMeshFormat(cTV_MESHFORMAT_SIMPLE | cTV_MESHFORMAT_DIFFUSE);
		Mesh->SetPrimitiveType(cTV_TRIANGLESTRIP);
		int iVert = Mesh->AddVertex(0, 0, Scale.z, 0, 0, -1, 0, 2, 0, 1, RGBA_RED);
		Mesh->AddVertex(0, Scale.y, Scale.z, 0, 0, -1, 0, 0, 0, 0, RGBA_AQUA);
		Mesh->AddVertex(Scale.x, 0, Scale.z, 0, 0, -1, 2, 2, 0, 0, RGBA_AQUA);
		Mesh->AddVertex(Scale.x, Scale.y, Scale.z, 0, 0, -1, 2, 0, 0, 0, RGBA_RED);
		Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
		Mesh->SetRotation(Rot.x, Rot.y, Rot.z);

		float tmp;
		int color = NULL;
		int iRed = RGBA_RED;
		Mesh->GetVertex(iVert, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp, &color);

		//*/
		Mesh->SetColor(RGBA_AQUA, true);
		//Mesh->SetLightingMode(cTV_LIGHTING_MANAGED); // cTV_LIGHTING_PERVERTEX_PRT //  cTV_LIGHTING_MANAGED

		if (iTexture)	Mesh->SetTexture(iTexture, -1);
		//Init(1);
	};
	CLMesh* CLMeshFactory::CreateCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshCube(iTexture, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateText3D(char* text, char* fontname, float fontsize, int color, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, char* name, bool bEnable, float extrude)
	{
		vecMesh.push_back(new CLMeshText3D(text, fontname, fontsize, color, Pos, Rot, name, bEnable, extrude));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshSphere(iTexture, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshCylinder(iTexture, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshCone(iTexture, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateFloor(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, float tileW, float tileH, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshFloor(iTexture, Pos, Rot, Scale, tileW, tileH, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateBillboard(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshBillboard(iTexture, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshCustom(PathToMesh, iTexture, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		vecMesh.push_back(new CLMeshX(PathToMesh, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateXMesh(char* PathToMesh, char* name, bool bEnable)
	{
		cTV_3DVECTOR Pos(0, 0, 0);
		cTV_3DVECTOR Rot(0, 0, 0);
		cTV_3DVECTOR Scale(1, 1, 1);
		vecMesh.push_back(new CLMeshX(PathToMesh, Pos, Rot, Scale, name, bEnable));
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreateDuplicate(char* SourceName, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable)
	{
		bool bFound = false;
		for each (CLMesh *Obj in vecMesh) {
			if (!strcmp(Obj->Mesh->GetMeshName(), SourceName)){
				vecMesh.push_back(new CLDuplicate(Obj, Pos, Rot, Scale, name, bEnable));
				bFound = true;
				return(vecMesh.back());
				break;
			}
		}
		if (!bFound) MSGBOX("The Mesh name for Duplicate hasn't been found...", name);
		//MessageBoxA(NULL, , "Warning", MB_ICONEXCLAMATION | MB_SETFOREGROUND);
		return(NULL);
	}

	CLMesh* CLMeshFactory::CreatePhysicMeshCube(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicMeshCube(iTexture, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreatePhysicMeshSphere(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicMeshSphere(iTexture, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreatePhysicMeshCylinder(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicMeshCylinder(iTexture, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreatePhysicMeshCone(int iTexture, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicMeshCone(iTexture, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreatePhysicMeshCustom(char* PathToMesh, int iTexture, cTV_3DVECTOR Rot, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicMeshCustom(PathToMesh, iTexture, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreatePhysicXMesh(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicXMesh(PathToMesh, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}
	CLMesh* CLMeshFactory::CreatePhysicXStatic(char* PathToMesh, cTV_3DVECTOR Pos, cTV_3DVECTOR Rot, cTV_3DVECTOR Scale, char* name, bool bEnable, float masse, int iNewtonMat, NewtonWorld* world, bool MeshControlled)
	{
		vecMesh.push_back(new CLPhysicXStatic(PathToMesh, Pos, Rot, Scale, name, bEnable, masse, iNewtonMat, world, MeshControlled));
		vecPhysEntity.push_back((CLPhysicEntity*)vecMesh.back());
		return(vecMesh.back());
	}

}