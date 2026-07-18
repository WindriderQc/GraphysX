#include "stdafx.h"
#include "GamePlayScreen.h"
#include "HighScoreForm.h"
#include "IScreenApp.h"
#include <GraphysX\ResourceManager.h>
#include <GraphysX\Spline3D.h>
#include <GraphysX\ZoneTrigger.h>
#include <GfxNet\GfxNet.h>

//#include <SDL\SDL.h>


// TODO:
/*

SKYBOX PAR MAP / SCENE / SCREEN!!  -- setter aussi facilement le choix de Clock Day/Night ou fix Skybox.

Scenebuilder - Ajout de Land avec landmap

ASCII MAP SYSTEM:

ASCIIMAP HEIGHT - hauteur de l'objet ou Y size of object, pourrait etre une ARRAY de cest ASCIIMAP pour permettre un3D mode 
ASCIIMAP TEXTURE - avec des chiffre...   le chiffre est lié nom de la texture.
ASCIIMAP SHAPE - chiffre et lettre pour une varité de shape/model


intro

-select guild  -> add select ball  ( changer la texture en fonction de la balle)


EDITOR
mode create RingPath  -- avec la ball , appui sur B pour ajouter un ring a BallPos jusqua MAX_RING
ajouter du 3D au ASCII map editor
-random map generator
-ajouter des JumpZone   et Speed zone   et des lumieres
-creer des map theme : selon les mode .X   terrain buisson style chinese.     stylle vieux port...  salon de la moto/auto, ...   etc..


gestion de level  fin de race, next level.  gameover??  avec trou, avion de récupère, perd un vie, t'as 3 vie.


ACTOR FACTORY
gestion d'animation
selection de séquencel


Shader

combiner le fichier Tools car présentement dupliquer enter GraphysX et Scene 3D

*/


namespace BallZ
{
	void GenericContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
	{
		std::cout << "Generic contact process:    NOT IMPLEMENTED YET  :S " << std::endl;
		/*NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
		NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
		CLMeshPrimitive* prim1 = (CLMeshPrimitive*)NewtonBodyGetUserData(body0);
		CLMeshPrimitive* prim2 = (CLMeshPrimitive*)NewtonBodyGetUserData(body1);
		
		if (!prim1->mesh)
		{
			if (!prim2->mesh)
			{
				MSGBOX("bug Physic", "Primitive sans mesh detected by physic engine collision  :S");  // should not be possible anymore
			}
			else std::cout << "Generic contact process: " << prim2->stParam.name << std::endl;
		}
		else std::cout << "Generic contact process: " << prim1->stParam.name << std::endl;
			*/
	}
	void ball_wallContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
	{
		std::cout << "Ball & Wall contact :   no action yet  " << std::endl;
	}
	void ball_groundContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
	{
		NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
		NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
		GraphysX::ZombieKiller* zk = (GraphysX::ZombieKiller*)NewtonBodyGetUserData(body0);
		if (zk->stParam.name != "ZombieKiller") 
			zk = (GraphysX::ZombieKiller*)NewtonBodyGetUserData(body1);

		zk->bJumping = false;
  		zk->bAllowJumps = true;
	}
	void human_wallContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
	{
		NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
		NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
		GraphysX::Human* human = (GraphysX::Human*)NewtonBodyGetUserData(body0);
		if (human->stParam.name != "human") 
			human = (GraphysX::Human*)NewtonBodyGetUserData(body1);

		//human->reverseDirection();
		human->setRandomDirection();
	}

