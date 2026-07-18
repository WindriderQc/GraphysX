#include "stdafx.h"
#include "App.h"


	App::App()
	{}
	App::~App()
	{}
	void App::onInit() 
	{
		if (!init())
			MSGBOX("Error", "Could not init App");
	}
	void App::addScreens() 
	{
		m_mainMenuScreen = std::make_unique<BallZ::MainMenuScreen>();
		m_screenList->addScreen(m_mainMenuScreen.get());
		m_gameplayScreen = std::make_unique<BallZ::GameplayScreen>();
		m_screenList->addScreen(m_gameplayScreen.get());
		m_editorScreen = std::make_unique<BallZ::EditorScreen>();
		m_screenList->addScreen(m_editorScreen.get());
		m_afterRaceScreen = std::make_unique<BallZ::AfterRaceScreen>();
		m_screenList->addScreen(m_afterRaceScreen.get());	
		m_selectRaceScreen = std::make_unique<BallZ::SelectRaceScreen>();
		m_screenList->addScreen(m_selectRaceScreen.get());
		m_mathGameScreen = std::make_unique<BallZ::MathGameScreen>();
		m_screenList->addScreen(m_mathGameScreen.get());
		
		m_screenList->setScreen(m_mainMenuScreen->getScreenIndex());
	}
	void App::onExit() {
		std::cout << "App onExit" << std::endl;   // TODO: check - seems never called
		system("PAUSE");
	}
