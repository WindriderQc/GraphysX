#pragma once
#include "IScreen3D.h"
#include "ScreenIndices.h"
#include "Formulas.h"

 namespace BallZ
 {
	 class MathGameScreen : public GraphysX::IScreen3D
	 {
	 public:
		 MathGameScreen();
		 ~MathGameScreen();

		 virtual int getNextScreenIndex() const override;
		 virtual int getPreviousScreenIndex() const override;
		 virtual void render() override;
		 virtual void update() override;
		 //called at beginning and end of application
		 virtual void build() override;
		 virtual void destroy()override;
		 // called on screen focus changes
		 virtual void onEntry() override;
		 virtual void onExit() override;

	 private:
		 MathGame::Formulas formulas;

		 virtual void checkInput();
		 int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
		 bool onChangeSliderA(const CEGUI::EventArgs& e);
		 bool onChangeSliderB(const CEGUI::EventArgs& e);
		 bool onChangeSliderC(const CEGUI::EventArgs& e);
		 bool onChangeSliderM(const CEGUI::EventArgs& e);
		 bool onChangeSliderX(const CEGUI::EventArgs& e);
		 bool onPreviousClick(const CEGUI::EventArgs& e);

		 CEGUI::Slider* sliderA;
		 CEGUI::Slider* sliderB;
		 CEGUI::Slider* sliderC;
		 CEGUI::Slider* sliderM;
		 CEGUI::Slider* sliderX;

	 };
 }