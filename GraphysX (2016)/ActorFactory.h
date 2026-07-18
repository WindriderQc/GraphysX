#pragma once
#include "Actor.h"


class CLActorFactory 
{
public:
	CLActorFactory(NewtonWorld* world);
	~CLActorFactory();
private:	
	int   IDDiffuse;             //This is an ID for our texture. You can use strings also but strings are slower.
	int   IDNormal;              //This is an ID for our bump texture. RGB is a normal map, A is a parallax map. I've done it this way to demonstrate that you can pack data in textures. So here we have all of our required bumping information in one sole RGBA texture map - instead of 2 seperate ones.
	NewtonWorld* World;
	

public:
	CLActor* clActorGate;
	CLActor* clTestAnim;
	CLActor* clTestAnim1;
	std::vector<CLActor*> vActors;
	void CreateActor(char* name, char* xMeshPath, cTV_3DVECTOR pos, char* diffuseTexPath);
	GRAPHYSX_API CLActor* createCubX();
	void Update();
	
};