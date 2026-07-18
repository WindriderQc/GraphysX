#include "stdafx.h"
#include "3DScenes.h"


void ElevatorForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
{
	// Cette fonction est une fonction Callback. Elle sera appelée à chaque fois
	// qu'une modification aura lieu sur le corps.

	// On récupère en premier lieu la masse ainsi que l'inertie
	//float masse; // Contiendra la masse de l'objet pris en paramètre par la fonction
	cTV_3DVECTOR inertie; // Contiendra l'inertie du corps
	cTV_3DVECTOR force; // Spécifiera la force appliquée sur le corps

	//Pas de gravité pour que l'object reste stable.
	/*
	NewtonBodyGetMassMatrix(body, &masse, &inertie.x, &inertie.y, &inertie.z);
	force.x = 0.0f;
	force.y = -masse * 9.81* SCALE; // 9.81 est l'attraction gravitationnelle de la Terre
	force.z = 0.0f;
	NewtonBodyAddForce(body, &force.x); // On ajoute la force au corps
	*/

	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
}
void ElevatorTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	cTV_3DMATRIX* MX = NULL;
	NewtonBodyGetMatrix(body, MX->m[0]);
	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	cl3DObject->Mesh->SetMatrix(MX);
}
static void GenericContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body0);
	if (!cl3DObject)
	{
		cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body1);
	}

	cl3DObject->bAllowJumps = true;

	/*

	for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact))
	{
	dFloat contactNormalSpeed;
	NewtonMaterial* material;

	// get the material for this contact;
	material = NewtonContactGetMaterial (contact);

	contactNormalSpeed = NewtonMaterialGetContactNormalSpeed (material);
	/*
	if (contactNormalSpeed > contactBestSpeed)
	{
	contactBestSpeed = contactNormalSpeed;
	dVector normal;
	contactBestSpeed = contactNormalSpeed;
	NewtonMaterialGetContactPositionAndNormal (material, body0, &contactPosit[0], &normal[0]);
	//bestSound = NewtonMaterialGetMaterialPairUserData (material);
	}

	}
	*/
}
static void FinishLineContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body0);
	if (!cl3DObject){
		cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body1);
	}

	cl3DObject->bIsFinishline = true;
	//  ICI faire un call de l'Avion qui va chercher la balle.  Devrait etre une creation d'objet avion, 
	//  qui sera détruit quand la balle arrive a destination

}
static void ElevatorContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body0);
	if (cl3DObject->Mesh->GetMeshName() != "BallZShell")
		cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body1);

	cl3DObject->bAllowJumps = true;
	cl3DObject->bIsOnElevator = true;
}