	//	CLMeshPrimitive* mesh = IGraphysX::getNewtonCollisionMesh(contactJoint);
		/*
			CLMesh* IGraphysX::getNewtonCollisionMesh(const NewtonJoint* contactJoint)
				{
					NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
					NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
					CLMesh* mesh1 = (CLMesh*)NewtonBodyGetUserData(body0);
					CLMesh* mesh2 = (CLMesh*)NewtonBodyGetUserData(body1);

					if (!mesh1->Mesh)
					{
						if (!mesh2->Mesh)
						{
							MSGBOX("bug Physic", "CLMesh sans mesh detected by physic engine collision  :S");
						}	
						return(mesh2);
					}
					if (!mesh1->clPhysicEntity)
						std::cout << "Houston, we got a problem" << std::endl;
					return(mesh1);
				}
		*/
		/*if (!mesh->mesh)
			MSGBOX("error", "collision avec CLMeshPrimitive sans mesh")
			if (!mesh->clPhysicEntity)
			{
				int test = mesh->TypeMesh; 
				std::string str = "collision avec CLMeshPrimitive sans clPhysicEntity: ";
				//str = str +  mesh->Mesh->GetMeshName();
				std::cout << str << std::endl;
			}
			else std::cout << "Generic contact process: " << mesh->Mesh->GetMeshName() << std::endl;*/
		/*
		//cl3DObject->bAllowJumps = true;

		for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact))
		{
		dFloat contactNormalSpeed;
		NewtonMaterial* material;

		// get the material for this contact;
		material = NewtonContactGetMaterial (contact);

		contactNormalSpeed = NewtonMaterialGetContactNormalSpeed (material);

		if (contactNormalSpeed > contactBestSpeed)
		{
		contactBestSpeed = contactNormalSpeed;
		dVector normal;
		contactBestSpeed = contactNormalSpeed;
		NewtonMaterialGetContactPositionAndNormal (material, body0, &contactPosit[0], &normal[0]);
		//bestSound = NewtonMaterialGetMaterialPairUserData (material);
		}

		}*/


