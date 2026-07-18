#pragma once
#include <GraphysX\GraphysXdef.h>
#include <GraphysX\TV3DMoteur.h>   //  PERMET A TOUTES les écrans de pouvoir caller CLTV3D....   TODO: surement plus a implanter autremwenr...
#include "IScreen3D.h"
#include <SDL\SDL.h>
#include <memory>

class Itv;
class ScreenList;
class IScreen3D;

namespace GraphysX
{
	class IScreenApp
	{
	public:
		IScreenApp();
		virtual ~IScreenApp();

		void run();  // create its own while(running) loop
		void exitGame();
		//GRAPHYSX_API void Cleanup(){};
		virtual void update();
		virtual void render();
		virtual void onSDLEvent(SDL_Event& evnt);
		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;
		
		bool isRunning() { return m_isRunning; }
		virtual Itv* getTv() { return m_tv; }
		
	protected:
		bool init();
		std::unique_ptr<ScreenList> m_screenList = nullptr;
	private:
		bool initSystems();
		void processInput();

		IScreen3D* m_currentScreen = nullptr;
		SDL_Window* sdlWindow = nullptr;
		HWND winHWND = nullptr;   // used to transfer the window handle to GraphysX
		Itv* m_tv = nullptr;  // TODO:  pourrait etre en static pour acceder easy a IScreenApp::m_GraphysyX??

		int iScreenWidth = 1280, iScreenHeight = 720;
		static bool m_isRunning;
	};
}