CLBallZScene::CLBallZScene() : CLScene()
{
	pOldMeshClicked = NULL;
	pMeshClicked = NULL;
	
	clSound->PlaySound(clSound->iIntro);
	
	clTV3D->pTVScene->SetRenderMode(cTV_SOLID); //);cTV_LINE
	clTV3D->pTVScene->SetShadowParameters(RGBA(0.0, 0.0, 0.0, 0.5), false);
	clTV3D->pTVScene->SetAutoTransColor(cTV_COLORKEY_USE_ALPHA_CHANNEL);// cTV_COLORKEY_MAGENTA);//
	SetPhysicMaterials();
	LoadTextures();
	SetupScene();
	clSound->PlaySound(clSound->iPiano);
}
CLBallZScene::~CLBallZScene(void)
{
	if (clAtmosphere){delete(clAtmosphere); clAtmosphere = NULL;}
	if (clAnneaux)   {delete(clAnneaux);	clAnneaux = NULL;}
	if (clBallZ)     {delete(clBallZ);		clBallZ = NULL;}
	cl3DObjMgr->kill();
	clSound->kill();
	clInput->kill();
}
void CLBallZScene::SetPhysicMaterials(void)
{
	// get the default material ID
	defaultID = NewtonMaterialGetDefaultGroupID(World);
	// set default material properties
	NewtonMaterialSetDefaultSoftness(World, defaultID, defaultID, 0.05f);
	NewtonMaterialSetDefaultElasticity(World, defaultID, defaultID, 0.4f);
	NewtonMaterialSetDefaultCollidable(World, defaultID, defaultID, 1);
	NewtonMaterialSetDefaultFriction(World, defaultID, defaultID, 1.0f, 0.5f);
	// create all materials ID
	woodID = NewtonMaterialCreateGroupID(World);
	FinishID = NewtonMaterialCreateGroupID(World);
	floorID = NewtonMaterialCreateGroupID(World);
	BallPhysID = NewtonMaterialCreateGroupID(World);
	ElevatorPhysID = NewtonMaterialCreateGroupID(World);
	// create the character material interactions
	// set the material properties for character on Wood
	NewtonMaterialSetDefaultElasticity(World, woodID, BallPhysID, 0.3f);
	NewtonMaterialSetDefaultFriction(World, woodID, BallPhysID, 1.1f, 0.8f);
	NewtonMaterialSetCollisionCallback(World, woodID, BallPhysID, NULL, NULL, GenericContactProcess);
	// set the material properties for character on Controllable plate
	NewtonMaterialSetDefaultElasticity(World, FinishID, BallPhysID, 0.1f);
	NewtonMaterialSetDefaultFriction(World, FinishID, BallPhysID, 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, FinishID, BallPhysID, NULL, NULL, FinishLineContactProcess);
	// set the material properties for character on level 
	NewtonMaterialSetDefaultElasticity(World, floorID, BallPhysID, 0.3f);
	NewtonMaterialSetDefaultFriction(World, floorID, BallPhysID, 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, floorID, BallPhysID, NULL, NULL, GenericContactProcess);
	// set the material properties for Wood on Floor 
	NewtonMaterialSetDefaultElasticity(World, floorID, woodID, 0.3f);
	NewtonMaterialSetDefaultFriction(World, floorID, woodID, 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, floorID, woodID, NULL, NULL, GenericContactProcess);
	// set the material properties for character on Elevator
	NewtonMaterialSetDefaultElasticity(World, ElevatorPhysID, BallPhysID, 0.3f);
	NewtonMaterialSetDefaultFriction(World, ElevatorPhysID, BallPhysID, 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, ElevatorPhysID, BallPhysID, NULL, NULL, ElevatorContactProcess);

}
void CLBallZScene::LoadTextures(void)
{
	//DEVRAIT ETRE UN VECTOR DE INT OU UN ENUM OU WHATEVER AUTRE QUE CA! :)

	iYellowtwowaysignTex = clTV3D->pTVTexturefactory->LoadTexture("Media\\yellowtwoway.jpg", "Ground");
	//iGridTex = clTV3D->pTVTexturefactory->LoadTexture("Media\\GridXL.bmp", "Grid");
	iDamierTex = clTV3D->pTVTexturefactory->LoadTexture("Media\\Damier.jpg", "Damier");
	i3D_Spheres = clTV3D->pTVTexturefactory->LoadTexture("Media\\3D_Spheres.jpg", "3D_Spheres");
	iMagicienTex = clTV3D->pTVTexturefactory->LoadTexture("Media\\Zack.jpg", "Zack");
	iAbstractCubes = clTV3D->pTVTexturefactory->LoadTexture("Media\\AbstractCubes.jpg", "AbstractCubes");
	//itexGrass = pTVTexturefactory->LoadTexture("Media\\grass.jpg", "Grass");

	//Load our particle texture.
	IDParticleTex = clTV3D->pTVTexturefactory->LoadTexture("Media\\p.dds", "particle tex", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);
	
	//Sky Box
	stSkyDay.SkyTop = clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\up.jpg", "SkyTop");
	stSkyDay.SkyBottom = clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\down.jpg", "SkyBottom");
	stSkyDay.SkyLeft = clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\left.jpg", "SkyLeft");
	stSkyDay.SkyRight = clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\right.jpg", "SkyRight");
	stSkyDay.SkyFront = clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\front.jpg", "SkyFront");
	stSkyDay.SkyBack = clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\back.jpg", "SkyBack");
	stSkyNight.SkyTop = clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\up.jpg", "NightSkyTop", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyBottom = clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\down.jpg", "NightSkyBottom", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyLeft = clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\left.jpg", "NightSkyLeft", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyRight = clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\right.jpg", "NightSkyRight", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyFront = clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\front.jpg", "NightSkyFront", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyBack = clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\back.jpg", "NightSkyBack", -1, -1, cTV_COLORKEY_NO, true);
}
void CLBallZScene::SetupScene(void)
{
	// We set a mouse pointer
	clTV3D->pTVScene->SetCursor(true, clTV3D->pTVTexturefactory->LoadTexture("Media\\pointer.bmp", "CursorTexture", -1, -1, cTV_COLORKEY_BLACK, true));
	// We set background color.
	//clTV3D->pTVScene->SetBackgroundColor(0.0f, 0.3f, 0.9f);
	
	// The land generation
	pLand = new CTVLandscape();
    pLand = clTV3D->pTVScene->CreateLandscape("Land");
	pLand->CreateEmptyTerrain(cTV_PRECISION_LOW,8,8,-1024,-100,-1024);
	// Generate the height of the land from the grayscale of the image.
	//pLand->GenerateTerrain("Media\\Heightmaps\\Track.jpg", cTV_PRECISION_LOW, 8, 8, -1024,-100, -1024, true);
	// We assign a texture to that land.
	pLand->SetTexture(iDamierTex);

	clAtmosphere = new CLAtmosphere(stSkyDay, stSkyNight);
	//clWater = new CLWater();
	//clLand = new CLLand(""media\\\\Heightmaps\\Height.jpg", "Media\\Grass.jpg", 10, 10, cTV_3DVECTOR(-768, -180, -512), World);



	XMesh = clTV3D->pTVScene->CreateMeshBuilder();
	XMesh->LoadXFile("c:\\media\\Nature\\bush1.X");
	XMesh->SetAlphaTest(true);
	XMesh->SetScale(0.1f, 0.1f, 0.1f);
	XMesh->SetBlendingMode(cTV_BLEND_ALPHA);

	
	vecPosition3D PosBuissons;
	PosBuissons = { cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(2, 0, 4), cTV_3DVECTOR(4, 0, 0) };   //cTVXmlSplineRead("Media\\Anneaux.xml");		
	for each(cTV_3DVECTOR vec in PosBuissons)
	{
		cl3DObjMgr->VecXMesh.push_back(XMesh->Duplicate());
		cl3DObjMgr->VecXMesh.back()->SetPosition(vec.x, vec.y, vec.z);
	}

	//XMesh->Create3DText("TEST", "Times New Roman", 32, 0.2f);
	//XMesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	//XMesh->SetColor(BLUE_RGBA);
	//XMesh->set
	//XMesh->SetBlendingMode(cTV_BLEND_ALPHA);
		


	/*
	CREATE 3D OBJECTS
	*/

	
	//  Flyer Zone
	int NbrFlyers = 8;
	clFormation = new CLFormation(NbrFlyers, 2, false);

	for (int i = 0; i < NbrFlyers; i++)
	{
		string name("Flyer" + to_string((long double)i));
		cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, &name[0], NULL, iMagicienTex, cTV_3DVECTOR((5 * (i + 1)) - 30, 0, 20), cTV_3DVECTOR(1.5f, 1.5f, 1.5f), true, 0.0f, PHYSICSTATIC, woodID, World));
		clFormation->clFlyers[i]->Mesh = cl3DObjMgr->VecObjects.back()->Mesh;
		clFormation->clFlyers[i]->Position.push_back(clFormation->clFlyers[i]->Mesh->GetPosition()); // 2 pts pour creer une étape
		clFormation->clFlyers[i]->Position.push_back(clFormation->clFlyers[i]->Mesh->GetPosition()); // 2 pts pour creer une étape
		clFormation->clFlyers[i]->Orientation.push_back(clFormation->clFlyers[i]->Mesh->GetRotation());
		clFormation->clFlyers[i]->Orientation.push_back(clFormation->clFlyers[i]->Mesh->GetRotation());
		if (!i)// Set the BaseFlyer's vertex color to green
			clFormation->clFlyers[i]->Mesh->SetColor(GREEN_RGBA, false);
	}
	///////////////////////////////////////////



	clAnneaux = new CLAnneaux(World, IDParticleTex, YELLOW_RGBA);
	clVoieLactee = new CLVoieLactee(World);
	clBallZ = new CLBallZ(World, cTV_3DVECTOR(0, 0.05f, 0), 0.02f, 1, BallPhysID);
	cl3DObjMgr->VecObjects.push_back(clBallZ->clCtrl);
	cl3DObjMgr->VecObjects.push_back(clBallZ->clShell);

	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "RefCube", NULL, iYellowtwowaysignTex, cTV_3DVECTOR(4, 1, 0), cTV_3DVECTOR(1, 1, 1), true, 2.0f, PHYSICALL, woodID, World));
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "RefCubeBlender", "Media\\BlenderRefBox3ds.tvm", iYellowtwowaysignTex, cTV_3DVECTOR(8, 1, 0), cTV_3DVECTOR(1, 1, 1), true, 2.0f, PHYSICALL, woodID, World));



	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "NoWhere", NULL, iYellowtwowaysignTex, cTV_3DVECTOR(0, 10, 0), cTV_3DVECTOR(0.1f, 0.1f, 0.1f), true, NULL, PHYSICSTATIC, woodID, World));
	clObjectNowhere = cl3DObjMgr->VecObjects.back();
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Finish", "Media\\Finish.TVM", iDamierTex, cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(0, 0, 0), true, NULL, PHYSICSTATIC, FinishID, World));
	//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "PlayGroung", NULL, iAbstractCubes, cTV_3DVECTOR(0, -0.5, -15), cTV_3DVECTOR(50, 0.25, 50), true, NULL, PHYSICSTATIC, floorID, World));
	//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Hole1", "Media\\World1LevelHole1.TVM", iYellowtwowaysignTex, cTV_3DVECTOR(0, 0, 0), vTaille, true, NULL, PHYSICSTATIC, floorID, World));
	//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Hole2", "Media\\World1LevelHole2.TVM", iYellowtwowaysignTex, cTV_3DVECTOR(0, 0, 0), vTaille, true, NULL, PHYSICSTATIC, floorID, World));
	//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Corridor", "Media\\Corridor.TVM", NULL, cTV_3DVECTOR(0, 0, 0), vTaille, true, NULL, PHYSICSTATIC, floorID, World));
	//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Obstacles", "Media\\Obstacles3ds.TVM", NULL, cTV_3DVECTOR(0, 0, 0), vTaille, true, NULL, PHYSICALL, floorID, World));
	//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Level2", "Media\\Level\\Level2.TVM", NULL, cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(0, 0, 0), true, NULL, PHYSICSTATIC, floorID, World));
	
	/*
	CL3DObject *test = cl3DObjMgr->VecObjects.back();
	// Drop all the group names to the debug file
	cout << " Number of Groups : " << test->Mesh->GetGroupsNumber() << endl;
	//cout
	for (int i = 1; i < test->Mesh->GetGroupsNumber(); i++)
	{
		cout << "Group #" << i << " : " << test->Mesh->GetGroupName(i);
	}
	*/

	
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Airplane", "Media\\Airplane\\AirplaneLP.TVM", NULL, cTV_3DVECTOR(3, 15, 0), cTV_3DVECTOR(1, 1, 1), true, 0.5, NULL, NULL, World));
	vecPosition3D Spline =  XmlSplineRead("Media\\Spline.xml");// { cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(10, 20, 0) };
	clPlanePATH = new CLSplineFollow(cl3DObjMgr->VecObjects.back()->Mesh, Spline, true);
	clPlanePATH->SetSpeed(4);

	for (int j = 0; j < 10; j++)
	{
		string name("Box" + to_string((long double)j));
		cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, &name[0], NULL, i3D_Spheres, cTV_3DVECTOR(2, -8.0f + j, 7.0f), cTV_3DVECTOR(1, 1, 1), true, 0.075f, PHYSICALL, woodID, World));
	}
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(BILLBOARD, "Zack", NULL, iMagicienTex, cTV_3DVECTOR(-20, 0, -30), cTV_3DVECTOR(8, 6, 0), true, NULL, NULL, NULL, World));

	CreateBoxTrianglePile(2.0f, cTV_3DVECTOR(0,5,0), iMagicienTex);
	
	//  TO DO:  UNE METHOD QUI PERMET DE CREER DES BILLBOARD EFFECTS

	//  CHECKER LE common.shader dans le répoertoire des flames...  il te manque de koi a faire le grand! ;)
	int iFlamesTex[8];
	int iFlamesTexCycle;
	for (int i = 0; i < 8; i++)
	{
		string path = "Media\\Flames\\Flame";
		string extension = ".jpg";
		stringstream ss;
		ss << path << i + 1 << extension;
		auto x = ss.str();
		const char* FullPath = x.c_str();
		string name = "Flame";
		stringstream ss1;
		ss1 << name << i + 1;
		auto y = ss1.str();
		const char* FullName = y.c_str();
		iFlamesTex[i] = clTV3D->pTVTexturefactory->LoadTexture(FullPath, FullName, -1, -1, cTV_COLORKEY_BLACK);
	}
	iFlamesTexCycle = clTV3D->pTVTexturefactory->CreateTextureCycle(8, iFlamesTex, 10, "Flames");
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(BILLBOARD, "Flame", NULL, iFlamesTexCycle, cTV_3DVECTOR(5, 5, 0), cTV_3DVECTOR(8, 6, 0), true, NULL, NULL, NULL, World));
	cl3DObjMgr->VecObjects.back()->Mesh->SetBlendingMode(cTV_BLEND_ALPHA);
	cl3DObjMgr->VecObjects.back()->Mesh->SetAlphaTest(true);

	
	//Elevator
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "ElevatorFloor", NULL, iDamierTex, cTV_3DVECTOR(0, -10, 9), cTV_3DVECTOR(10, 0.5, 10), true, 0.5, PHYSICALL, ElevatorPhysID, World));
	clElevatorfloor = cl3DObjMgr->VecObjects.back();
	NewtonBodySetForceAndTorqueCallback(clElevatorfloor->pbody, ElevatorForceAndTorqueCallback);
	NewtonBodySetTransformCallback(clElevatorfloor->pbody, ElevatorTransformationCallback);
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "ElevatorFoot", NULL, iDamierTex, cTV_3DVECTOR(0, -12, 9), cTV_3DVECTOR(3, 1, 3), true, 0.5, PHYSICSTATIC, ElevatorPhysID, World));
	iElevatorGate = GATE0;


	iUpDown = 1; // 1 pour up, -1 pour down pour inversé le vector
	// we will Link the ElevatorFloor to a slider newtonjoint
	cTV_3DVECTOR position;
	dMatrix matrix;
	matrix = GetIdentityMatrix();

	position = clElevatorfloor->Mesh->GetPosition();
	matrix.m_posit.m_x = position.x;
	matrix.m_posit.m_y = position.y;
	matrix.m_posit.m_z = position.z;
	// specify the limits for defining a Hinge around the x axis
	dVector  SliderPos(0.0f, -10.0f, 9.0f);
	dVector  SliderDirection(0.0f, 2.0f, 3.0f);
	NewtonJoint* ElevatorSlider;// , *ElevatorFenceSlider;
	ElevatorSlider = NewtonConstraintCreateSlider(World, &SliderPos[0], &SliderDirection[0], clElevatorfloor->pbody, NULL);
	//ElevatorFenceSlider = NewtonConstraintCreateSlider(World, &SliderPos[0], &SliderDirection[0], clElevatorFence->pbody, NULL);

	// adding two Kinematic controlled object
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "TestKinematic", NULL, iYellowtwowaysignTex, cTV_3DVECTOR(2, 2, 2), cTV_3DVECTOR(0.5f, 0.5f, 0.5f), true, 2.0f, PHYSICALL, woodID, World));

	/*
	// we will Link the Door Body to the world with Hinge regenerated from a Generic 6DOF joint.
	cTV_3DVECTOR Doorposition;
	dMatrix doormatrix;
	doormatrix = GetIdentityMatrix();

	position = clDoor->GetMesh()->GetPosition();
	doormatrix.m_posit.m_x = Doorposition.x;// + 1.25f;
	doormatrix.m_posit.m_y = Doorposition.y + 0.5f;
	doormatrix.m_posit.m_z = Doorposition.z;
	// specify the limits for defining a Hinge around the x axis
	dVector  minLinearLimits (0.0f, 0.0f, 0.0f, 0.0f);
	dVector  maxLinearLimits (0.0f, 0.0f, 0.0f, 0.0f);
	dVector  minAngulaLimits ( 0.0f, 0.0f, -0.5f * 3.1416f, 0.0f);
	dVector  maxAngulaLimits ( 0.0f, 0.0f,  0.5f * 3.1416f, 0.0f);
	NewtonUserJoint* DoorHinge;
	// Create a 6DOF joint
	DoorHinge = CreateCustomJoint6DOF (&doormatrix[0][0], &doormatrix[0][0], clDoor->pbody, NULL);
	// set the hinge Limits
	CustomJoint6DOF_SetLinearLimits (DoorHinge, &minLinearLimits[0], &maxLinearLimits[0]);
	CustomJoint6DOF_SetAngularLimits (DoorHinge, &minAngulaLimits[0], &maxAngulaLimits[0]);
	*/
	
}
void CLBallZScene::Render(long lTimeOfDay)
{
	clAtmosphere->UpdateAndRender(lTimeOfDay,-1);  // Doit etre avant le rendering 2D sinon skybox recouvre les axes 
	clTV3D->DrawTV3DAxis();
	//pLand->Render();
	//int msx, msy;  cTV_3DVECTOR v1, v2;
	//clTV3D->pTVMaths->GetMousePickVectors(msx, msy, &v1, &v2);
	//clTV3D->DrawTV3DLine(v1, v2, GREEN_RGBA);
	cl3DObjMgr->Render();
	clAnneaux->Render();  // DOIT ETRE REVU...   les anneaux sont des 3DObjects mais pas gerer par le 3DObjManager a cause des particles...
	clTV3D->DrawSpline(clPlanePATH->GetSpline(), WHITE_RGBA);
	clFormation->Debug();
	clTV3D->pTVScene->FinalizeShadows();
	XMesh->Render();

	UpdateScene();
}
void CLBallZScene::UpdateScene(void)
{
	
	if (clCamera->bKeyboardNMouseCtrl == false){
	// Sert a ajuster la distance de la cam en fonction de la vitesse de la balle...   trop drastique pour l'instant.
	// Doit se servir du vecteur de direction de la balle pour définir la position de la cam...  genre vector/mirroir * 10...
	cTV_3DVECTOR AimVec, VelVec;
	dVector Velocity;
	AimVec = clBallZ->clShell->Mesh->GetPosition();
	clCamera->pCameraAim->SetPosition(AimVec.x, AimVec.y, AimVec.z);
	NewtonBodyGetVelocity(clBallZ->clShell->pbody, &Velocity.m_x);
	VelVec = cTV_3DVECTOR(-Velocity.m_x *0.7f, Velocity.m_y*0.7f, -Velocity.m_z*0.7f);
	//AimVec = clCamera->pCameraAim->GetPosition();
	//->SetCamera(VelVec.x,VelVec.y,VelVec.z,AimVec.x,AimVec.y,AimVec.z);
	clObjectNowhere->Mesh->SetPosition(VelVec.x, VelVec.y, VelVec.z);
	clCamera->SetChase(cTV_3DVECTOR(0, 0.3f, -0.15f), cTV_3DVECTOR(0, 0, 0), 50);
	}

	clVoieLactee->Update();
	clAnneaux->Update(NewtonBodyGetCollision(clBallZ->clShell->pbody), clBallZ->clShell->Mesh->GetMatrix());
	ManageInput();
	clBallZ->Update();

	int i = 0;
	// Pas CHIC, devrait touver une facon de lancer un msg des callback vers le message loop ou de koi du genre
	if (clBallZ->clShell->bIsFinishline){
		clBallZ->SetPosition(cTV_3DVECTOR(0, 0.1f, 0), cTV_3DVECTOR(0, 0, 0));
		clPlanePATH->SetSpeed(4);
		clBallZ->clShell->bIsFinishline = false;
		clDisplayer->iLap = clDisplayer->iLap + 1;
	}

	if ((clElevatorfloor->Mesh->GetPosition().y > 14) && (iUpDown == 1)) // valeur a revoir
		iUpDown = -1; // pour inversé le vector creant l'aller retour
	else if ((clElevatorfloor->Mesh->GetPosition().y < -10) && (iUpDown == -1))
		iUpDown = 1;

	if (clBallZ->clShell->bIsOnElevator)
	{
		int iScale = SCALE;

		cTV_3DVECTOR velo1 = cTV_3DVECTOR(0 * iScale * (float)iUpDown, 0 * iScale * (float)iUpDown, 3 * iScale * (float)iUpDown);
		//cTV_3DVECTOR velo2 = cTV_3DVECTOR(0 * iScale * (float)iUpDown, 2 * iScale * (float)iUpDown, 1 * iScale * (float)iUpDown);
		switch (iElevatorGate)
		{
		case GATE0:
			if (clElevatorfloor->Mesh->GetPosition().z < 40)
				NewtonBodySetVelocity(clElevatorfloor->pbody, (float*)&velo1);
			else iElevatorGate = GATE1;
			break;
		case GATE1:
			//NewtonBodySetVelocity(clElevatorfloor->pbody, (float*)&velo2);
			break;
		}
		clBallZ->clShell->bIsOnElevator = false;
	}
	
	clSound->Update();
	NewtonUpdate(World, (1 / 160) * clGlobalVar->fTimeElapsed);
}
void CLBallZScene::UpdateWater(long lTimeOfDay)
{
	clWater->StartReflectRender();
	clWater->StartRefractRender();
	//clBallZ->Render();   Nécessaire???
	cl3DObjMgr->Render();
	clAtmosphere->UpdateAndRender(lTimeOfDay);
	clWater->StopRefractRender();
	clWater->StopReflectRender();
	/*
	clWater->StartReflectRender();
	clWater->StartRefractRender();
	clBallZ->Render();
	cl3DObjMgr->Render();
	clLand->Render();
	clAtmosphere->UpdateAndRender(lTimeOfDay);
	clWater->StopRefractRender();
	clWater->StopReflectRender();
	clWater->Render();
	*/
}
void CLBallZScene::ManageInput(void)
{
	float Angle;
	cTV_3DVECTOR vec, vTorque, vNewtonTorque, jump, origin;

	Angle = clTV3D->pTVMaths->Direction2Ang((float)clInput->stGamePad.LeftX, (float)clInput->stGamePad.LeftY); // Y);
	vec = clBallZ->clCtrl->Mesh->GetRotation();
	clBallZ->clCtrl->Mesh->SetRotation(vec.x, Angle, vec.z);

	if (clInput->stGamePad.b1)
	{
		jump = cTV_3DVECTOR(0.0f, 0.1f, 0.1f);
		origin = cTV_3DVECTOR(0, 0, 0);
		//NewtonBodyAddImpulse(clShell->pbody, &jump.x, &origin.x);
		clBallZ->clShell->bJumping = true;
		bTest = 0;
	}
	if (clInput->stGamePad.b0)
	{
		clBallZ->fThrottle = clBallZ->fThrottle + clBallZ->fThrottleStep;
		if (clBallZ->fThrottle > clBallZ->fMaxThrottle)
			clBallZ->fThrottle = clBallZ->fMaxThrottle;
	}
	else {
		clBallZ->fThrottle = clBallZ->fThrottle - clBallZ->fThrottleStep;
		if (clBallZ->fThrottle < 0)
			clBallZ->fThrottle = 0;
	}

	// Détermine le vector de torque en fonction de la position du joystick
	// Ptete ajouter une petite valeur au lieu de 0 pour palier au mini fluctuation de joystick.
	if ((clInput->stGamePad.LeftX != 0) || (clInput->stGamePad.LeftY != 0)) //	Y = clInput->stGamePad.LeftY * -1;  //  on ne prend pas compte si le joystick pointe vers le bas
	{
		vTorque.x = (float)clInput->stGamePad.LeftX;
		vTorque.y = 0;
		vTorque.z = (float)clInput->stGamePad.LeftY;
		// Valider si Y reste tjrs a 0 apres la normalisation... sinon ajuster directement la valeur recu du gamepad.
		vTorque = clTV3D->pTVMaths->VNormalize(&vTorque);
		// Tweak pour setter le torque dans le bon sens... car Newton et TV3D ne semble pas avoir le meme systeme d'Axe
		vNewtonTorque.x = vTorque.z;
		vNewtonTorque.y = vTorque.y;
		vNewtonTorque.z = -vTorque.x;
	}

	clBallZ->clShell->vCtrlTorque.x = vNewtonTorque.x * clBallZ->fThrottle;
	clBallZ->clShell->vCtrlTorque.y = vNewtonTorque.y * clBallZ->fThrottle;
	clBallZ->clShell->vCtrlTorque.z = vNewtonTorque.z * clBallZ->fThrottle;

	if (clInput->stKeyboard.Pressedkey == cTV_KEY_B)
	{
		//clSound->PlaySound(3);
	}
	if (clInput->stGamePad.b2 || clInput->stKeyboard.Pressedkey == cTV_KEY_C)
	{
		if (clCamera->bKeyboardNMouseCtrl)
			clCamera->bKeyboardNMouseCtrl = false;
		else
			clCamera->bKeyboardNMouseCtrl = true;
	}
	if (clInput->stKeyboard.Pressedkey == cTV_KEY_R)
	{
		cTV_3DVECTOR pos = clBallZ->clShell->Mesh->GetPosition();
		XmlSplineAddNode(pos, "Media\\Anneaux.xml");
	}
	if (clInput->stKeyboard.Pressedkey == cTV_KEY_S)
	{
		clTV3D->pTV->Screenshot("ScreenShot.png", cTV_IMAGE_PNG);
	}



	if (pMeshClicked)
	{
		//if (clInput->stKeyboard.Pressedkey == cTV_KEY_LEFTSHIFT)
		cTV_3DVECTOR GrabbedPosition;
		cTV_3DVECTOR GrabbedDestination;
		cTV_3DVECTOR GrabbedDirection;

		int iMeshIndex = pMeshClicked->GetEntityIndex();
		CL3DObject* Clicked3D = cl3DObjMgr->Find3DObjectWithMesh(pMeshClicked);
		if (Clicked3D == NULL)
			MouseDrop();
		else if (clInput->stMouse.B1 == true)
		{
			CTVCollisionResult* pMouseMoveCollision = NULL;
			pMouseMoveCollision = clTV3D->pTVScene->MousePick(clInput->stMouse.PosX, clInput->stMouse.PosY, cTV_OBJECT_LANDSCAPE, cTV_TESTTYPE_ACCURATETESTING);
			if (pMouseMoveCollision->IsCollision())
			{
				
				cTV_3DVECTOR dVector;
				// Give object a direction
				GrabbedDestination = pMouseMoveCollision->GetCollisionImpact();
				GrabbedPosition = pMeshClicked->GetPosition();
				
				clTV3D->pTVMaths->TVVec3Subtract(&dVector, &GrabbedDestination, &GrabbedPosition);
				clTV3D->pTVMaths->TVVec3Normalize(&dVector, &dVector);
				GrabbedDirection = dVector;
				Clicked3D->Mesh->LookAtPoint(&GrabbedDestination, true);
				/*
				// Update the Flyer's angle
				if (FlyerDirection[intFlyerSelected].z > 0)
					FlyerAngleY[intFlyerSelected] = clTV3D->pTVMaths->Rad2Deg(clTV3D->pTVMaths->ATan((double)FlyerDirection[intFlyerSelected].x / FlyerDirection[intFlyerSelected].z));
				else
					FlyerAngleY[intFlyerSelected] = clTV3D->pTVMaths->Rad2Deg(clTV3D->pTVMaths->ATan((double)FlyerDirection[intFlyerSelected].x / FlyerDirection[intFlyerSelected].z)) + 180;
				*/
				// Set the Grabbed mesh rotation
				//Flyer[intFlyerSelected]->SetPosition(Flyer[intFlyerSelected]->Mesh->GetPosition(), cTV_3DVECTOR(0, FlyerAngleY[intFlyerSelected], 0));
				
			}
		}
		// position by adding a scale of the vector destination.
		if ((clTV3D->pTVMaths->GetDistance3D(GrabbedDestination.x, 0, GrabbedDestination.z, GrabbedPosition.x, 0, GrabbedPosition.z)) > 2)
	{
			// Update the Flyer's position
			cTV_3DVECTOR dV2;
			clTV3D->pTVMaths->TVVec3Scale(&dV2, &GrabbedDirection, ((float)clTV3D->pTV->TimeElapsed() * 0.1f));
			clTV3D->pTVMaths->TVVec3Add(&GrabbedPosition, &GrabbedPosition, &dV2);
			// Update the Flyer's mesh position
			Clicked3D->SetPosition(GrabbedPosition, Clicked3D->Mesh->GetRotation());
	}
}
}

