#pragma once
#include <vector>

namespace GraphysX
{
	class IScreenApp;
	class IScreen3D;

	class ScreenList
	{
	public:
		ScreenList(IScreenApp* game);
		~ScreenList();
		
		IScreen3D* moveNext();
		IScreen3D* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IScreen3D* newScreen);
		IScreen3D *getCurrent();
		
		void destroy();

	protected:
		IScreenApp* m_screenApp = nullptr;
		std::vector<IScreen3D*> m_screens;
		int m_currentScreenIndex = -1;
	};
}
