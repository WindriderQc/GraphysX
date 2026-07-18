// ConsoleGraphysX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib, "..\\Debug\\GraphysX")
#pragma comment(lib, "..\\Deps\\Lib\\TV3D65")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2")
//#pragma comment(lib, "..\\Deps\\Lib\\SDL2Main")
//#pragma comment(lib, "..\\Deps\\Lib\\CEGUIBase-0_d")
//#pragma comment(lib, "..\\Deps\\Lib\\CEGUIDirect3D9Renderer-0_d")
//Newton Lib
//#pragma comment(lib,"..\\Deps\\Lib\\newton_d")
//#pragma comment(lib,"..\\Deps\\Lib\\core_d")
//#pragma comment(lib,"..\\Deps\\Lib\\dCustomJoints_d")
//#pragma comment(lib,"..\\Deps\\Lib\\dMath_d")
//#pragma comment(lib,"..\\Deps\\Lib\\physics_d")
//#pragma comment(lib,"..\\Deps\\Lib\\pthread_d")
//#pragma comment(lib,"..\\Deps\\Lib\\dContainers_d")

//#pragma comment(lib,"..\\Deps\\Lib\\fmod_vc")

/*int main()
{
    return 0;
}
*/
#include "..\GraphysX\GraphysXdef.h"
#include "..\GraphysX\ITv.h"
#include "..\GraphysX\SceneEditor.h"
#include "..\GraphysX\Tools.h"
//#include "..\GraphysX\CLCEGUI.h"

#include "..\GraphysX\camera.h"

#include <SDL\SDL.h>
#include <SDL\SDL_syswm.h>

static bool m_isRunning = true;
GraphysX::Itv* tv;
GraphysX::CLSceneEditor* activeScene;

