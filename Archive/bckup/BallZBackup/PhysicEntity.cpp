#include "stdafx.h"
#include "PhysicEntity.h"

void DefaultForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
{ // Force and Torque callback (called everytime a modification occurs on a body)
	
	float masse;
	dVector inertie;
	dVector force;

	// On récupère en premier lieu la masse ainsi que l'inertie
	NewtonBodyGetMassMatrix(body, &masse, &inertie.m_x, &inertie.m_y, &inertie.m_z);
	force.m_x = 0.0f;
	force.m_y = -masse * GRAVITY;
	force.m_z = 0.0f;
	NewtonBodyAddForce(body, &force.m_x); // On ajoute la force au corps

}
void DefaultTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{ // transformation callback (called after the body has been simulated)

}

void DebugShowGeometryCollision(void* userData, int vertexCount, const dFloat* faceVertec, int id)
{
	CTVScreen2DImmediate* p2D = new CTVScreen2DImmediate();
	int i = vertexCount - 1;
	dVector p0(faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
	for (int i = 0; i < vertexCount; i++)
	{
		dVector p1(faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
		p2D->Draw_Line3D(p0.m_x, p0.m_y, p0.m_z, p1.m_x, p1.m_y, p1.m_z, RGBA_WHITE, -2);
		p0 = p1;
	}

	delete(p2D); p2D = NULL;
}

CLPhysicEntity::CLPhysicEntity(dMatrix Matrix, float masse, dVector Scale, int newtonMat, NewtonWorld* world, NewtonCollision* coli) : fMasse(masse), Collision(coli), iNewtonMat(newtonMat)
{
	
	Matrix.m_front.m_x = 1;// PaTCH  LAITTEEE!!!  SEMBLE AFFECTER LA PHYSICS..   si on scale le TVmesh avant, le vector d'orientation n'est plus de 1 partout... ok pur now mais si on set le ROT du mesh pendant la creation, ca va chier.
	Matrix.m_up.m_y = 1;
	Matrix.m_right.m_z = 1; 
	pbody = NewtonCreateDynamicBody(world, Collision, (float*)&Matrix); // crash si on scale le mesh avant sans la patch
	NewtonBodySetMaterialGroupID(pbody, iNewtonMat);
	
	if (fMasse) {
		// On calcul l'inertie du corps, en passant par une petite formule
		dVector inertie;
		inertie.m_x = 0.7f * fMasse * (Scale.m_y * Scale.m_y + Scale.m_z * Scale.m_z) / 12;
		inertie.m_y = 0.7f * fMasse * (Scale.m_x * Scale.m_x + Scale.m_z * Scale.m_z) / 12;
		inertie.m_z = 0.7f * fMasse * (Scale.m_x * Scale.m_x + Scale.m_y * Scale.m_y) / 12;
		// On définit ensuite la masse et l'inertie pour ce corps
		NewtonBodySetMassMatrix(pbody, fMasse, inertie.m_x, inertie.m_y, inertie.m_z);
		// On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
		NewtonBodySetForceAndTorqueCallback(pbody, DefaultForceAndTorqueCallback);
		NewtonBodySetTransformCallback(pbody, DefaultTransformationCallback);
	}
	physMat = DEF_PHYSMAT;
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
CLPhysicEntity::~CLPhysicEntity()
{
	if (Collision) NewtonDestroyCollision(Collision);  Collision = NULL;
	if (pbody)	NewtonDestroyBody(pbody);   pbody = NULL;
}
void CLPhysicEntity::NewtonDebug(void)
{
	if (pbody != NULL)
	{
		dMatrix matrix;
		NewtonBodyGetMatrix(pbody, &matrix[0][0]);
		NewtonCollisionForEachPolygonDo(NewtonBodyGetCollision(pbody), &matrix[0][0], DebugShowGeometryCollision, NULL);
	}
}
void CLPhysicEntity::setForceNTorqueCallback(NewtonApplyForceAndTorque callback)
{
	NewtonBodySetForceAndTorqueCallback(pbody, callback);

}
void CLPhysicEntity::setTransformCallback(NewtonSetTransform callback)
{
	NewtonBodySetTransformCallback(pbody, callback);

}
void CLPhysicEntity::setBodyUserData(void* const data)
{
	NewtonBodySetUserData(pbody, data);
}
void*  CLPhysicEntity::getBodyUserData(const NewtonBody* body)
{
	return(NewtonBodyGetUserData(body));
}
void CLPhysicEntity::updateMassMatrix()
{
NewtonBodyGetMassMatrix(pbody, &fMasse, &inertie->x, &inertie->y, &inertie->z);
}
void CLPhysicEntity::addForce(cTV_3DVECTOR force)
{
  NewtonBodyAddForce(pbody, &force.x);
}


void CLPhysicEntity::getMatrix(float* matrix )  {
	NewtonBodyGetMatrix(pbody, matrix);
}

cTV_3DVECTOR CLPhysicEntity::getVelocity()
{
	cTV_3DVECTOR velo;
	NewtonBodyGetVelocity(pbody, &velo.x);
	return(velo);
}

// DEVRAIT ETRE SPLITTÉ en 2...  construire une array de vertex/face d'une fonction dans Mesh Factory, 
//et ensuite creer le physic collision ici en recevant l'array en param, ce qui rendrait 
//Physics entity independant ( pas utilisé TVMoteur)
NewtonCollision* CLPhysicEntity::BuildPhysicMesh(CTVMesh* Mesh, NewtonWorld* World) {
	//  Build the physics mesh and colision tree	
	float Temp;
	int i, vec1, vec2, vec3, Group, color, NbrFaces, NbrVertex;

	NbrFaces = Mesh->GetTriangleCount();
	NbrVertex = Mesh->GetVertexCount();
	char* name = Mesh->GetMeshName();
	std::cout << "Creating " << name << " physic body" << "- Faces/Vertex: " << NbrFaces << "/" << NbrVertex <<  std::endl;
	sVERTEX *sVertex = new sVERTEX[NbrFaces * 3];
	for (int j = 0; j<NbrFaces * 3; j++)
	{
		sVertex[j].x = 0;    // Initialize all elements to zero.
		sVertex[j].y = 0;
		sVertex[j].z = 0;
	}
	NewtonMesh* nMesh = NewtonMeshCreate(World);
	NewtonMeshBeginFace(nMesh);
	
	for (i = 0; i < NbrFaces * 3; i = i + 3)
	{
		Mesh->GetTriangleInfo(i / 3, &vec1, &vec2, &vec3, &Group);
		Mesh->GetVertex(vec1, &sVertex[i].x, &sVertex[i].y, &sVertex[i].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		Mesh->GetVertex(vec2, &sVertex[i + 1].x, &sVertex[i + 1].y, &sVertex[i + 1].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		Mesh->GetVertex(vec3, &sVertex[i + 2].x, &sVertex[i + 2].y, &sVertex[i + 2].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		NewtonMeshAddFace(nMesh, 3, &sVertex[i].x, sizeof(sVERTEX), 0);
	}
	// Finalize the collision tree build
	NewtonMeshEndFace(nMesh);
	delete[] sVertex;  // When done, free memory pointed to by sVertex.
	sVertex = NULL;     // Clear a to prevent using invalid memory reference.

	NewtonCollision* MeshCollision = NewtonCreateConvexHullFromMesh(World, nMesh, 0.01f, 666); // tolerance: 0.05
	NewtonMeshDestroy(nMesh);
	return(MeshCollision);
}
NewtonCollision* CLPhysicEntity::BuildPhysicStaticBody(CTVMesh* Mesh, NewtonWorld* World) {
	//  Build the physics mesh and colision tree	
	float Temp;
	int i, vec1, vec2, vec3, Group, color, NbrFaces, NbrVertex;
	NbrFaces = Mesh->GetTriangleCount();
	NbrVertex = Mesh->GetVertexCount();
	char* name = Mesh->GetMeshName();
	std::cout << std::endl << "Creating " << name << " physic body" << "- Faces: " << NbrFaces << "Vertex: " << NbrVertex << std::endl;
	sVERTEX *sVertex = new sVERTEX[NbrFaces * 3];
	for (int j = 0; j<NbrFaces * 3; j++)
	{
		sVertex[j].x = 0;    // Initialize all elements to zero.
		sVertex[j].y = 0;
		sVertex[j].z = 0;
	}
	NewtonCollision* MeshCollision = NewtonCreateTreeCollision(World, 666);
	NewtonTreeCollisionBeginBuild(MeshCollision);
	std::cout << "Triangles info retreived from TVMesh: " << std::endl;
	for (i = 0; i < NbrFaces * 3; i = i + 3)
	{
		//std::cout << "Face: " << i/3 << std::endl;
		Mesh->GetTriangleInfo(i/3, &vec1, &vec2, &vec3, &Group);
		Mesh->GetVertex(vec1, &sVertex[i].x, &sVertex[i].y, &sVertex[i].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		Mesh->GetVertex(vec2, &sVertex[i + 1].x, &sVertex[i + 1].y, &sVertex[i + 1].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		Mesh->GetVertex(vec3, &sVertex[i + 2].x, &sVertex[i + 2].y, &sVertex[i + 2].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
	//	std::cout << " - Vertex 1: " << sVertex[i].x << "'" << sVertex[i].y << "'" << sVertex[i].z << std::endl;
	//	std::cout << " - Vertex 2: " << sVertex[i + 1].x << "'" << sVertex[i + 1].y << "'" << sVertex[i + 1].z << std::endl;
	//	std::cout << " - Vertex 3: " << sVertex[i + 2].x << "'" << sVertex[i + 2].y << "'" << sVertex[i + 2].z << std::endl;
		NewtonTreeCollisionAddFace(MeshCollision, 3, &sVertex[i].x, sizeof(sVERTEX), 0);
	}
	// Finalize the collision tree build
	NewtonTreeCollisionEndBuild(MeshCollision, 0);
	delete[] sVertex;  // When done, free memory pointed to by sVertex.
	sVertex = NULL;     // Clear a to prevent using invalid memory reference.
	return(MeshCollision);
}
