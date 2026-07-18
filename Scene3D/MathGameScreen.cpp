#include "stdafx.h"
#include "MathGameScreen.h"
#include "IScreenApp.h"
#include <SDL\SDL.h>

#define RANGEMAX 10
#define RANGEOFFSET RANGEMAX / 2

namespace BallZ
{
	MathGameScreen::MathGameScreen() {
		m_ScreenIndex = SCREEN_INDEX_MATHGAME;
	}
	MathGameScreen::~MathGameScreen() {
	}
	int MathGameScreen::getNextScreenIndex() const {
		return m_nextScreenIndex;
	}
	int MathGameScreen::getPreviousScreenIndex() const {
		return SCREEN_INDEX_MAINMENU;
	}
	void MathGameScreen::render() {
		GraphysX::CLTV3D::renderBegin();
		m_graphysX->renderActiveScene();
		m_gui.Draw();
		GraphysX::CLTV3D::renderEnd();
	}
	void MathGameScreen::update() {
		m_graphysX->Update();
		checkInput();

		formulas.a = sliderA->getCurrentValue() - RANGEOFFSET;
		formulas.b = sliderB->getCurrentValue() - RANGEOFFSET;
		formulas.c = sliderC->getCurrentValue() - RANGEOFFSET;
		formulas.m = sliderM->getCurrentValue() - RANGEOFFSET;
		formulas.xOffset = sliderX->getCurrentValue() - RANGEOFFSET;

		formulas.moleculesUpdate();
	}
	void MathGameScreen::checkInput() {
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			m_screenApp->onSDLEvent(evnt);
			switch (evnt.type) {
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << "." << evnt.motion.y << std::endl;
				break;
			case SDL_KEYDOWN:
			{
				switch (evnt.key.keysym.sym)
				{
				case SDLK_c:
				{
					m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = !m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl;
				}
				break;
				case SDLK_f:
				{
					string input = "";
					// How to get a number.
					double myNumber = 0;
					/*while (true) {
					cout << "Please enter a M value: ";
					getline(cin, input);
					// This code converts from string to number safely.
					stringstream myStream(input);
					if (myStream >> myNumber)
					break;
					cout << "Invalid number, please try again" << endl;
					}
					formulas.m = myNumber;
					cout << endl;

					myNumber = 0;*/
					while (true) {
						cout << "Please enter a A value: ";
						getline(cin, input);
						// This code converts from string to number safely.
						stringstream myStream(input);
						if (myStream >> myNumber)
							break;
						cout << "Invalid number, please try again" << endl;
					}
					formulas.a = myNumber;
					cout << endl;
					myNumber = 0;
					while (true) {
						cout << "Please enter a B value: ";
						getline(cin, input);
						// This code converts from string to number safely.
						stringstream myStream(input);
						if (myStream >> myNumber)
							break;
						cout << "Invalid number, please try again" << endl;
					}
					formulas.b = myNumber;
					cout << endl;
					myNumber = 0;
					while (true) {
						cout << "Please enter a C value: ";
						getline(cin, input);
						// This code converts from string to number safely.
						stringstream myStream(input);
						if (myStream >> myNumber)
							break;
						cout << "Invalid number, please try again" << endl;
					}
					formulas.c = myNumber;
					cout << endl << endl;

					/*
					// How to get a string/sentence with spaces
					cout << "Please enter a valid sentence (with spaces):\n>";
					getline(cin, input);
					cout << "You entered: " << input << endl << endl;

					// How to get a single char.
					char myChar = { 0 };

					while (true) {
					cout << "Please enter 1 char: ";
					getline(cin, input);

					if (input.length() == 1) {
					myChar = input[0];
					break;
					}

					cout << "Invalid character, please try again" << endl;
					}
					cout << "You entered: " << myChar << endl << endl;
					cout << "All done. And without using the >> operator" << endl;*/
				}
				break;
				}
			}
			}
		}
	}
	void MathGameScreen::build() {
		m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice(), "MathGameScreen.layout");  // TODO : check pkoi lui marche avec les slider de Alfiskoskin sans loader un autre theme.....  vs mainMenu 

		//CEGUI::PushButton* StartRaceButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("StaticImage/GroupBox/ButtonStartRace")); // 
	//	StartRaceButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SelectRaceScreen::onStartRaceClick, this));
		CEGUI::PushButton* PreviousButton = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("ButtonPrevious")); // 
		PreviousButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MathGameScreen::onPreviousClick, this));

		sliderA = static_cast<CEGUI::Slider*>(m_gui.GetLayoutWnd()->getChild("VerticalSliderA")); // 
		sliderA->subscribeEvent(CEGUI::Slider::EventMouseButtonUp, CEGUI::Event::Subscriber(&MathGameScreen::onChangeSliderA, this));
		sliderA->setMaxValue(RANGEMAX);
		sliderA->setCurrentValue(RANGEOFFSET);
		sliderB = static_cast<CEGUI::Slider*>(m_gui.GetLayoutWnd()->getChild("VerticalSliderB")); // 
		sliderB->subscribeEvent(CEGUI::Slider::EventMouseButtonUp, CEGUI::Event::Subscriber(&MathGameScreen::onChangeSliderB, this));
		sliderB->setMaxValue(RANGEMAX);
		sliderB->setCurrentValue(RANGEOFFSET);
		sliderC = static_cast<CEGUI::Slider*>(m_gui.GetLayoutWnd()->getChild("VerticalSliderC")); // 
		sliderC->subscribeEvent(CEGUI::Slider::EventMouseButtonUp, CEGUI::Event::Subscriber(&MathGameScreen::onChangeSliderC, this));
		sliderC->setMaxValue(RANGEMAX);
		sliderC->setCurrentValue(RANGEOFFSET);
		sliderM = static_cast<CEGUI::Slider*>(m_gui.GetLayoutWnd()->getChild("VerticalSliderM")); // 
		sliderM->subscribeEvent(CEGUI::Slider::EventMouseButtonUp, CEGUI::Event::Subscriber(&MathGameScreen::onChangeSliderM, this));
		sliderM->setMaxValue(RANGEMAX);
		sliderM->setCurrentValue(RANGEOFFSET);
		sliderX = static_cast<CEGUI::Slider*>(m_gui.GetLayoutWnd()->getChild("HorizontalSliderX")); // 
		sliderX->subscribeEvent(CEGUI::Slider::EventMouseButtonUp, CEGUI::Event::Subscriber(&MathGameScreen::onChangeSliderX, this));
		sliderX->setMaxValue(RANGEMAX);
		sliderX->setCurrentValue(RANGEOFFSET);

	}
	void MathGameScreen::destroy(){
	}
	void MathGameScreen::onEntry() {
		m_graphysX->pActiveScene->clCamera.pCamera->SetCamera(0, 0, -200, 0, 50, 100);
		m_graphysX->pActiveScene->clSky.setCurrentSky(&m_graphysX->pActiveScene->clSky.loadSkybox("NightSky", true));
		m_graphysX->pActiveScene->clSky.setDayNightEnable(false);
		formulas.moleculesCreate();
		std::cout << "OnEntry - MathGame\n";
	}
	void MathGameScreen::onExit(){
		formulas.moleculesDelete();
		std::cout << "OnExit - MathGame\n";
	}

	bool MathGameScreen::onChangeSliderA(const CEGUI::EventArgs& e) {
		return true;
	}
	bool MathGameScreen::onChangeSliderB(const CEGUI::EventArgs& e) {
		return true;
	}
	bool MathGameScreen::onChangeSliderC(const CEGUI::EventArgs& e) {
		return true;
	}
	bool MathGameScreen::onChangeSliderM(const CEGUI::EventArgs& e) {
		return true;
	}
	bool MathGameScreen::onChangeSliderX(const CEGUI::EventArgs& e) {
		return true;
	}

	bool MathGameScreen::onPreviousClick(const CEGUI::EventArgs& e) {
		m_currentState = GraphysX::ScreenState::CHANGE_PREVIOUS;
		return true;
	}
}