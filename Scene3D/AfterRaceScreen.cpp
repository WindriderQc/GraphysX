#include "stdafx.h"
#include "AfterRaceScreen.h"
#include "Anneaux.h"
#include "IScreenApp.h"
#include <iostream>
#include <GraphysX\Tools.h>
#include <SDL\SDL.h>

namespace BallZ
{
	AfterRaceScreen::AfterRaceScreen() {
		m_ScreenIndex = SCREEN_INDEX_AFTERRACE;
	}
	AfterRaceScreen::~AfterRaceScreen() {
	}
	int AfterRaceScreen::getNextScreenIndex() const {
		return m_nextScreenIndex;
	}
	int AfterRaceScreen::getPreviousScreenIndex() const {
		return SCREEN_INDEX_NO_SCREEN;
	}
	void AfterRaceScreen::render() {
		GraphysX::CLTV3D::renderBegin();
		m_graphysX->renderActiveScene();
		m_gui.Draw();
		//m_graphysX->MouseHighlight();// Find3DObjectWithName("BallZ")->Mesh->GetPosition().y);
		GraphysX::CLTV3D::renderEnd();
		//////////// 
	}
	void AfterRaceScreen::update() {
		m_graphysX->Update();
		checkInput();

	}
	void AfterRaceScreen::checkInput() {
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			m_screenApp->onSDLEvent(evnt);
			switch (evnt.type) {
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << "." << evnt.motion.y << std::endl;
				break;
			default:
				break;
			}
		}
	}
	void AfterRaceScreen::build() {
		m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice(), "AfterRaceScreen.layout");

		CEGUI::PushButton* NextRaceButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindow/ButtonNextLvl")); // 
		NextRaceButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&AfterRaceScreen::onNextRaceClick, this));

		CEGUI::PushButton* MainMenuButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindow/ButtonMainMenu")); // 
		MainMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&AfterRaceScreen::onMainMenuClick, this));

	}
	void AfterRaceScreen::destroy(){
	}
	void AfterRaceScreen::onEntry() {
		std::cout << "OnEntry - AfterRace\n";

		CEGUI::Window* TextTotalTime = static_cast<CEGUI::Window*>(m_gui.GetLayoutWnd()->getChild("FrameWindow/Time/TextTotalTime")); // 
		TextTotalTime->setText(Tools::to_string(m_graphysX->getSceneTime()));

		CEGUI::Window* TextCollected = static_cast<CEGUI::Window*>(m_gui.GetLayoutWnd()->getChild("FrameWindow/Rings/TextCollected")); // 

		std::ostringstream os;
		os << CLAnneaux::getScore() << " / " << CLAnneaux::getMaxRing();
		TextCollected->setText(os.str());

	}
	void AfterRaceScreen::onExit(){
		std::cout << "OnExit - AfterRace\n";
	}
	bool AfterRaceScreen::onNextRaceClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}
	bool AfterRaceScreen::onMainMenuClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_MAINMENU;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}

}
