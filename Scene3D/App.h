#pragma once
#include "IScreenApp.h"
#include "ScreenList.h"
#include "GamePlayScreen.h"
#include "MainMenuScreen.h"
#include "EditorScreen.h"
#include "AfterRaceScreen.h"
#include "MathGameScreen.h"
#include "SelectRaceScreen.h"


	class App : public GraphysX::IScreenApp
	{
	public:
		App();
		~App();

		virtual void onInit() override;
		virtual void addScreens() override;
		virtual void onExit() override;
	private:
		std::unique_ptr<BallZ::GameplayScreen> m_gameplayScreen = nullptr;
		std::unique_ptr<BallZ::MainMenuScreen> m_mainMenuScreen = nullptr;
		std::unique_ptr<BallZ::SelectRaceScreen> m_selectRaceScreen = nullptr;
		std::unique_ptr<BallZ::EditorScreen> m_editorScreen = nullptr;
		std::unique_ptr<BallZ::AfterRaceScreen> m_afterRaceScreen = nullptr;
		std::unique_ptr<BallZ::MathGameScreen> m_mathGameScreen = nullptr;
	};