	GameplayScreen::GameplayScreen() {
		m_ScreenIndex = SCREEN_INDEX_GAMEPLAY;
	}
	GameplayScreen::~GameplayScreen() {
	}
	int GameplayScreen::getNextScreenIndex() const {
		return SCREEN_INDEX_AFTERRACE;
	}
	int GameplayScreen::getPreviousScreenIndex() const {
		return SCREEN_INDEX_MAINMENU;
	}
	void GameplayScreen::render() {
		//  Render Water surfaces
		//pActiveScene->UpdateWater(clSceneStaff->clClock->getTimeOfDay());
		// SCREEN RENDER BEGIN
		//////////////
		GraphysX::CLTV3D::renderBegin();
		m_graphysX->MouseHighlight();
		m_graphysX->renderActiveScene();
		m_graphysX->renderDisplayer();
		//if(bMenu) m_gui.Draw();
		GraphysX::CLTV3D::renderEnd();
		//////////// 
	}
	void GameplayScreen::update() {
		
		checkInput();
		
		m_graphysX->Update();
		m_zombieKiller->setMouse3Dpos(m_graphysX->pActiveScene->getMouse3DPos());
		m_graphysX->pActiveScene->vecLine3D.push_back(m_zombieKiller->BallDirectionTag);
		//ADAPTER LE ENDPOS POUR QUE CA SUIVE LA VELOCITY DE LA BALLE
		if (m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl == false) {
			//m_graphysX->pActiveScene->clCamera.SetChase(m_zombieKiller->getInsideMesh()->mesh, cTV_3DVECTOR(0, 4, -8), cTV_3DVECTOR(0, 0, 0), 0.5f);
			m_graphysX->pActiveScene->clCamera.pCamera->SetPosition((float)currentScene3D->mapSize->x / 2, 10.0f, (float)currentScene3D->mapSize->y / 2 );
			cTV_3DVECTOR look = m_zombieKiller->getPos();
			m_graphysX->pActiveScene->clCamera.pCamera->SetLookAt(look.x, look.y, look.z);
		}
			

		clAnneaux->Update();  //  sert just a updater les particles...   devrait etre plus un Particlemanager->update()
		cTV_3DVECTOR zkPos = m_zombieKiller->mesh->GetPosition();
		GraphysX::CLMeshPrimitive* ring = clAnneaux->CheckProximity(zkPos, m_zombieKiller->stParam.scale);
		if (ring) 
		{
			clAnneaux->deleteRing(ring);
			m_graphysX->pActiveScene->mapMeshPrim.erase(ring->mesh);
			m_graphysX->pActiveScene->vEntity.erase(std::remove(m_graphysX->pActiveScene->vEntity.begin(), 
																		m_graphysX->pActiveScene->vEntity.end(), ring), 
																		m_graphysX->pActiveScene->vEntity.end());
			delete(ring);
		}
		if ((clAnneaux->getScore() == 2) && (clAnneaux->isGameOver == false))
		{//if ((clAnneaux->CheckContact(m_zombieKiller->getOutsideMesh()) == 10) && (clAnneaux->isGameOver == false)) {
				cout << "VICTORY";
				clAnneaux->isGameOver = true;
				//HighScoreForm^ ScoreSheet = gcnew HighScoreForm(m_graphysX->getSceneTime());
				//ScoreSheet->Show();
				iCurrentLevel++;
				if ((UINT)iCurrentLevel >= vecLevels.size())
				{
					iCurrentLevel = 0;
					std::cout << " Levels all Completed. Congrats!!!    Back to level 0..." << std::endl;
				}
				m_graphysX->pActiveScene->ClearScene();
				currentScene3D->EntityNETList.Clear();
				m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		}
		
		if (lapChecker->IsLapCompleted(zkPos))  m_graphysX->clDisplayer.addLap();

		Physics::CheckAllZoneTriggers(zkPos);
		
				
		//Voie Lactée                  AJOUTER le calcul de lever/coucher de soleil de l'atmosphere pour passer earth the earthmap a earthnnight texture
		/*clEarth->Rotate(cTV_3DVECTOR(0, 1, 0), 0.02f);
		clEarthClouds->Rotate(cTV_3DVECTOR(0, 1, 0), 0.06f);
		clMoon->RotateAround(-0.003f * clSceneStaff->clGlobalVar->fTimeElapsed, &clEarth->Mesh->GetPosition(), 8.0f);*/


		/*Tools3D->m_CEGUI->Update(Tools3D->m_GlobalVar->getTimeElapsed(),
		Tools3D->m_Input->stKeyboard.Pressedkey,
		Tools3D->m_CEGUI->ScancodeToASCII(Tools3D->m_Input->stKeyboard.Pressedkey, Tools3D->m_Input->KEY_PRESSED),
		Tools3D->m_Input->stKeyboard.Releasedkey);*/

		/*
		if (Screen3DForm->ClickedMesh){
		CTVMesh* mesh = Screen3DForm->ClickedMesh->Mesh;
		//String^ str2 = gcnew String(std::string(mesh->GetMeshName()).c_str());
		//Clicked->labelName->Text = str2;
		Clicked->labelName->Text = Tools::CharsToSystemString(mesh->GetMeshName());
		Clicked->labelPos->Text = mesh->GetPosition().x + "/" + mesh->GetPosition().y + "/" + mesh->GetPosition().z;
		Clicked->Show();
		}
		else
		Clicked->Hide();*/

		/*MousePosLabel->Text = Screen3DForm->MouseX + ":" + Screen3DForm->MouseY;
		//label3DPosValue->Text = Screen3DForm->Mouse3DPos->x + "." + Screen3DForm->Mouse3DPos->y + "." + Screen3DForm->Mouse3DPos->z;
		label3DPosValue->Text = Tools3D->m_ActiveScene->Mouse3DPos.x + " ' " + Tools3D->m_ActiveScene->Mouse3DPos.y + " ' " + Tools3D->m_ActiveScene->Mouse3DPos.z;
		labelThrottleValue->Text = Tools3D->m_MeshFactory->BallZCtrler->fThrottle + "/" + Tools3D->m_MeshFactory->BallZCtrler->fMaxThrottle;
		*/
	}
	void GameplayScreen::checkInput() 
	{
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) 
		{
			m_screenApp->onSDLEvent(evnt);
			//m_gui.onSDLEvent(evnt);  // this is already done in m_screenApp->on SDLEvent.   do we want to seperate that for screen without GUI?
			switch (evnt.type) 
			{
			case SDL_WINDOWEVENT_LEAVE:
				// TODO: Stop the ball control here !!!!!!!!!!!!!!  //  PAUSE THE PHYS SIMULATION??
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_zombieKiller->bThrottle = true;
				m_graphysX->pActiveScene->mouseClick(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				m_zombieKiller->bThrottle = false;
				break;
			case SDL_MOUSEMOTION:
				MouseX = (float)evnt.motion.x;
				MouseY = (float)evnt.motion.y;
				//if (Tools3D->m_CEGUI)
				//	Tools3D->m_CEGUI->getContext()->injectMousePosition(MouseX, MouseY);
			case SDL_KEYDOWN:
				//m_zombieKiller->bJumping = false; // serait normalement fait dans la gestion des contact de materiaux physique
				switch (evnt.key.keysym.sym)
				{
				case SDLK_DOWN:
					break;
				case SDLK_UP:
					break;
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_c:
					m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = !m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl;
					break;
				case SDLK_SPACE:
					m_zombieKiller->bJumping = true;
					//m_zombieKiller->bAllowJumps = true;   //  Devrait etre implanté dans la gestion de contact physique de BALL material
					break;
				case SDLK_b: 
					{
						cTV_3DVECTOR dir;
						cTV_3DVECTOR pos = m_zombieKiller->getPos(); 
						GraphysX::CLTV3D::pTVMaths.TVVec3Subtract(&dir, &m_graphysX->pActiveScene->getMouse3DPos(), &pos);
						GraphysX::CLTV3D::pTVMaths.TVVec3Normalize(&dir, &dir);
						vecBullets.push_back(m_graphysX->pActiveScene->shotBullet("metal13.jpg", pos, dir, m_zombieKiller->stParam.scale.x*2, 0.15f, ePHYSMAT::DEF_PHYSMAT, 500.0f,  1.0f, 1000));
						break;
					}
				case SDLK_t:
					{
						//testPlate->bTrigger = true;
						break;
					}
				case SDLK_ESCAPE:
				{
					//m_nextScreenIndex = SCREEN_INDEX_MAINMENU;
					//m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
					//bMenu = !bMenu;
					break;
				}
				}
			default:
				break;
			}
		}
	}
	void GameplayScreen::build() {
		m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice());  // TODO : semble etre loadé a chaque screen..  mettre un flag static dans l'init de CEGUI...
		vecLevels.push_back(BallZLevel());  //  Checkerboard.png
		vecLevels.push_back(BallZLevel("..\\StockRoom\\Suzanne2.xml", "..\\StockRoom\\Suzanne2.ASCII", true, "GrassSample.jpg", "..\\StockRoom\\Suzanne1.obj", RGBA_AQUA, 0.2f, 300));
	}
	void GameplayScreen::destroy()
	{
		GraphysX::ResourceManager::releaseSounds(); 
	}
	void GameplayScreen::onEntry() {
		std::cout << "OnEntry - GamePlay\n";// TODO: check pkoi les physics material semble etre loader 2x ici....
		FMOD::Sound* snd = GraphysX::ResourceManager::getSound("intro.mp3", false);
		m_graphysX->clSound.PlaySound(snd);
		m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = false;
		//m_graphysX->pActiveScene->clCamera.fMoveFactor = 0.08f;
		m_graphysX->pActiveScene->clCamera.pCamera->SetPosition(-10,7.5, -5);
		m_graphysX->pActiveScene->clCamera.pCamera->SetLookAt(0, 0, 0);
		NewtonWorld* m_world = m_graphysX->pActiveScene->GetWorld();
		GfxNet::SceneNET scene;
		std::cout << "Deserializing xml Scene...\n\r";
		currentScene3D = scene.Deserialized(vecLevels[iCurrentLevel].XMLFile);// Désérialise le XML dans le Active Scene3D
		m_graphysX->pActiveScene->BuildScene(GraphysX::Tv::sceneNETtoNative(currentScene3D));
		if (vecLevels[iCurrentLevel].ASCIIFilePath.length() >= 1)
		{
			cTV_2DVECTOR dim = m_graphysX->pActiveScene->BuildASCIIScene(vecLevels[iCurrentLevel].ASCIIFilePath, GraphysX::Tv::sceneNETtoNative(currentScene3D));
			currentScene3D->mapSize->x = dim.x;
			currentScene3D->mapSize->y = dim.y;
		}
		if (vecLevels[iCurrentLevel].bAddFloor)
			m_graphysX->pActiveScene->addFloor(currentScene3D->mapSize->x, vecLevels[iCurrentLevel].FloorTexName);  // TODO : arrange rpour que le x et Y soit pris en compte..
		
		// Finish Line
		float length = GraphysX::CLTV3D::pTVMaths.GetDistance3D(currentScene3D->fS->x, currentScene3D->fS->y, currentScene3D->fS->z, currentScene3D->fF->x, currentScene3D->fF->y, currentScene3D->fF->z);
		cTV_3DVECTOR pos;
		cTV_3DVECTOR fs = cTV_3DVECTOR(currentScene3D->fS->x, currentScene3D->fS->y + 1, currentScene3D->fS->z);
		cTV_3DVECTOR ff = cTV_3DVECTOR(currentScene3D->fF->x, currentScene3D->fF->y + 1, currentScene3D->fF->z);
		GraphysX::CLTV3D::pTVMaths.TVVec3Subtract(&pos, &ff, &fs);
		GraphysX::CLTV3D::pTVMaths.TVVec3Scale(&pos, &pos, 0.5f);
		GraphysX::CLTV3D::pTVMaths.TVVec3Add(&pos, &pos, &fs);
		GraphysX::CLMeshPrimitive* finishLine = m_graphysX->pActiveScene->addPrimCube("finishboard", pos, { 0.25f, 0.75f, length }, "Checkerboard.png");  // 
		finishLine->mesh->LookAtPoint(&ff);
		GraphysX::STEntityParam stTextParam;
		stTextParam.name = "finishText";
		stTextParam.geom = eGeometry::NON_PRIMITIVE;
		stTextParam.pos = { ff.x, ff.y - 0.3f, ff.z - 1.5f };
		stTextParam.rot = { 0, 90, 0 };
		stTextParam.color = RGBA_BLUE;
		GraphysX::CLMesh3DText* finishText = m_graphysX->pActiveScene->addMesh3DText("Finish", "Arial", 42, stTextParam);
	//	finishText->mesh->SetMaterial(ResourceManager::getMaterial(eMATERIAL::DEF_MATERIAL));
		
		lapChecker = gcnew LapChecker(m_graphysX
			, Vector3(currentScene3D->fS->x, currentScene3D->fS->y, currentScene3D->fS->z)
			, Vector3(currentScene3D->fF->x, currentScene3D->fF->y, currentScene3D->fF->z)
			, Vector3(currentScene3D->hS->x, currentScene3D->hS->y, currentScene3D->hS->z)
			, Vector3(currentScene3D->hF->x, currentScene3D->hF->y, currentScene3D->hF->z));

		std::cout << "  - Creating Special Character...\n\r";
		m_graphysX->pActiveScene->CreateVoieLactee();
	
		
		float zkSize = 0.3f;
		cTV_3DVECTOR vRadiusIn(0.75f*zkSize, 0.75f*zkSize, 0.75f*zkSize);
		cTV_3DVECTOR zkPos(currentScene3D->playerPos->x, currentScene3D->playerPos->y, currentScene3D->playerPos->z);
		
		GraphysX::STEntityParam param(eObjet::PRIMITIVE, eGeometry::CUSTOM_MESH, "..\\StockRoom\\SuperCage.x", zkPos, { zkSize, zkSize, zkSize });
		GraphysX::CLMeshPrimitive* cage = m_graphysX->pActiveScene->addMeshPrimitive(param);
		
		GraphysX::STEntityParam paramIn(eObjet::PRIMITIVE, eGeometry::SPHERE, "BallZInside", zkPos, vRadiusIn, Vector3(-90, 0, 0));
		paramIn.texName = "FireArrow800.jpg";
		GraphysX::CLMeshPrimitive* insideBall = m_graphysX->pActiveScene->addMeshPrimitive( paramIn);

		GraphysX::STEntityParam paramOut(eObjet::PHYSPRIMITIVE, eGeometry::SPHERE, "ZombieKiller", zkPos, { zkSize, zkSize, zkSize });
		m_zombieKiller = new GraphysX::ZombieKiller(cage, insideBall, m_world, paramOut);  // TODO:  new sans delete!!!
		m_graphysX->pActiveScene->vEntity.push_back(m_zombieKiller); // TODO:  Si on donne CLScene comme param a la creation d'entity, l'ajout au vecEntity se ferait dans la base class. tout comme le Physic world.

		std::cout << "  - Physics Materials loaded - Qty : " << sizeof(GraphysX::ResourceManager::nbrMaterial()) << std::endl;

		// create the character material interactions	
		m_graphysX->pActiveScene->setPhysmatCollisionCallback(BALL, WALL, ball_wallContactProcess);
		m_graphysX->pActiveScene->setPhysmatCollisionCallback(BALL, GROUND, ball_groundContactProcess);
		m_graphysX->pActiveScene->setPhysmatCollisionCallback(HUMAN, WALL, human_wallContactProcess);
		

		
		GraphysX::STEntityParam stPlane;
		stPlane.pos = { 5, 5, 5 };
		stPlane.scale = { 1, 1, 1 };
		stPlane.speed = 5;
		stPlane.name = "..\\StockRoom\\Airplane\\AirplaneLP.TVM";
		stPlane.geom = eGeometry::CUSTOM_MESH;
		stPlane.masse = 2.0f;
		//airplane = new CLDynamicMesh(m_world, stPlane,ePHYSMAT::DEF_PHYSMAT, 2.0f);  // TODO:  new sans delete!!!
		airplane = m_graphysX->pActiveScene->addDynamicMesh(stPlane);  

		
		GraphysX::Spline spline(Tools::XmlSplineRead("..\\StockRoom\\Spline.xml"));
		airplane->setFollowSpline(spline, true);
		m_graphysX->pActiveScene->add3DSpline(spline, true);  // TODO : sassurer qu'on fasse un delete scene et que ca efface tout ce qui est added! 


		GraphysX::STEntityParam stHuman;
		stHuman.name = "human";
		stHuman.rot = { 0, 0, 0 };
		stHuman.geom = eGeometry::SPHERE;
		stHuman.scale = { 0.3f, 0.3f, 0.3f };
		stHuman.speed = 0.1f;
		stHuman.color = RGBA_GREEN;
		stHuman.physMat = ePHYSMAT::HUMAN;
		stHuman.masse = 1.0f;
		for (int i = 0; i < 10; i++)
		{
			stHuman.pos = cTV_3DVECTOR(Tools::random_float(2.0f, currentScene3D->mapSize->x - 2.0f), 2.5f, Tools::random_float(2.0f, currentScene3D->mapSize->x - 2.0f));
			GraphysX::Human* flameHuman = m_graphysX->pActiveScene->addSphereHuman(stHuman, m_graphysX->pActiveScene->cycleTexFire);
		//	flameHuman->mesh->SetMaterial(ResourceManager::getMaterial(eMATERIAL::DEF_MATERIAL));
			flameHuman->mesh->SetColor(RGBA_BLUE); // TODO:  si on applique le materiel, la couleur tombe a white...   a cause du ambiant setté dans le mattériel I guess.  faire un get color et ajuster l'ambient du mat avant d'appliquer
		}


		

		GraphysX::STEntityParam stCone;
		stCone.name = "testcone";
		stCone.geom = eGeometry::CONE;
		stCone.pos = { 5, 5, 5 };
		stCone.scale = { 1, 1,1 };
		stCone.rot = { 45, 180.0f, 0 };
		stCone.texName = "twoway.jpg";
		stCone.masse = 1.0f;
		GraphysX::CLDynamicMesh* cone = m_graphysX->pActiveScene->addDynamicMesh(stCone);

		m_graphysX->pActiveScene->createFire({5,0.1f,5});

		/*STEntityParam stPar;
		stPar.name = "sliderbar";
		stPar.geom = eGeometry::CUBE;
		stPar.pos = { 8, 2, 12 };
		stPar.scale = {5, 0.25f, 0.25f };
		stPar.rot = { 0, 0, 0 };
		stPar.color = RGBA_DARKGRAY;
		CLDynamicMesh* sliderbar = m_graphysX->pActiveScene->addDynamicMesh("twoway.jpg", stPar, ePHYSMAT::DEF_PHYSMAT, 0.0f); 
		cTV_3DMATRIX tvMat = sliderbar->mesh->GetMatrix();
		/*std::cout << "tvMat:" << std::endl;
		float *val = &tvMat.m11;
		for (int i = 0; i < 16; i++)
		{
			std::cout << *val << " ";
			val++;
			if (i == 3) std::cout << std::endl;
			if (i == 7) std::cout << std::endl;
			if (i == 11) std::cout << std::endl;

		}//*/
		/*dMatrix newtMat;
		NewtonBodyGetMatrix(sliderbar->getBody(), &newtMat[0][0]);
		testPlate = new PushPlate(m_world, STEntityParam("pushplate", eGeometry::CUBE, stPar.pos, { 1.0f, 1.0f, 1.0f }, stPar.rot), newtMat, { 100.0f, 0, 0 }, { -1.0f, 0, 0 }, 1.0f, sliderbar->getBody());
		testPlate->setLimits(-5.0f, 5.0f, 0, 0);
	*/
		

		// Creating Rings
		clAnneaux = new CLAnneaux(m_graphysX, 0.2f);
		for each (Vec3D^ vec in currentScene3D->ringPosList)  {
			clAnneaux->addRing(cTV_3DVECTOR(vec->x, vec->y+0.1f, vec->z), RGBA_WHITE, 0.5f);
		}
		//Reset time and Score here
		m_graphysX->clDisplayer.resetScore();
		m_graphysX->clDisplayer.resetLap();
		GraphysX::CLClock::Reset(); 

		snd = GraphysX::ResourceManager::getSound("Piano.mp3", true);
		m_graphysX->clSound.PlaySound(snd);
		std::cout << "Screen Entry completed - GamePlay\n";
	}
	void GameplayScreen::onExit(){
		std::cout << "OnExit - GamePlay\n";
		delete(lapChecker); lapChecker = nullptr;	  // TODO devrait etre un smart pointer  pas besoin de ca...
		//Builder.ClearScene3D(currentScene3D);

	}
}