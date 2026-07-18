#pragma once
#include "IScreen3D.h"
#include "ScreenIndices.h"

namespace BallZ
{
	class MainMenuScreen : public GraphysX::IScreen3D
	{
	public:
		MainMenuScreen();
		~MainMenuScreen();

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

		bool onStartGameClick(const CEGUI::EventArgs& e);
		bool onEditorClick(const CEGUI::EventArgs& e);
		bool onMathClick(const CEGUI::EventArgs& e);
		bool onExitClick(const CEGUI::EventArgs& e);

	private:
		virtual void checkInput();

		int m_nextScreenIndex = SCREEN_INDEX_EDITOR;
	};
}