void CLBallZScene::MouseGrab(void)
{
		
	pClickCollision = NULL;
	pOldMeshClicked = pMeshClicked;  pMeshClicked = NULL;

	pClickCollision = clTV3D->pTVScene->MousePick(clInput->stMouse.PosX, clInput->stMouse.PosY, cTV_OBJECT_MESH, cTV_TESTTYPE_ACCURATETESTING);
	if (pClickCollision->IsCollision())
	{
		pMeshClicked = pClickCollision->GetCollisionMesh();
		pMeshClicked->SetColor(GREEN_RGBA, false);
		cTV_3DVECTOR Pos = pMeshClicked->GetPosition();
		pLand->SetPosition(Pos.x - 1024, Pos.y, Pos.z - 1024);
	}
	/*
	int W, H, BP;
	clTV3D->pTV->GetVideoMode(&W, &H, &BP);
	CTVCollisionResult* Pick = clTV3D->pTVScene->MousePick(W / 2, H / 2, cTV_OBJECT_MESH, cTV_TESTTYPE_ACCURATETESTING);
	*/
}


void CLBallZScene::ClickedOnMesh(void)
{
	int MovementCycleCount;
	cTV_3DVECTOR MeshDirectionMoveVector1, MeshCollisionRefPos1;

	cTV_3DVECTOR PickedMeshPos1 = clInput->pMeshClicked->GetPosition();
	cTV_3DVECTOR MeshCollisionPos1 = pClickCollision->GetCollisionImpact();

	cTV_3DVECTOR CameraPos1 = clCamera->pCamera->GetPosition();
	/*
	clTV3D->pTVMaths->TVVec3Subtract(&MeshDirectionMoveVector1, &MeshCollisionPos1, &MeshCollisionRefPos1);
	MeshCollisionRefPos1 = MeshCollisionPos1;

	// need 2 cycles with no movement by mouse
	if (MovementCycleCount >= 1)
	{
	if (pMeshClicked){
		pMeshClicked->SetColor(WHITE_RGBA);
		pMeshClicked = NULL;
	}
	MovementCycleCount++;
	if (MovementCycleCount > 2) MovementCycleCount = 2;
	*/

	clInput->pMeshClicked->MoveRelative(clInput->stMouse.DeltaPosX, 0, clInput->stMouse.DeltaPosY);


}
void CLBallZScene::CreateBoxTrianglePile(float fBoxSize, cTV_3DVECTOR pos, int iTexture)
{
	vecPosition3D Triangle = clTV3D->CreateTrianglePile(6, pos, fBoxSize); // cTV_3DVECTOR(1, -7.5f, 6)  cTV_3DVECTOR(0, 0, 0)
	int i = 0;
	for each(cTV_3DVECTOR vec in Triangle)
	{
		i++; // trouver comment ajouter le i dans la string "Triangle" !!!!!!!!!!!!!!!!!!!!!!!!  <-
		cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "Triangle", NULL, iTexture, vec, cTV_3DVECTOR(fBoxSize, fBoxSize, fBoxSize), true, 0.075f, PHYSICALL, woodID, World));
		
	}
}

