#pragma once
#include "GraphysXdef.h"
#include "IGraphysX.h"
#include <SDL\SDL.h>
#include "Clock.h"
#include <memory>

namespace GraphysX
{
	class ScreenList;
	class IScreen3D;

	class GRAPHYSX_API IScreenApp
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
		 virtual IGraphysX* getGraphysX() { return m_GraphysX; }
		
	protected:
		 bool init();
		std::unique_ptr<ScreenList> m_screenList = nullptr;
	private:
		bool initSystems();
		void processInput();

		IScreen3D* m_currentScreen = nullptr;
		SDL_Window* sdlWindow = nullptr;
		HWND winHWND = nullptr;   // used to transfer the window handle to GraphysX
		IGraphysX* m_GraphysX = nullptr;  // TODO:  pourrait etre en static pour acceder easy a IScreenApp::m_GraphysyX??

		FpsLimiter m_fpsLimiter;
		int iScreenWidth = 1280, iScreenHeight = 720;
		static bool m_isRunning;
	};
}
