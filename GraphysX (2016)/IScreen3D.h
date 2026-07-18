#pragma once
#include "IGraphysX.h"
#include "CLCEGUI.h"


#define SCREEN_INDEX_NO_SCREEN -1   //  Other SCREEN INDEX are defined in the application


namespace GraphysX
{
	enum class ScreenState {
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IScreenApp;
	class CLCEGUI;

	class IScreen3D
	{
	public:
		friend class ScreenList;
		IScreen3D(){};
		virtual ~IScreen3D(){};

		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		virtual void render() = 0;
		virtual void update() = 0;
		//called at beginning and end of application
		virtual void build() = 0;
		virtual void destroy() = 0;
		// called on screen focus changes
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		int getScreenIndex() const { return m_ScreenIndex; }
		void setRunning() { m_currentState = ScreenState::RUNNING; }
		ScreenState getState() const { return m_currentState; }

		void setParentGraphApp(IScreenApp* graphApp) { m_screenApp = graphApp; }
		void setGraphysX(IGraphysX* graphysx) { m_graphysX = graphysx; }

		CLCEGUI m_gui;
		ScreenState m_currentState = ScreenState::NONE;
	
	protected: 
		int m_ScreenIndex = -1;
		IScreenApp* m_screenApp = nullptr;
		IGraphysX* m_graphysX = nullptr;
		
	};
}
