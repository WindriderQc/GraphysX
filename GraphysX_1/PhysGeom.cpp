#include "stdafx.h"
#include "PhysGeom.h"
#include "ResourceManager.h"
#include "TV3DMoteur.h"


namespace GraphysX
{
	int CLPhysGeom::nbrPhysGeom = 0;

	void ForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
	{
		CLPhysGeom* entity = (CLPhysGeom*)NewtonBodyGetUserData(body);
		if (entity){
			entity->ForceNTorque();
		}


	}
	void TransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{
		// transformation callback (called after the body has been simulated)
		CLPhysGeom* entity = (CLPhysGeom*)NewtonBodyGetUserData(body);
		if (entity){
			entity->Transform();
		}
	}

	CLPhysGeom::CLPhysGeom(NewtonWorld* world, STEntityParam stParam) : CLMeshPrimitive(stParam), m_World(world)
	{
		pbody = loadBody();

		setPhysmat(stParam.physMat);
		
		if (stParam.masse != 0) // static object
		{
			matrix_setMass();
			// On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
			NewtonBodySetForceAndTorqueCallback(pbody, ForceAndTorqueCallback);
			NewtonBodySetTransformCallback(pbody, TransformCallback);
		}
		NewtonBodySetUserData(pbody, this);

		if (Collision) NewtonDestroyCollision(Collision);  Collision = NULL;  // Destroy les collision qui ne sont plus utilisées
		//vecItem.push_back(this);
		nbrPhysGeom++;

		/*
		For realistic physics these values need to be set sensibly. Here are some simple formulae for uniform density shapes:
		Solid Sphere
		Ixx = Iyy = Izz = \frac{2*radius^2*mass}{5}

		Solid Box
		Ixx = mass * (ly*ly + lz*lz) / 12
		Iyy = mass * (lx*lx + lz*lz) / 12
		Izz = mass * (lx*lx + ly*ly) / 12
		Solid Cylinder
		Ixx = Iyy = mass * ( \frac{radius^2}{4} + \frac{1}{12}) * height^2 Izz = mass * radius^2 / 2

		To make an object unmovable (static, infinite mass) either set the mass to below 0.001f (it is best to set it to 0.0f) or
		do not call the function (as the object will default to static). When static, an object's force and position callbacks are not called.
		*/
	}
	CLPhysGeom::~CLPhysGeom()
	{
		if (pbody)	NewtonDestroyBody(pbody);   pbody = NULL;
		//vecItem.erase((std::remove(vecItem.begin(), vecItem.end(), this)), vecItem.end());
		nbrPhysGeom--;
	}
	int CLPhysGeom::getNbrPhysGeom()
	{
		return nbrPhysGeom;
	}
	NewtonBody* CLPhysGeom::loadBody()
	{
		dMatrix mat = matrix_TVMeshToNewton();
		NewtonBody* body =  NewtonCreateDynamicBody(m_World, loadCollision(), &mat[0][0]); // crash si on scale le mesh avant sans la patch

		if ((stParam.geom) == eGeometry::PLANE)  // Le centre du floor tv3D est dans le coin bas alors que le newton cube est au centre. serait plus clean avec le newton Heightmap collision
		{
			cTV_3DMATRIX mat;
			CLTV3D::pTVMaths.TVMatrixIdentity(&mat);
			mat.m41 = stParam.pos.x; // x  // on ne prends plus le scale en compte
			mat.m42 = stParam.pos.y-stParam.scale.y/2; // y
			mat.m43 = stParam.pos.z; // z
			NewtonBodySetMatrix(body, mat.m[0]);
		}
		return body;
	}
	void CLPhysGeom::setPhysmat(ePHYSMAT physMat)
	{
		stParam.physMat = physMat;
		NewtonBodySetMaterialGroupID(pbody, ResourceManager::getPhysmat(physMat, m_World));
	}
	NewtonCollision* CLPhysGeom::loadCollision()
	{
		if (stParam.scale.y > 1)
			int i = 1;
		switch (stParam.geom)
		{
		case eGeometry::CUBE:
			Collision = NewtonCreateBox(m_World, stParam.scale.x, stParam.scale.y, stParam.scale.z, 0, NULL);
			break;
		case eGeometry::SPHERE:
			Collision = NewtonCreateSphere(m_World, stParam.scale.x, 0, NULL);
			break;
		case eGeometry::CYLINDER:
			Collision = NewtonCreateCylinder(m_World, stParam.scale.x, stParam.scale.y, 0, NULL);
			break;
		case eGeometry::CONE:
			Collision = NewtonCreateCone(m_World, stParam.scale.x, stParam.scale.y, 0, NULL);
			break;
		case eGeometry::PLANE:
			Collision = NewtonCreateBox(m_World, stParam.scale.x*2, stParam.scale.y, stParam.scale.z*2, 0, NULL);  // *2 test->   pas sur   TODO
			break;
		case eGeometry::NON_PRIMITIVE:
		case eGeometry::CUSTOM_MESH:
			Collision = BuildCustomBody(mesh, !stParam.masse, m_World);
			break;
		default:
			MSGBOX("error loadBody", "Geometry not implemented with Physic Engine");
			break;
		}

		return(Collision);
	}
	dMatrix CLPhysGeom::matrix_TVMeshToNewton()
	{
		dMatrix Matrix;
		cTV_3DMATRIX tvMatrix = mesh->GetMatrix();	
		/*CTVMathLibrary math;
		cTV_3DMATRIX rotMat;
		//math.TVMatrixRotationY(&rotMat,90);
		if ((stParam.geom == eGeometry::CONE) || (stParam.geom == eGeometry::CYLINDER))
		{
			cTV_3DMATRIX rotCone;
			math.TVMatrixRotationZ(&rotCone, -90);
			math.TVMatrixMultiply(&rotMat, &rotCone, &rotMat);
			
		}
			
	//	math.TVMatrixRotationYawPitchRoll(&rotMat, 0, -, 0);
		cTV_3DMATRIX transMat;
		math.TVMatrixMultiply(&transMat, &tvMatrix, &rotMat);

		
		Matrix.m_front = &transMat.m[2][0];
		Matrix.m_front.m_w = 0;   // TODO: is ok??
		Matrix.m_up = &transMat.m[1][0];
		Matrix.m_right.m_x = -transMat.m11;
		Matrix.m_right.m_y = -transMat.m12;
		Matrix.m_right.m_z = -transMat.m13;
		Matrix.m_right.m_w = -transMat.m14;  // we dont really care  ??
		
		Matrix.m_posit = &tvMatrix.m[3][0];
		Matrix.m_posit.m_w = 1;  //  ok??*/

		
		Matrix = &tvMatrix.m[0][0];
		CTVMathLibrary math;
		if ((stParam.geom == eGeometry::CONE) || (stParam.geom == eGeometry::CYLINDER))
		{
			Matrix = dRollMatrix(float(PI * 0.5f)) * Matrix;
			
		}
		return (Matrix);
	}
	cTV_3DMATRIX* CLPhysGeom::matrix_bodyToTV()
	{
		dMatrix Matrix;
		NewtonBodyGetMatrix(pbody, &Matrix[0][0]);
		//Matrix = Matrix * dYawMatrix(3.141592f * -0.5f);
		//Matrix = Matrix * dPitchMatrix(3.141592f * -0.5f);
		
		
		/*CTVMathLibrary math;
		cTV_3DMATRIX rotMat;
		math.TVMatrixRotationYawPitchRoll(&rotMat, 0, 0,0);  // TODO  :L check, pourrait etre identity matrix si set a 0.

		if ((stParam.geom == eGeometry::CONE) || (stParam.geom == eGeometry::CYLINDER))
		{
			math.TVMatrixRotationYawPitchRoll(&rotMat, 0,-45,-90);
		}
		MX.m11 = -Matrix.m_right.m_x;
		MX.m12 = -Matrix.m_right.m_y;
		MX.m13 = -Matrix.m_right.m_z;
		MX.m14 = 0; // -Matrix.m_right.m_w;
		MX.m21 = Matrix.m_up.m_x;
		MX.m22 = Matrix.m_up.m_y;
		MX.m23 = Matrix.m_up.m_z;
		MX.m24 = 0; // Matrix.m_up.m_w;
		MX.m31 = Matrix.m_front.m_x;
		MX.m32 = Matrix.m_front.m_y;
		MX.m33 = Matrix.m_front.m_z;
		MX.m34 = 0; // Matrix.m_front.m_w;
		math.TVMatrixMultiply(&MX,  (cTV_3DMATRIX*)&Matrix[0][0], &rotMat);


		MX.m41 = Matrix.m_posit.m_x;
		MX.m42 = Matrix.m_posit.m_y;
		MX.m43 = Matrix.m_posit.m_z;
		MX.m44 = 1; // Matrix.m_posit.m_w;*/

		if ((stParam.geom == eGeometry::CONE) || (stParam.geom == eGeometry::CYLINDER))
		{
			Matrix = dRollMatrix(float(PI * -0.5f)) * Matrix;

		}
		
		
		MX.m11 = Matrix[0][0];
		MX.m12 = Matrix[0][1]; 
		MX.m13 = Matrix[0][2];
		MX.m14 = Matrix[0][3]; // -Matrix.m_right.m_w;
		MX.m21 = Matrix[1][0];
		MX.m22 = Matrix[1][1];
		MX.m23 = Matrix[1][2];
		MX.m24 = Matrix[1][3]; // Matrix.m_up.m_w;
		MX.m31 = Matrix[2][0];
		MX.m32 = Matrix[2][1];
		MX.m33 = Matrix[2][2];
		MX.m34 = Matrix[2][3]; // Matrix.m_front.m_w;
		MX.m41 = Matrix[3][0];
		MX.m42 = Matrix[3][1];
		MX.m43 = Matrix[3][2];
		MX.m44 = Matrix[3][3];

		/*float* val = &MX.m[0][0];
		std::cout << std::endl << "convtMat:" << std::endl;
		for (int i = 0; i < 16; i++)
		{
			std::cout << *val << " ";
			val++;
			if (i == 3) std::cout << std::endl;
			if (i == 7) std::cout << std::endl;
			if (i == 11) std::cout << std::endl;
		
		}*/
		return(&MX);
	}
	void CLPhysGeom::matrixTVmeshToBody()
	{
		NewtonBodySetMatrix(pbody, &matrix_TVMeshToNewton()[0][0]);
	}
	void CLPhysGeom::setMass(float mass)
	{
		stParam.masse = mass;
		matrix_setMass();
	}
	void CLPhysGeom::matrix_setMass()
	{
		// On calcul l'inertie du corps, en passant par une petite formule
		dVector inertie;
		inertie.m_x = 0.7f * stParam.masse * (stParam.scale.y * stParam.scale.y + stParam.scale.z * stParam.scale.z) / 12;
		inertie.m_y = 0.7f * stParam.masse * (stParam.scale.x * stParam.scale.x + stParam.scale.z * stParam.scale.z) / 12;
		inertie.m_z = 0.7f * stParam.masse * (stParam.scale.x * stParam.scale.x + stParam.scale.y * stParam.scale.y) / 12;
		// On définit ensuite la masse et l'inertie pour ce corps
		NewtonBodySetMassMatrix(pbody, stParam.masse, inertie.m_x, inertie.m_y, inertie.m_z);
	}
	void CLPhysGeom::addGravity()
	{
		float masse;
		dVector inertie;
		dVector force;
		// On récupère en premier lieu la masse ainsi que l'inertie
		NewtonBodyGetMassMatrix(pbody, &masse, &inertie.m_x, &inertie.m_y, &inertie.m_z);
		force.m_x = 0.0f;
		force.m_y = -masse * GRAVITY;
		force.m_z = 0.0f;
		NewtonBodyAddForce(pbody, &force.m_x); // On ajoute la force au corps
	}
	void CLPhysGeom::addForce(cTV_3DVECTOR force)
	{
		NewtonBodyAddForce(pbody, &force.x); // On ajoute la force au corps
	}
	cTV_3DVECTOR CLPhysGeom::getVelocity()
	{
		cTV_3DVECTOR velo;
		NewtonBodyGetVelocity(pbody, &velo.x);
		return(velo);
	}
	cTV_3DVECTOR CLPhysGeom::getPos()
	{
		/*
		cTV_3DMATRIX matrix;
		cTV_3DVECTOR bodyPos;
		NewtonBodyGetMatrix(pbody, matrix.m[0]);
		bodyPos.x = matrix.m41;
		bodyPos.y = matrix.m42;
		bodyPos.z = matrix.m43;
		*/
		return(mesh->GetPosition());
	}
	void CLPhysGeom::reset()
	{
		mesh->SetPosition(stParam.pos.x, stParam.pos.y, stParam.pos.z);
		matrixTVmeshToBody();
		dVector force(0, 0, 0);
		NewtonBodySetForce(pbody, &force.m_x);
	}
	void DebugShowGeometryCollision(void* userData, int vertexCount, const dFloat* faceVertec, int id)
		{
			CTVScreen2DImmediate* p2D = new CTVScreen2DImmediate();
			int i = vertexCount - 1;
			dVector p0(faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
			for (int i = 0; i < vertexCount; i++)
			{
				dVector p1(faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
				p2D->Draw_Line3D(p0.m_x, p0.m_y, p0.m_z, p1.m_x, p1.m_y, p1.m_z, RGBA_WHITE);
				p0 = p1;
			}

			delete(p2D); p2D = NULL;
		}
	void CLPhysGeom::NewtonDebug()
	{
			dMatrix matrix;
			NewtonBodyGetMatrix(pbody, &matrix[0][0]);
			NewtonCollisionForEachPolygonDo(NewtonBodyGetCollision(pbody), &matrix[0][0], DebugShowGeometryCollision, NULL);
	}
	NewtonCollision* CLPhysGeom::BuildCustomBody(CTVMesh* Mesh, bool isStatic, NewtonWorld* World) {
		//  Build the physics mesh and colision tree	
		float Temp;
		int i, vec1, vec2, vec3, Group, color, NbrFaces, NbrVertex;
		NbrFaces = Mesh->GetTriangleCount();
		NbrVertex = Mesh->GetVertexCount();
		char* name = Mesh->GetMeshName();
		std::cout << std::endl << "Creating " << name << " physic body" << "- Faces: " << NbrFaces << "Vertex: " << NbrVertex << std::endl;
		sVERTEX *sVertex = new sVERTEX[NbrFaces * 3];
		for (int j = 0; j < NbrFaces * 3; j++)
		{
			sVertex[j].x = 0;    // Initialize all elements to zero.
			sVertex[j].y = 0;
			sVertex[j].z = 0;
		}


		NewtonCollision* MeshCollision = nullptr;
		NewtonMesh* nMesh = nullptr;

		if (isStatic) {
			MeshCollision = NewtonCreateTreeCollision(World, 666);
			NewtonTreeCollisionBeginBuild(MeshCollision);
		} else {
			nMesh = NewtonMeshCreate(World);
			NewtonMeshBeginFace(nMesh);
		}

		std::cout << "Triangles info retreived from TVMesh: " << std::endl;
		for (i = 0; i < NbrFaces * 3; i = i + 3)
		{
			//std::cout << "Face: " << i/3 << std::endl;
			Mesh->GetTriangleInfo(i / 3, &vec1, &vec2, &vec3, &Group);
			Mesh->GetVertex(vec1, &sVertex[i].x, &sVertex[i].y, &sVertex[i].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
			Mesh->GetVertex(vec2, &sVertex[i + 1].x, &sVertex[i + 1].y, &sVertex[i + 1].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
			Mesh->GetVertex(vec3, &sVertex[i + 2].x, &sVertex[i + 2].y, &sVertex[i + 2].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
			/*	std::cout << " - Vertex 1: " << sVertex[i].x << "'" << sVertex[i].y << "'" << sVertex[i].z << std::endl;
				std::cout << " - Vertex 2: " << sVertex[i + 1].x << "'" << sVertex[i + 1].y << "'" << sVertex[i + 1].z << std::endl;
				std::cout << " - Vertex 3: " << sVertex[i + 2].x << "'" << sVertex[i + 2].y << "'" << sVertex[i + 2].z << std::endl;*/
			if (isStatic)
				NewtonTreeCollisionAddFace(MeshCollision, 3, &sVertex[i].x, sizeof(sVERTEX), 0);
			else
				NewtonMeshAddFace(nMesh, 3, &sVertex[i].x, sizeof(sVERTEX), 0);
		}

		// Finalize the collision tree build
		if (isStatic)
			NewtonTreeCollisionEndBuild(MeshCollision, 0);
		else {
			NewtonMeshEndFace(nMesh);
			MeshCollision = NewtonCreateConvexHullFromMesh(World, nMesh, 0.05f, 666); // tolerance: 0.05
			NewtonMeshDestroy(nMesh);
		}
		delete[] sVertex;  // When done, free memory pointed to by sVertex.
		sVertex = NULL;     // Clear a to prevent using invalid memory reference.
		return(MeshCollision);
	}
}