// IGraphysic.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "IGraphysX.h"
#include "ResourceManager.h"

//#include "tinyxml.h"

float IGraphysX::fTimeElapsed = 0.0f;
std::string  IGraphysX::m_AppPath = "";
//GraphysX::CLScene* IGraphysX::pActiveScene = nullptr;

extern "C"
{
	IGraphysX::IGraphysX(std::string PathString, int iWinWidth, int iWinHeight)
	{
		std::cout << "IGraphysX - TV3D Engine combined with latest Newton Game Dynamics Physics engine." << std::endl;
		//Set up la simulation 3D
		clTV3D.init(createSDLwindow(iWinWidth, iWinHeight), PathString);
		//  Create the 3D Scene
		defaultScene = new GraphysX::CLScene();
		pActiveScene = defaultScene;
		clActorFactory = new CLActorFactory(pActiveScene->GetWorld());
		clDisplayer.setFont("font", "Consolas", 8); // AJOUTER A L'INTERFACE DE GRAPHYSICS pour permettre la modification du font;
		m_AppPath = PathString;
		std::cout << "TV3DInterface constructed.\r\n   - App Path recorded as: " << m_AppPath << std::endl;
	}
	IGraphysX::~IGraphysX()
	{
		if (clActorFactory) delete(clActorFactory); clActorFactory = NULL;
		if (defaultScene) delete(defaultScene); defaultScene = NULL;
		clTV3D.kill();
		std::cout << "TV3DInterface destructed" << std::endl;
	}

	HWND IGraphysX::createSDLwindow(int width, int height)
	{
		SDL_Window* sdlWindow = nullptr;
		SDL_Init(SDL_INIT_EVERYTHING);
		sdlWindow = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version); // initialize info structure with SDL version info
		if (-1 == SDL_GetWindowWMInfo(sdlWindow, &wmInfo))	std::cout << "Couldn't get WM Info!" << std::endl;
		return(wmInfo.info.win.window); // return the handle HWND of the created window
	}
	/*	CTVParticleSystem* IGraphysX::CreateParticleSystem(const char* name)
		{
			CTVParticleSystem* partsys = GraphysX::CLTV3D::pTVScene.CreateParticleSystem(name);
			return partsys;
		}*/
	void IGraphysX::Update()
		{
			fTimeElapsed = GraphysX::CLTV3D::pTV.AccurateTimeElapsed(); // call from the engine only once a loop
			pActiveScene->Update(fTimeElapsed);
			clActorFactory->Update(); // TODO: devrait etre dans la scene!!!!!!!!!
			clSound.Update();
			/*clCEGUI->Update(clGlobalVar.getTimeElapsed()
					, clInput->stKeyboard.Pressedkey
					, clCEGUI->ScancodeToASCII(clInput->stKeyboard.Pressedkey,clInput->KEY_PRESSED)
					, clInput->stKeyboard.Releasedkey);*/
		}
	float IGraphysX::getTimeElapsed()
	{
		return fTimeElapsed;
	}

	/*CTVMesh* IGraphysX::getNewtonCollisionMesh(const NewtonJoint* contactJoint)
	{
		NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
		NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
		CLPhysGeom* obj1 = (CLPhysGeom*)NewtonBodyGetUserData(body0);
		CLPhysGeom* obj2 = (CLPhysGeom*)NewtonBodyGetUserData(body1);

		if (!obj1->Mesh)
		{
			if (!obj2->Mesh)
			{
				MSGBOX("bug Physic", "CLMeshPrimitive sans mesh detected by physic engine collision  :S");
			}	
			return(obj2);
		}
		if (!obj1->clPhysicEntity)
			std::cout << "Houston, we got a problem" << std::endl;
		return(obj1);
	}*/


	std::string IGraphysX::getAppPath()
	{
		return m_AppPath;
	}
	void IGraphysX::renderActiveScene()  {
		pActiveScene->Render();
	}
	void IGraphysX::renderDisplayer()  {
		clDisplayer.Render();
	}
	float IGraphysX::getSceneTime() {
		return((float)GraphysX::CLClock::getRealTime() / 1000);
	}
	void IGraphysX::MouseHighlight()
	{
		pActiveScene->MouseHighlight();
	}
	bool IGraphysX::AppStillIdle()
	{
		MSG msg;
		return !PeekMessage(&msg, NULL, 0, 0, 0);
	}
	GraphysX::CLMeshPrimitive* IGraphysX::isClickOnMesh(float mouseX, float mouseY) // TODO: devrait ptete plus retourner l'entity..
	{
		CTVMesh* tvMesh = nullptr;
		CTVCollisionResult* ColiRes = clTV3D.pTVScene.MousePick((int)mouseX, (int)mouseY);
		if (ColiRes->IsCollision())  {
			CTVMesh* tvMesh = ColiRes->GetCollisionMesh();
			if (tvMesh)
			{
				auto mit = pActiveScene->mapMeshPrim.find(tvMesh);
				return mit->second;
			}	
		}
		return(NULL);

	}

}
