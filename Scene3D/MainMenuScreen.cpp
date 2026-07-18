#include "stdafx.h"
#include "MainMenuScreen.h"
#include "IScreenApp.h"
#include <SDL\SDL.h>

// TODO:  Pkoi le light indicator mesh est visible tout le temps??
// TODO: le soleil  passe très proche de la cam dans sa rotation et ce n'est pas très nice
// TODO: Ajouter un land pour creer un effet que le soleil va se cacher derriere quelques chose.

namespace BallZ
{
	MainMenuScreen::MainMenuScreen() {
		m_ScreenIndex = SCREEN_INDEX_MAINMENU;
	}
	MainMenuScreen::~MainMenuScreen() {
	}
	int MainMenuScreen::getNextScreenIndex() const {
		return m_nextScreenIndex;
	}
	int MainMenuScreen::getPreviousScreenIndex() const {
		return SCREEN_INDEX_NO_SCREEN;
	}
	void MainMenuScreen::render() {
		GraphysX::CLTV3D::renderBegin();
		m_graphysX->renderActiveScene();
		m_gui.Draw();
		GraphysX::CLTV3D::renderEnd();
	}
	void MainMenuScreen::update() {
		m_graphysX->Update();
		checkInput();
		m_graphysX->pActiveScene->clCamera.pCamera->RotateY(0.01f * GraphysX::CLTV3D::getTimeElapsed());
	}
	void MainMenuScreen::checkInput() {
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
	void MainMenuScreen::build() {
		m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice(), "MainMenuScreen.layout", "AlfiskoSkin.scheme", "", "AlfiskoSkin/MouseArrow");
		/*m_gui.LoadScheme("AlfiskoSkin.scheme");*/
		/*CEGUI::PushButton* StartGameButton;
		StartGameButton = static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("TaharezLook/Button",
		cTV_4DVECTOR(0.3f, 0.6f, 0.4f, 0.1f), cTV_4DVECTOR(0, 0, 0, 0), "StartGameButton"));
		StartGameButton->setText("Start Game");*/
		CEGUI::PushButton* StartGameButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("bBallZ")); // 
		StartGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onStartGameClick, this));

		CEGUI::PushButton* EditorButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("bMapEditor")); // 
		EditorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onEditorClick, this));

		CEGUI::PushButton* MathGameButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("bMathGame")); // 
		MathGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onMathClick, this));

		CEGUI::PushButton* ExitButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("bExit")); // 
		ExitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClick, this));
	}
	void MainMenuScreen::destroy(){
	}
	void MainMenuScreen::onEntry() {
		m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = false;      
		m_graphysX->pActiveScene->clCamera.pCamera->SetCamera(0, 0, 0, 0, 20, 1000);// TODO : CLCAMERA improve....  le pCamera qui est la default camera de TV3D devrait etre static CLTV3D::CLCamera::pCamera.blablaba   ou pActiveScene.pCamera serait ptete mieux?
		std::cout << "OnEntry - MainMenu\n";
	}
	void MainMenuScreen::onExit(){
		std::cout << "OnExit - MainMenu\n";
	}
	bool MainMenuScreen::onStartGameClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_SELECTRACE;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}
	bool MainMenuScreen::onEditorClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_EDITOR;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}
	bool MainMenuScreen::onMathClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_MATHGAME;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}
	bool MainMenuScreen::onExitClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_NO_SCREEN;
		m_currentState = GraphysX::ScreenState::EXIT_APPLICATION;
		return true;
	}

}