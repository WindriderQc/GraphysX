#include "stdafx.h"
#include "IScreenApp.h"
#include "ScreenList.h"
#include "IScreen3D.h"
#include "Tools.h"
#include <SDL\SDL_syswm.h>

//bool GraphysX::IScreenApp::m_isRunning = false;

namespace GraphysX
{

	bool IScreenApp::m_isRunning = false;

	IScreenApp::IScreenApp()
	{
		m_screenList = std::make_unique<ScreenList>(this);
	}
	IScreenApp::~IScreenApp()
	{
		//if (m_GraphysX) delete(m_GraphysX); m_GraphysX = nullptr;
	}
	void IScreenApp::run()
	{
		if (!init()) return;
		
		m_isRunning = true;
		while (m_isRunning) {
			update();
			render();
		}
	}
	void IScreenApp::exitGame()
	{
		m_currentScreen->onExit();
		if (m_screenList) {
			m_screenList->destroy();
			m_screenList.reset();
		}
		if (m_GraphysX) delete(m_GraphysX); m_GraphysX = nullptr;
		m_isRunning = false;
		std::cout << "m_isRunning = false, Exit Game" << std::endl;
		SDL_Quit();
	}
	bool IScreenApp::init()
	{
		initSystems();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();
		m_isRunning = true;
		return true;
	}
	bool IScreenApp::initSystems()
	{
		std::string path = Tools::getexepath();
		m_GraphysX = new IGraphysX(path, 1280, 720);
		std::cout << "Application Path: " << path << std::endl;
		m_fpsLimiter.init(120.0f);
		return true;
	}
	
	void IScreenApp::update()
	{		
		if (m_currentScreen) {
			switch (m_currentScreen->getState())
			{
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				exitGame();
				break;
			default:
				break;
			}
		}
		else {
			std::cout << "Weird, no currentscreen" << std::endl;
			exitGame();
		}
	}
	void IScreenApp::render()
	{
		m_fpsLimiter.beginFrame();

		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) {
			m_currentScreen->render();
		}
		
		

		float currentFps = m_fpsLimiter.endFrame();
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10){
			//std::cout << currentFps << std::endl;
			frameCounter = 0;
		}
	}
	void IScreenApp::onSDLEvent(SDL_Event& evnt) 
	{
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) 
		{
			m_currentScreen->m_gui.Update(m_GraphysX->getTimeElapsed());
			m_currentScreen->m_gui.onSDLEvent(evnt);
			
			switch (evnt.type) 
			{
			case SDL_QUIT:
				m_currentScreen->m_currentState = GraphysX::ScreenState::EXIT_APPLICATION;
				std::cout << "ScreenState = Exit application" << std::endl;
				break;
			
			case SDL_WINDOWEVENT:
				switch (evnt.window.event) 
				{
				case SDL_WINDOWEVENT_RESIZED:
					// we tell CEGUI and TV3D that the window size changed
					m_currentScreen->m_gui.getRenderer()->setDisplaySize(CEGUI::Sizef((float)evnt.window.data1,(float)evnt.window.data2));
					m_GraphysX->clTV3D.pTV.GetViewport()->Resize();
					break;
				case SDL_WINDOWEVENT_ENTER:
					m_GraphysX->clTV3D.pTV.ShowWinCursor(false);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					m_GraphysX->clTV3D.pTV.ShowWinCursor(true);
					break;
				}
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_PRINTSCREEN:
					{
					std::string str = "..\\screenshot_" + Tools::getTimeString();
					m_GraphysX->clTV3D.ScreenShot((char*)str.c_str(), cCONST_TV_IMAGEFORMAT::cTV_IMAGE_PNG);
					std::cout << "screenshot:  " << str << std::endl;
					break;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}