#pragma once
#include "IScreen3D.h"
#include "ScreenIndices.h"

namespace BallZ
{
	class SelectRaceScreen : public GraphysX::IScreen3D
	{
	public:
		SelectRaceScreen();
		~SelectRaceScreen();

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
		virtual void checkInput();
		int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
		bool onStartRaceClick(const CEGUI::EventArgs& e);
		bool onPreviousClick(const CEGUI::EventArgs& e);
	};
}

