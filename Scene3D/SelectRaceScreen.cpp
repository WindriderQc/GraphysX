#include "stdafx.h"
#include "SelectRaceScreen.h"
#include "IScreenApp.h"
#include <SDL\SDL.h>


namespace BallZ
{
	SelectRaceScreen::SelectRaceScreen() {
		m_ScreenIndex = SCREEN_INDEX_SELECTRACE;
	}
	SelectRaceScreen::~SelectRaceScreen() {
	}
	int SelectRaceScreen::getNextScreenIndex() const {
		return m_nextScreenIndex;
	}
	int SelectRaceScreen::getPreviousScreenIndex() const {
		return SCREEN_INDEX_MAINMENU;
	}
	void SelectRaceScreen::render() {
		GraphysX::CLTV3D::renderBegin();
		m_graphysX->renderActiveScene();
		m_gui.Draw();
		GraphysX::CLTV3D::renderEnd();
	}
	void SelectRaceScreen::update() {
		m_graphysX->Update();
		checkInput();
	}
	void SelectRaceScreen::checkInput() {
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
	void SelectRaceScreen::build() {
		m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice(), "SetRaceScreen.layout");

		CEGUI::PushButton* StartRaceButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("StaticImage/GroupBox/ButtonStartRace")); // 
		StartRaceButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SelectRaceScreen::onStartRaceClick, this));
		CEGUI::PushButton* PreviousButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("StaticImage/ButtonPrevious")); // 
		PreviousButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SelectRaceScreen::onPreviousClick, this));
	}
	void SelectRaceScreen::destroy(){
	}
	void SelectRaceScreen::onEntry() {
		std::cout << "OnEntry - SelectRace\n";
	}
	void SelectRaceScreen::onExit(){
		std::cout << "OnExit - SelectRace\n";
	}
	bool SelectRaceScreen::onStartRaceClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}
	bool SelectRaceScreen::onPreviousClick(const CEGUI::EventArgs& e) {
		m_currentState = GraphysX::ScreenState::CHANGE_PREVIOUS;
		return true;
	}
}