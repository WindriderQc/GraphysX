#include "stdafx.h"
#include "3DScenes.h"

static void ElevatorForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
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
static void ElevatorTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	cTV_3DMATRIX* MX = new cTV_3DMATRIX();
	NewtonBodyGetMatrix(body, MX->m[0]);
	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	cl3DObject->Mesh->SetMatrix(MX);
	delete(MX); MX = NULL;
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
	char* name = cl3DObject->Mesh->GetMeshName();
	if ( strcmp(name,"BallZShell"))
		cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body1);

	cl3DObject->bAllowJumps = true;
	cl3DObject->bIsOnElevator = true;
}

//  FAIRE LE CLEAN UP ET MIEUX POPULER SCENE.H POUR QUE ACTIVESCENE soit utilisable a partir de Area... (mousegrab, etc..)

CLBallZScene::CLBallZScene() : CLScene()
{
	pOldMeshClicked = NULL;
	pMeshClicked = NULL;
	Mouse3DPos = cTV_3DVECTOR(0,0,0);
	/*
	if (!clSceneStaff->clInput->bGamePadConnected)
			clSceneStaff->clCamera->bKeyboardNMouseCtrl = true;
	*/
	//clSound->PlaySound(clSound->iIntro);
	
	clSceneStaff->clTV3D->pTVScene->SetRenderMode(cTV_SOLID); //);cTV_LINE
	clSceneStaff->clTV3D->pTVScene->SetShadowParameters(RGBA(0.0, 0.0, 0.0, 0.5), false);
	clSceneStaff->clTV3D->pTVScene->SetAutoTransColor(cTV_COLORKEY_USE_ALPHA_CHANNEL);// cTV_COLORKEY_MAGENTA);//
	SetPhysicMaterials();
	LoadTextures();

	// We set a mouse pointer
	//clTV3D->pTVScene->SetCursor(true, clTV3D->pTVTexturefactory->LoadTexture("Media\\pointer.bmp", "CursorTexture", -1, -1, cTV_COLORKEY_BLACK, true));
	// We set background color.
	//clTV3D->pTVScene->SetBackgroundColor(0.0f, 0.3f, 0.9f);
	clAtmosphere = new CLAtmosphere(stSkyDay, stSkyNight);
	//clWater = new CLWater();
	//clLand = new CLLand(""media\\\\Heightmaps\\Height.jpg", "Media\\Grass.jpg", 10, 10, cTV_3DVECTOR(-768, -180, -512), World);

	// The land generation    //   ACTUALLY only used for the mouse picking
	pLand = new CTVLandscape();
	pLand = clSceneStaff->clTV3D->pTVScene->CreateLandscape("Land");
	pLand->CreateEmptyTerrain(cTV_PRECISION_LOW, 8, 8, -1024, -100, -1024);
	// Generate the height of the land from the grayscale of the image.
	//pLand->GenerateTerrain("Media\\Heightmaps\\Track.jpg", cTV_PRECISION_LOW, 8, 8, -1024,-100, -1024, true);
	// We assign a texture to that land.
	pLand->SetTexture(iDamierTex);
	pLand->Enable(false);
	
	clActorFactory = CLActorFactory::getInstance();
	clAnimator = new CLAnimator();

	SetupScene();

	clAnneaux = new CLAnneaux(World, IDParticleTex, YELLOW_RGBA);
	clVoieLactee = new CLVoieLactee(World);
	//clSound->PlaySound(clSound->iPiano);
}
CLBallZScene::~CLBallZScene(void)
{
	if (clAtmosphere)     delete(clAtmosphere);     clAtmosphere = NULL;
	if (clAnneaux)        delete(clAnneaux);	    clAnneaux = NULL;
	if (clBallZ)          delete(clBallZ);		    clBallZ = NULL;
	clSceneStaff->cl3DObjMgr->kill();
	clSceneStaff->clSound->kill();
}
void CLBallZScene::SetPhysicMaterials(void)
{
	// create all materials ID
	cout << "Physics Materials Qty : " << sizeof(iPhysicMaterials) << " / " << sizeof(ePHYSMAT);
	iPhysicMaterials[WOOD] = NewtonMaterialCreateGroupID(World);
	iPhysicMaterials[FINISH] = NewtonMaterialCreateGroupID(World);
	iPhysicMaterials[LEVEL] = NewtonMaterialCreateGroupID(World);
	iPhysicMaterials[BALL] = NewtonMaterialCreateGroupID(World);
	iPhysicMaterials[ELEVATOR] = NewtonMaterialCreateGroupID(World);
	// create the character material interactions
	// set the material properties for character on Wood
	NewtonMaterialSetDefaultElasticity(World, iPhysicMaterials[WOOD], iPhysicMaterials[BALL], 0.3f);
	NewtonMaterialSetDefaultFriction(World, iPhysicMaterials[WOOD], iPhysicMaterials[BALL], 1.1f, 0.8f);
	NewtonMaterialSetCollisionCallback(World, iPhysicMaterials[WOOD], iPhysicMaterials[BALL], NULL, NULL, GenericContactProcess);
	// set the material properties for character on Controllable plate
	NewtonMaterialSetDefaultElasticity(World, iPhysicMaterials[FINISH], iPhysicMaterials[BALL], 0.1f);
	NewtonMaterialSetDefaultFriction(World, iPhysicMaterials[FINISH], iPhysicMaterials[BALL], 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, iPhysicMaterials[FINISH], iPhysicMaterials[BALL], NULL, NULL, FinishLineContactProcess);
	// set the material properties for character on level 
	NewtonMaterialSetDefaultElasticity(World, iPhysicMaterials[LEVEL], iPhysicMaterials[BALL], 0.3f);
	NewtonMaterialSetDefaultFriction(World, iPhysicMaterials[LEVEL], iPhysicMaterials[BALL], 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, iPhysicMaterials[LEVEL], iPhysicMaterials[BALL], NULL, NULL, GenericContactProcess);
	// set the material properties for Wood on Floor 
	NewtonMaterialSetDefaultElasticity(World, iPhysicMaterials[LEVEL], iPhysicMaterials[WOOD], 0.3f);
	NewtonMaterialSetDefaultFriction(World, iPhysicMaterials[LEVEL], iPhysicMaterials[WOOD], 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, iPhysicMaterials[LEVEL], iPhysicMaterials[WOOD], NULL, NULL, GenericContactProcess);
	/*// set the material properties for character on Elevator
	NewtonMaterialSetDefaultElasticity(World, iPhysicMaterials[ELEVATOR], iPhysicMaterials[WOOD], 0.3f);
	NewtonMaterialSetDefaultFriction(World, iPhysicMaterials[ELEVATOR], iPhysicMaterials[WOOD], 0.8f, 0.99f);
	NewtonMaterialSetCollisionCallback(World, iPhysicMaterials[ELEVATOR], iPhysicMaterials[WOOD], NULL, NULL, ElevatorContactProcess);
	*/
	cout << "Physics Materials loaded : " << iPhysicMaterials[WOOD] << " " << iPhysicMaterials[FINISH] << " " << iPhysicMaterials[LEVEL] << endl;
}
void CLBallZScene::LoadTextures(void)
{
	//DEVRAIT ETRE UN VECTOR DE INT OU UN ENUM OU WHATEVER AUTRE QUE CA! :)
	iYellowtwowaysignTex = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\yellowtwoway.jpg", "TwoWay");
	iGridTex = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\GreenGrig.png", "Grid");
	iDamierTex = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Damier.jpg", "Damier");
	i3D_Spheres = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\3D_Spheres.jpg", "3D_Spheres");
	iMagicienTex = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Zack.jpg", "Zack");
	iAbstractCubes = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\AbstractCubes.jpg", "AbstractCubes");
	//itexGrass = pTVTexturefactory->LoadTexture("Media\\grass.jpg", "Grass");
	//Load our particle texture.
	IDParticleTex = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\p.dds", "particle tex", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);
	
	//Sky Box
	stSkyDay.SkyTop = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\up.jpg", "SkyTop");
	stSkyDay.SkyBottom = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\down.jpg", "SkyBottom");
	stSkyDay.SkyLeft = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\left.jpg", "SkyLeft");
	stSkyDay.SkyRight = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\right.jpg", "SkyRight");
	stSkyDay.SkyFront = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\front.jpg", "SkyFront");
	stSkyDay.SkyBack = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("Media\\Sky\\SkyX\\back.jpg", "SkyBack");
	stSkyNight.SkyTop = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\up.jpg", "NightSkyTop", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyBottom = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\down.jpg", "NightSkyBottom", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyLeft = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\left.jpg", "NightSkyLeft", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyRight = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\right.jpg", "NightSkyRight", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyFront = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\front.jpg", "NightSkyFront", -1, -1, cTV_COLORKEY_NO, true);
	stSkyNight.SkyBack = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("media\\sky\\clearnight\\back.jpg", "NightSkyBack", -1, -1, cTV_COLORKEY_NO, true);
}
void CLBallZScene::SetupScene(void)
{
	std::cout << "Newton Version : " << NewtonWorldGetVersion();
	/*
	vecPosition3D PosBuissons;
	PosBuissons = { cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(2, 0, 4), cTV_3DVECTOR(4, 0, 0) };   //cTVXmlSplineRead("Media\\Anneaux.xml");		
	for each(cTV_3DVECTOR vec in PosBuissons)
	{
		cl3DObjMgr->VecXMesh.push_back(XMesh->Duplicate());
		cl3DObjMgr->VecXMesh.back()->SetPosition(vec.x, vec.y, vec.z);
	}
	*/
	//MeshFactory::getInstance()->CreatePhysicMeshCustom("Media\\Airplane\\AirplaneLP.TVM", NULL, cTV_3DVECTOR(3, 15, 0), cTV_3DVECTOR(1, 1, 1), "Airplane", true, 0.5, woodID, World, true);

	CTVMesh *TestText = clSceneStaff->clTV3D->pTVScene->CreateMeshBuilder();
	TestText->Create3DText("POS: 0, 10, 0", "Times New Roman", 32, 0.2f);
	TestText->SetLightingMode(cTV_LIGHTING_MANAGED);
	TestText->SetColor(BLUE_RGBA);
	TestText->SetBlendingMode(cTV_BLEND_ALPHA);
	TestText->SetPosition(0, 10, 0);

	/*
	CREATE 3D OBJECTS
	*/

	//  Flyer Zone
	int NbrFlyers = 8;
	
	clFormation = new CLFormation(NbrFlyers, 2, false);
	CreateFormation(NbrFlyers);

	///////////////////////////////////////////

	
	clBallZ = new CLBallZ(World, cTV_3DVECTOR(-2.0f, 1.5f, -2.0f), 0.1f, 5, iPhysicMaterials[BALL]); // masse = 1, radius = 0.02
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(clBallZ->clCtrl);
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(clBallZ->clShell);
	//clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "RefCube", NULL, iYellowtwowaysignTex, cTV_3DVECTOR(-1, 0.5, 3), cTV_3DVECTOR(1, 1, 1), true, 2.0f, PHYSICALL, iPhysicMaterials[WOOD], World));
	//clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Finish", "Media\\Finish.TVM", iDamierTex, cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(0, 0, 0), true, NULL, PHYSICSTATIC, iPhysicMaterials[FINISH], World));
	for (int j = 0; j < 10; j++)
	{
		string name("Box" + to_string((long double)j));
		clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, &name[0], NULL, i3D_Spheres, cTV_3DVECTOR(3, 0.02f + j, 2.0f), cTV_3DVECTOR(0.1, 0.1,0.1), true, 0.075f, PHYSICALL, iPhysicMaterials[WOOD], World));
	}
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(BILLBOARD, "Zack", NULL, iMagicienTex, cTV_3DVECTOR(-20, 0, -30), cTV_3DVECTOR(8, 6, 0), true, NULL, NULL, NULL, World));

	CreateBoxTrianglePile(0.1f, cTV_3DVECTOR(0,0.05f,0.1f), iMagicienTex);
	

	// Shader test
	IDDiffuse = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("..\\BallZ2015\\Media\\tv3dlogo_d.dds", "diffuse texture", -1, -1, cTV_COLORKEY_NO, true);
	IDNormal = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture("..\\BallZ2015\\Media\\cubenormal.png", "Bump texture", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);
	fBumpAmount = 0.01f;// 001f;
	m_Shader = clSceneStaff->clTV3D->pTVScene->CreateShader("Objet_simple");
	m_Shader->CreateFromEffectFile("..\\BallZ2015\\Media\\shaders\\ppl.shade");
	m_Shader->SetEffectParamVector3("LPos", &Vector3(0, 0, -10));      //Give the shader a 3-vector, the shader will take this as a light position.
	m_Shader->SetEffectParamFloat("BumpAmount", fBumpAmount);
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "ShadedCube", NULL, IDDiffuse, cTV_3DVECTOR(-10, 10, 10), cTV_3DVECTOR(10, 10, 10), true, 0.0f, PHYSICSTATIC, iPhysicMaterials[WOOD], World));
	clTestBox = clSceneStaff->cl3DObjMgr->VecObjects.back();
	m_Shader->SetEffectParamTexture("Normal", IDNormal); //Set our bump map so the shader can find it. You can do this via semantics, but I wanted to demonstrate SetEffectParam etc.
	//Apply the shader to the mesh.
	clTestBox->Mesh->SetShader(m_Shader);
	//Tell TV we want tagent information etc.
	clTestBox->Mesh->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE, -1, -1);


	//  TO DO:  UNE METHOD QUI PERMET DE CREER DES BILLBOARD EFFECTS

	//  CHECKER LE common.shader dans le répoertoire des flames...  il te manque de koi a faire le grand! ;)
	int iFlamesTex[8];
	int iFlamesTexCycle;
	for (int i = 0; i < 8; i++)
	{
		string path = "..\\BallZ2015\\Media\\Flames\\Flame";
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
		iFlamesTex[i] = clSceneStaff->clTV3D->pTVTexturefactory->LoadTexture(FullPath, FullName, -1, -1, cTV_COLORKEY_BLACK);
	}
	iFlamesTexCycle = clSceneStaff->clTV3D->pTVTexturefactory->CreateTextureCycle(8, iFlamesTex, 10, "Flames");
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(BILLBOARD, "Flame", NULL, iFlamesTexCycle, cTV_3DVECTOR(5, 5, 0), cTV_3DVECTOR(8, 6, 0), true, NULL, NULL, NULL, World));
	clSceneStaff->cl3DObjMgr->VecObjects.back()->Mesh->SetBlendingMode(cTV_BLEND_ALPHA);
	clSceneStaff->cl3DObjMgr->VecObjects.back()->Mesh->SetAlphaTest(true);

	
	//Elevator
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "ElevatorFloor", NULL, iDamierTex, cTV_3DVECTOR(0, -10, 9), cTV_3DVECTOR(10, 0.5, 10), true, 0.5, PHYSICALL, iPhysicMaterials[ELEVATOR], World));
	clElevatorfloor = clSceneStaff->cl3DObjMgr->VecObjects.back();
	NewtonBodySetForceAndTorqueCallback(clElevatorfloor->pbody, ElevatorForceAndTorqueCallback);
	NewtonBodySetTransformCallback(clElevatorfloor->pbody, ElevatorTransformationCallback);
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "ElevatorFoot", NULL, iDamierTex, cTV_3DVECTOR(0, -12, 9), cTV_3DVECTOR(3, 1, 3), true, 0.5, PHYSICSTATIC, iPhysicMaterials[ELEVATOR], World));
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
	NewtonJoint* ElevatorSlider;
	ElevatorSlider = NewtonConstraintCreateSlider(World, &SliderPos[0], &SliderDirection[0], clElevatorfloor->pbody, NULL);

	// adding two Kinematic controlled object
	clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "TestKinematic", NULL, iYellowtwowaysignTex, cTV_3DVECTOR(2, 2, 2), cTV_3DVECTOR(0.5f, 0.5f, 0.5f), true, 2.0f, PHYSICALL, iPhysicMaterials[WOOD], World));

	
	
}
void CLBallZScene::AddInteractions(void)
{
//cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUSTOM, "Airplane", "Media\\Airplane\\AirplaneLP.TVM", NULL, cTV_3DVECTOR(3, 15, 0), cTV_3DVECTOR(1, 1, 1), true, 0.5, NULL, NULL, World));
vecPosition3D Spline = XmlSplineRead("..\\BallZ2015\\Media\\Spline.xml");// { cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(10, 20, 0) };

CLMesh* Airplane = MeshFactory::getInstance()->Find3DObjectWithName("Airplane");
if (Airplane){
	clPlanePATH = new CLSplineFollow(Airplane->Mesh, Spline, true);
	clPlanePATH->SetSpeed(4);
}
else MSGBOX("Airplane", "Erreur Interaction");

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


void CLBallZScene::Render(void)
{
	long lTimeOfDay = clSceneStaff->clClock->getTimeOfDay();

	//  Render Water surfaces
	//pActiveScene->UpdateWater(clClock->getTimeOfDay());
	
	//START Rendering
	clSceneStaff->clTV3D->pTV->Clear(false);
	
	clAtmosphere->UpdateAndRender(lTimeOfDay, -1);  // Doit etre avant le rendering 2D sinon skybox recouvre les axes 
	clSceneStaff->clTV3D->DrawTV3DAxis();
	clSceneStaff->clTV3D->pTVScene->RenderAll(true);
	clSceneStaff->clTV3D->pTVScene->FinalizeShadows();
	
	clSceneStaff->clDisplayer->Render(fBumpAmount); // BUMP and IDFont from Shader bumpmapping cube test
	MouseHighlight();
	
	UpdateScene();

	clSceneStaff->clTV3D->pTV->RenderToScreen();
	//END Rendering

	
}
void CLBallZScene::UpdateScene(void)
{
	ManageInput();
	ManageMeshClicked();

	cTV_3DVECTOR BallPos = clBallZ->clShell->Mesh->GetPosition();

	cTV_3DVECTOR campos = BallPos + (BallPos - Mouse3DPos);
	clSceneStaff->clTV3D->DrawTV3DLine(BallPos, campos, YELLOW_RGBA);
	/*
	clTV3D->pTVMaths->TVVec3Normalize(&campos, &campos);
	clTV3D->pTVMaths->TVVec3Scale(&campos, &campos, 2);
	*/
	//clCamera->pCamera->SetPosition(campos.x, 3, campos.z);
	//ADAPTER LE ENDPOS POUR QUE CA SUIVE LA VELOCITY DE LA BALLE
	clSceneStaff->clCamera->SetChase(clBallZ->clShell->Mesh, cTV_3DVECTOR(0, 2, 0), cTV_3DVECTOR(0, 0, 0), 0.5);
	
	//clCamera->pCamera->SetLookAt(BallPos.x, BallPos.y, BallPos.z);
	clSceneStaff->clCamera->ManageCamCtrl(clSceneStaff->clGlobalVar->fTimeElapsed);

	/*
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
	//clObjectNowhere->Mesh->SetPosition(VelVec.x, VelVec.y, VelVec.z);
	clCamera->SetChase(cTV_3DVECTOR(0, 0.75f, -0.5f), cTV_3DVECTOR(0,0,0), 50);
	}*/
	
	int i = 0;
	// Pas CHIC, devrait touver une facon de lancer un msg des callback vers le message loop ou de koi du genre
	if (clBallZ->clShell->bIsFinishline){
		//clBallZ->SetPosition(cTV_3DVECTOR(0, 0.1f, 0), cTV_3DVECTOR(0, 0, 0));
		clPlanePATH->SetSpeed(4);
		clBallZ->clShell->bIsFinishline = false;
		clSceneStaff->clDisplayer->iLap++;
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
	clAnneaux->Update(NewtonBodyGetCollision(clBallZ->clShell->pbody), clBallZ->clShell->Mesh->GetMatrix());
	clBallZ->Update();
	clVoieLactee->Update();

	clSceneStaff->clSound->Update();
	CLMesh* Airplane = MeshFactory::getInstance()->Find3DObjectWithName("Airplane");
	if (Airplane) Airplane->Debug();
	//MeshFactory::getInstance()->NewtonDebug();
	clBallZ->clShell->NewtonDebug(false,true);
	//clFormation->Debug();
	//clTV3D->DrawSpline(clPlanePATH->GetSpline(), WHITE_RGBA);
	for each(sLine3D line in vec3DVecToDraw)
	{
		clSceneStaff->clTV3D->DrawTV3DLine(line.Start, line.End, GREEN_RGBA);
	}
	vec3DVecToDraw.clear();


	clTestBox->Rotate(cTV_3DVECTOR(0, 1, 0), 0.25);
	/*CLMesh* Rotator = MeshFactory::getInstance()->Find3DObjectWithName("Suzanne1.RotatorCube");
	if (Rotator) {
		clAnimator->Rotate(Rotator, cTV_3DVECTOR(0, 5, 0), 0.1);
		Rotator->Debug();
	}
	else MSGBOX("Rotator", "Erreur UpdateScene");*/
	
	UpdatePhysics(clSceneStaff->clGlobalVar->fTimeElapsed);
	//NewtonUpdate(World, (1 / 160) * clGlobalVar->fTimeElapsed);
	//NewtonUpdate(World, (1 / 240000));
	clSceneStaff->clClock->update(clSceneStaff->clGlobalVar->fTimeElapsed);
}
void CLBallZScene::UpdateWater(long lTimeOfDay)
{
	clWater->StartReflectRender();
	clWater->StartRefractRender();
	//clBallZ->Render();   Nécessaire???
	clSceneStaff->cl3DObjMgr->Render();
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

		Angle = clSceneStaff->clTV3D->pTVMaths->Direction2Ang((float)clSceneStaff->clInput->stGamePad.LeftX, (float)clSceneStaff->clInput->stGamePad.LeftY); // Y);
		vec = clBallZ->clCtrl->Mesh->GetRotation();
		clBallZ->clCtrl->Mesh->SetRotation(vec.x, Angle, vec.z);

		if (clSceneStaff->clInput->stGamePad.b1 || clSceneStaff->clInput->KEY_PRESSED[cTV_KEY_SPACE])
		{
			jump = cTV_3DVECTOR(0.0f, 0.1f, 0.1f);
			origin = cTV_3DVECTOR(0, 0, 0);
			//NewtonBodyAddImpulse(clShell->pbody, &jump.x, &origin.x);
			clBallZ->clShell->bJumping = true;
			bTest = 0;
		}
		/*
		if (clInput->stGamePad.b0 || clInput->KEY_PRESSED[cTV_KEY_Z])
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
		*/
		// Détermine le vector de torque en fonction de la position du joystick
		// Ptete ajouter une petite valeur au lieu de 0 pour palier au mini fluctuation de joystick.
	/*	if ((clInput->stGamePad.LeftX != 0) || (clInput->stGamePad.LeftY != 0)) //	Y = clInput->stGamePad.LeftY * -1;  //  on ne prend pas compte si le joystick pointe vers le bas
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
			sLine3D line;
			line.Start = clBallZ->clShell->Mesh->GetPosition();
			line.End = cTV_3DVECTOR(line.Start.x + vTorque.x, line.Start.y + vTorque.y, line.Start.z + vTorque.z);
			vec3DVecToDraw.push_back(line);
		}*/
		if (clSceneStaff->clInput->IsMouseButtonPressed(0)){
			cTV_3DVECTOR BallPos = clBallZ->clShell->Mesh->GetPosition();
			cTV_3DVECTOR BallDirection = Mouse3DPos - BallPos;
			clSceneStaff->clTV3D->DrawTV3DLine(BallPos, Mouse3DPos, YELLOW_RGBA);


			vTorque.x = BallDirection.x;
			vTorque.y = 0;
			vTorque.z = BallDirection.z;
			// Valider si Y reste tjrs a 0 apres la normalisation... sinon ajuster directement la valeur recu du gamepad.
			vTorque = clSceneStaff->clTV3D->pTVMaths->VNormalize(&vTorque);
			// Tweak pour setter le torque dans le bon sens... car Newton et TV3D ne semble pas avoir le meme systeme d'Axe
			vNewtonTorque.x = vTorque.z;
			vNewtonTorque.y = vTorque.y;
			vNewtonTorque.z = -vTorque.x;

			clBallZ->fThrottle = clBallZ->fThrottle + clBallZ->fThrottleStep;
			if (clBallZ->fThrottle > clBallZ->fMaxThrottle)
				clBallZ->fThrottle = clBallZ->fMaxThrottle;
		}
		else {
			clBallZ->fThrottle = clBallZ->fThrottle - clBallZ->fThrottleStep;
			if (clBallZ->fThrottle < 0)
				clBallZ->fThrottle = 0;
		}

	/*

			vTorque.x = Mouse3DPos.x;
			vTorque.y = 0;
			vTorque.z = Mouse3DPos.z;
			// Valider si Y reste tjrs a 0 apres la normalisation... sinon ajuster directement la valeur recu du gamepad.
			vTorque = clTV3D->pTVMaths->VNormalize(&vTorque);
			// Tweak pour setter le torque dans le bon sens... car Newton et TV3D ne semble pas avoir le meme systeme d'Axe
			vNewtonTorque.x = vTorque.z;
			vNewtonTorque.y = vTorque.y;
			vNewtonTorque.z = -vTorque.x;
			sLine3D line;
			line.Start = clBallZ->clShell->Mesh->GetPosition();
			line.End = vNewtonTorque;
			vec3DVecToDraw.push_back(line);

			clBallZ->fThrottle = clBallZ->fThrottle + clBallZ->fThrottleStep;
			if (clBallZ->fThrottle > clBallZ->fMaxThrottle)
				clBallZ->fThrottle = clBallZ->fMaxThrottle;
		}
		else {
			clBallZ->fThrottle = clBallZ->fThrottle - clBallZ->fThrottleStep;
			if (clBallZ->fThrottle < 0)
				clBallZ->fThrottle = 0;
		}*/
		clBallZ->clShell->vCtrlTorque.x = vNewtonTorque.x * clBallZ->fThrottle;
		clBallZ->clShell->vCtrlTorque.y = vNewtonTorque.y * clBallZ->fThrottle;
		clBallZ->clShell->vCtrlTorque.z = vNewtonTorque.z * clBallZ->fThrottle;
		

		if (clSceneStaff->clInput->stKeyboard.Pressedkey == cTV_KEY_R)
	{
		cTV_3DVECTOR pos = clBallZ->clShell->Mesh->GetPosition();
		XmlSplineAddNode(pos, "Media\\Anneaux.xml");
	}
	

	if (clSceneStaff->clInput->stKeyboard.Pressedkey == cTV_KEY_B)
	{
		//clSound->PlaySound(3);
	}
	if (clSceneStaff->clInput->stGamePad.b2 || clSceneStaff->clInput->stKeyboard.Pressedkey == cTV_KEY_C)
	{
		if (clSceneStaff->clCamera->bKeyboardNMouseCtrl)
			clSceneStaff->clCamera->bKeyboardNMouseCtrl = false;
		else
			clSceneStaff->clCamera->bKeyboardNMouseCtrl = true;
	}
	if (clSceneStaff->clInput->stKeyboard.Pressedkey == cTV_KEY_S)
	{
		clSceneStaff->clTV3D->pTV->Screenshot("ScreenShot.png", cTV_IMAGE_PNG); //  + CLClock::getInstance()->getTimeOfDay()   Car ca overwrite maintenant...
	}

	//Increase the bump amount in the shader if the "1" key is pressed.
	if (clSceneStaff->clInput->stKeyboard.Pressedkey == (cTV_KEY_1))
	{
		fBumpAmount += 0.00001f * clSceneStaff->clGlobalVar->fTimeElapsed;
		m_Shader->SetEffectParamFloat("BumpAmount", fBumpAmount);
	}

	//Decrease the bump amount in the shader if the "2" key is pressed.
	if (clSceneStaff->clInput->stKeyboard.Pressedkey == (cTV_KEY_2))
	{
		fBumpAmount -= 0.00001f * clSceneStaff->clGlobalVar->fTimeElapsed;
		m_Shader->SetEffectParamFloat("BumpAmount", fBumpAmount);
	}

}
void CLBallZScene::ManageMeshClicked(void)
{
	if (pMeshClicked)
	{
		pOldMeshClicked = pMeshClicked;
		cTV_3DVECTOR GrabbedPosition;
		cTV_3DVECTOR GrabbedDestination;
		cTV_3DVECTOR GrabbedDirection;

		int iMeshIndex = pMeshClicked->GetEntityIndex();
		CLMesh* Clicked3D = MeshFactory::getInstance()->Find3DObjectWithTVMesh(pMeshClicked);


		if ((Clicked3D != NULL) && (clSceneStaff->clInput->stMouse.B1 == true))
		{
			CTVCollisionResult* pMouseMoveCollision = NULL;
			pLand->Enable(true);
			pMouseMoveCollision = clSceneStaff->clTV3D->pTVScene->MousePick(clSceneStaff->clInput->stMouse.PosX, clSceneStaff->clInput->stMouse.PosY, cTV_OBJECT_LANDSCAPE, cTV_TESTTYPE_ACCURATETESTING);
			pLand->Enable(false);
			if (pMouseMoveCollision->IsCollision())
			{
				cTV_3DVECTOR dVector;
				// Give object a direction
				GrabbedDestination = pMouseMoveCollision->GetCollisionImpact();
				GrabbedPosition = pMeshClicked->GetPosition();

				clSceneStaff->clTV3D->pTVMaths->TVVec3Subtract(&dVector, &GrabbedDestination, &GrabbedPosition);
				clSceneStaff->clTV3D->pTVMaths->TVVec3Normalize(&dVector, &dVector);
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
		if ((clSceneStaff->clTV3D->pTVMaths->GetDistance3D(GrabbedDestination.x, 0, GrabbedDestination.z, GrabbedPosition.x, 0, GrabbedPosition.z)) > 2)   //  Le 2 est trop gros, les petit movement sont impossible.. technique a revoir.

		{
			// Update the Flyer's position
			cTV_3DVECTOR dV2;
			clSceneStaff->clTV3D->pTVMaths->TVVec3Scale(&dV2, &GrabbedDirection, ((float)clSceneStaff->clTV3D->pTV->TimeElapsed() * 0.1f));
			clSceneStaff->clTV3D->pTVMaths->TVVec3Add(&GrabbedPosition, &GrabbedPosition, &dV2);
			// Update the Flyer's mesh position
			Clicked3D->SetPosNRot(GrabbedPosition, Clicked3D->Mesh->GetRotation());
		}
	}
}
void CLBallZScene::MouseHighlight(void)
{
	CTVCollisionResult* pMouseCollision = NULL;

	pLand->SetPosition(-1024, clBallZ->clShell->Mesh->GetPosition().y, -1024);
	pLand->Enable(true);
	pMouseCollision = clSceneStaff->clTV3D->pTVScene->MousePick(clSceneStaff->clInput->stMouse.PosX, clSceneStaff->clInput->stMouse.PosY, cTV_OBJECT_LANDSCAPE, cTV_TESTTYPE_ACCURATETESTING);
	pLand->Enable(false);
	if (pMouseCollision->IsCollision())
	{
		Mouse3DPos = pMouseCollision->GetCollisionImpact();
		clSceneStaff->clTV3D->DrawTV3DLine(Mouse3DPos, cTV_3DVECTOR(Mouse3DPos.x, Mouse3DPos.y + 1, Mouse3DPos.z), YELLOW_RGBA);
	}
}
void CLBallZScene::MouseDrop(void)
{
	if (pMeshClicked){
		pMeshClicked->SetColor(BLUE_RGBA, false);
		pMeshClicked = NULL;
	}
	else{
		MessageBox(NULL, (LPCTSTR)"MeshDrop sans MeshClicked en mémoire...  Bug", NULL, NULL);
	}
}
void CLBallZScene::MouseGrab(void)
{

	pClickCollision = NULL;
	pOldMeshClicked = pMeshClicked;  pMeshClicked = NULL;

	pClickCollision = clSceneStaff->clTV3D->pTVScene->MousePick(clSceneStaff->clInput->stMouse.PosX, clSceneStaff->clInput->stMouse.PosY, cTV_OBJECT_MESH, cTV_TESTTYPE_ACCURATETESTING);

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

void CLBallZScene::CreateBoxTrianglePile(float fBoxSize, cTV_3DVECTOR pos, int iTexture)
{
	vecPosition3D Triangle = clSceneStaff->clTV3D->CreateTrianglePile(6, pos, fBoxSize); // cTV_3DVECTOR(1, -7.5f, 6)  cTV_3DVECTOR(0, 0, 0)
	int i = 0;
	for each(cTV_3DVECTOR vec in Triangle)
	{
		i++; // trouver comment ajouter le i dans la string "Triangle" !!!!!!!!!!!!!!!!!!!!!!!!  <-
		clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, "Triangle", NULL, iTexture, vec, cTV_3DVECTOR(fBoxSize, fBoxSize, fBoxSize), true, 0.075f, PHYSICALL, iPhysicMaterials[WOOD], World));
		
	}
}
void CLBallZScene::LoadFormation(CLXMLData* data)
{
		int i = 0;
		clFormation->clFlyers.clear();

		clFormation->clFlyers = data->clFlyers;
		for each(CLFlyer* fly in clFormation->clFlyers){
			string name("Flyer" + to_string((long double)i));
			clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, &name[0], NULL, iMagicienTex, fly->Positions[0], cTV_3DVECTOR(1.5f, 1.5f, 1.5f), true, 0.0f, PHYSICSTATIC, iPhysicMaterials[WOOD], World));
			if (!i)// Set the BaseFlyer's vertex color to green
				clSceneStaff->cl3DObjMgr->VecObjects.back()->Mesh->SetColor(GREEN_RGBA, false);
			
			fly->Flyer3D = clSceneStaff->cl3DObjMgr->VecObjects.back();
			fly->AddActualPoint();
			i++;
		}

};
void CLBallZScene::CreateFormation(int iDefaultNbrFlyer)
{
	for (int i = 0; i < iDefaultNbrFlyer; i++){
		string name("Flyer" + to_string((long double)i));
		clSceneStaff->cl3DObjMgr->VecObjects.push_back(new CL3DObject(CUBE, &name[0], NULL, iMagicienTex, cTV_3DVECTOR((5 * ((float)i + 1)) - 30, 0, 20), cTV_3DVECTOR(1.5f, 1.5f, 1.5f), true, 0.0f, PHYSICSTATIC, iPhysicMaterials[WOOD], World));
		clFormation->clFlyers.push_back(new CLFlyer(BLUE_RGBA, clSceneStaff->cl3DObjMgr->VecObjects.back()));
		clFormation->clFlyers.back()->AddActualPoint();
		if (!i)// Set the BaseFlyer's vertex color to green
			clFormation->clFlyers[i]->Flyer3D->Mesh->SetColor(GREEN_RGBA, false);
	}
};