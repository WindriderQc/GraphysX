#pragma once
#include "IScreen3D.h"
#include "ScreenIndices.h"

namespace BallZ
{
	class AfterRaceScreen : public GraphysX::IScreen3D
	{

	public:
		AfterRaceScreen();
		~AfterRaceScreen();

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

		virtual void checkInput();

		bool onNextRaceClick(const CEGUI::EventArgs& e);
		bool onMainMenuClick(const CEGUI::EventArgs& e);


	private:
		int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	};
}