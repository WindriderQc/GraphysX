#pragma once
#include <vector>

namespace GraphysX
{
	class IScreenApp;
	class IScreen3D;

	class ScreenList
	{
	public:
		GRAPHYSX_API ScreenList(IScreenApp* game);
		~ScreenList();
		
		GRAPHYSX_API IScreen3D* moveNext();
		GRAPHYSX_API IScreen3D* movePrevious();

		GRAPHYSX_API void setScreen(int nextScreen);
		GRAPHYSX_API void addScreen(IScreen3D* newScreen);
		GRAPHYSX_API IScreen3D *getCurrent();
		
		void destroy();

	protected:
		IScreenApp* m_screenApp = nullptr;
		std::vector<IScreen3D*> m_screens;
		int m_currentScreenIndex = -1;
	};
}
