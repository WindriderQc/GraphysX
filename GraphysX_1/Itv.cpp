#include "stdafx.h"
#include "Itv.h"
#include "TV3DMoteur.h"
#include "Scene.h"
#include "Tools.h"


#include <SDL\SDL.h>
#include <SDL\SDL_syswm.h>

namespace GraphysX
{
	extern "C"
	{
	// GRAPHYSX Static function
	HWND createSDLwindow(int width, int height)
		{
			SDL_Init(SDL_INIT_EVERYTHING);
			SDL_Window* sdlWindow = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version); // initialize info structure with SDL version info
			if (-1 == SDL_GetWindowWMInfo(sdlWindow, &wmInfo))	std::cout << "Couldn't get WM Info!" << std::endl;
			return(wmInfo.info.win.window); // return the handle HWND of the created window//
			//return nullptr;
		}
		
		// Itv
		CLScene* Itv::activeScene = nullptr;

		Itv::Itv(void* handle)
		{
			CLTV3D::init(HWND(handle), Tools::getexepath());
			activeScene = new CLScene();
			//activeScene->addPrimSphere("test", { 0, 0, 0 }, 1.0f);// , std::string textureName = "")
			//CLTV3D::init(createSDLwindow(), Tools::getexepath());
		}
		Itv::~Itv()
		{
			delete activeScene;
			std::cout << " tv3d kill start" << std::endl;
			CLTV3D::kill();
			std::cout << " tv3d kill completed" << std::endl;
			//SDL_DestroyWindow(sdlWindow);
		}

		void Itv::render()
		{
			CLTV3D::renderBegin();  //  TODO :  devra etre séparé Begin, render, end, pour permettre au user différent control sur render...
			activeScene->Render();
			//CLTV3D::pTVScene.RenderAll(true);
			CLTV3D::renderEnd();
		}
		void Itv::update()
		{
			activeScene->Update();
		}
		bool Itv::AppStillIdle()
		{
			MSG msg;
			return !PeekMessage(&msg, NULL, 0, 0, 0);
		}
		void Itv::CreateEntity(STEntityParam obj)
		{
		//  TODO  :  A FAIRE
		};
		void Itv::addPhysMesh(STEntityParam param)
		{
			activeScene->addDynamicMesh(param);
		}
		void Itv::ClearActiveScene()
		{
			activeScene->ClearScene();
		}
		void Itv::BuildScene(STSceneParam Scene2Build)
		{
			activeScene->BuildScene(Scene2Build);
		}
		cTV_2DVECTOR Itv::BuildASCIIScene(std::string asciiFilePath)
		{
			return(activeScene->BuildASCIIScene(asciiFilePath));
		}
		void Itv::addFloor(int mapSize, std::string floorTextureName)
		{
			activeScene->addFloor(mapSize, floorTextureName);
		}
		void Itv::createSmoke()
		{
			activeScene->createSmoke();
		}
		//void CreatePiston(float x, float z, float AngleY);
		void Itv::CreateVoieLactee(void)
		{
			activeScene->CreateVoieLactee();
		}
		void Itv::CreateChain(cTV_3DVECTOR Pos, float fScale, const float angle, float masse, std::string texName)
		{
			activeScene->CreateChain(Pos, fScale, angle, masse, texName);
		}
	}
}