void ExitLoop() {
	m_isRunning = false;
}
void checkConsoleInput()
{
	//  SCRIPTING SECTION	-  To be completed
	if (_kbhit())  // If you have pressed any key
	{
		char ch = _getch();
		switch (ch)
		{
		case 'w':
			std::cout << "w";
			//test->param.pos.y += 0.25f;
			break;
		case 'a':
			std::cout << "a";
			//test->param.pos.x -= 0.25f;
			break;
		case 's':
			std::cout << "s";
			//test->param.pos.y -= 0.25f;
			break;
		case 'd':
			std::cout << "d";
			//test->param.pos.x += 0.25f;
			break;
		case 27: //27 for the ESC key,
			ExitLoop();
			break;
		}
	}
}
void checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_MOUSEMOTION:
			//std::cout << evnt.motion.x << "." << evnt.motion.y << std::endl;
			break;
		case SDL_QUIT:
			ExitLoop();
			break;
		case SDL_MOUSEBUTTONDOWN:
			activeScene->mousePick();
			//m_zombieKiller->bThrottle = true; //  TODO:   ceci devrait etre dans la classe ZombieKiller.  donc a la creation de l×objet, le update va gerer autot ses propre comntrol.  Avoir un check Input en  pure virtual?
			//m_graphysX->pActiveScene->mouseClick(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_MOUSEBUTTONUP:
			//m_zombieKiller->bThrottle = false;
			break;
		case SDL_KEYDOWN:
		{
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
				//activeScene->clCam->bKeyboardNMouseCtrl = true;
				break;
			case SDLK_v:
				//activeScene->clCam->bKeyboardNMouseCtrl = false;
				break;
			case SDLK_SPACE:
				//m_zombieKiller->bJumping = true;
				//m_zombieKiller->bAllowJumps = true;   //  Devrait etre implanté dans la gestion de contact physique de BALL material
				break;
			case SDLK_b:
			{
				/*GraphysX::STEntityParam stParam;
				stParam.name = "cylinder";
				stParam.type = eObjet::CYLINDER;
				stParam.pos = { 3, 6, 4 };
				stParam.scale = { 1, 2, 8 };
				stParam.rot = { 0, 0, 90 };
				stParam.texName = "twoway.jpg";
				//activeScene->addMeshPhysic(stParam);
				cTV_3DVECTOR dir;
				cTV_3DVECTOR pos = m_zombieKiller->getPos();
				GraphysX::CLTV3D::pTVMaths.TVVec3Subtract(&dir, &m_graphysX->pActiveScene->getMouse3DPos(), &pos);
				GraphysX::CLTV3D::pTVMaths.TVVec3Normalize(&dir, &dir);
				m_graphysX->pActiveScene->shotBullet("metal13.jpg", pos, dir, m_zombieKiller->stParam.scale.x * 2, 0.15f, ePHYSMAT::DEF_PHYSMAT, 500.0f, 1.0f, 1000);*/
				break;
			}
			case SDLK_t:
			{
				//testPlate->bTrigger = true;  
				break;
			}
			case SDLK_ESCAPE:
			{
				ExitLoop();
				break;
			}
			}
		}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string path = Tools::getexepath();
	HWND w = GraphysX::createSDLwindow(1280, 720);
	tv = new GraphysX::Itv(w);
	activeScene = new GraphysX::CLSceneEditor();
	activeScene->setMinimal();
	activeScene->addVoieLactee();
	
	activeScene->clCam.PosLookAt(0, 5, -5, 0, 0, 0);
	
	std::cout << "Application Path: " << path << std::endl;
	//m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = false;

	/*GraphysX::STEntityParam stParam;
	stParam.type = eObjet::PLANE;  // floor
	stParam.pos = { 0, 0, 0 };
	stParam.scale = { 50, 50, 10 };
	stParam.physMat = ePHYSMAT::GROUND;
	stParam.texName = "Damier.jpg";
	activeScene->addMeshPhysic(stParam);*/

	/*
	stParam.name = "cube";
	stParam.type = eObjet::CUBE;
	stParam.pos = { -1, 2, 0 };
	stParam.scale = { 4, 2.5, .5 };
	stParam.rot = { 45, 90,30 };
	stParam.physMat = ePHYSMAT::DEF_PHYSMAT;
	stParam.texName = "twoway.jpg";
	stParam.masse = 1.0f;
	activeScene->addDynamicMesh(stParam);


	stParam.name = "cone";
	stParam.type = eObjet::CONE;
	stParam.pos = { -1, 2, -2 };
	stParam.scale = { 1, 2, 8 };
	stParam.rot = { 180, 45, 45 };
	activeScene->addDynamicMesh(stParam);
	//stParam.geom = eGeometry::SPHERE;
	//stParam.pos = { 3, 1.5f, 0 };
	//m_GraphysX->pActiveScene->addKinematicPrimitive("Galaxy\\EarthHI.jpg", stParam, ePHYSMAT::DEF_PHYSMAT);
	stParam.name = "cylinder";
	stParam.type= eObjet::CYLINDER;
	stParam.pos = { 3, 2, 2 };
	stParam.scale = { 0.25, 2, 8 };
	stParam.rot = { 0, 0, 90 };
	activeScene->addDynamicMesh(stParam);*/
	/*
	for (int i = 0; i < 10; i++)
	{
		stParam.name = "cylinder";
		stParam.type = eObjet::CUBE;
		stParam.pos = { -3.0f, 0.5f + i, 10.0f };
		stParam.scale = { 0.5f, 0.5f, 8 };
		stParam.rot = { 0, 0, 0 };
		stParam.texName = "Zack.jpg";
		stParam.masse = 0.5f;
		activeScene->addDynamicMesh(stParam);
		stParam.pos = { -1.0f, 0.5f + i, 7.50f };
		stParam.texName = "twoway.jpg";
		stParam.masse = 1.0f;
		activeScene->addDynamicMesh(stParam);
	}*/


	//Load the shader script
	//'NOTE : The textures are loaded before loading the shaders.
	CTVScene pScene;
	CTVShader* test;
	//test = pScene.CreateShader("test");
	//test->CreateFromEffectFile("..\\StockRoom\\Shaders\\common.shader");

	//'Create the billboards on which the shaders will be applied
	//CTVMesh* Flame = pScene.CreateBillboard(0, 0, 0, 0, 20, 40);
	//CTVMesh* Explosion = pScene.CreateBillboard(0, 50, 0, 0, 40, 40);

	//'Apply the shader to the mesh
	// 'Note that the shader names are defined in the Script file
	//Flame->SetShader(test);// ->GetTechniqueByName("Flame1"));
	//Explosion->SetShader(test);// GetShader("Explosion")

	while (m_isRunning)
	{
		tv->renderBegin();
		activeScene->Render();
		tv->renderEnd();
		activeScene->Update();
		//m_graphysX->MouseHighlight();// Find3DObjectWithName("BallZ")->Mesh->GetPosition().y);
		//m_zombieKiller->setMouse3Dpos(m_graphysX->pActiveScene->getMouse3DPos());   // TODO:   Pas beau ca....    devrait etre fait auto non?
		//m_zombieKiller->update();
		checkConsoleInput();
		checkInput();
	}

	delete(activeScene);
	delete(tv);
	//system("PAUSE");
	return 0;
}
