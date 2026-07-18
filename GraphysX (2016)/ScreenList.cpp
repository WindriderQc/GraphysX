#include "stdafx.h"
#include "ScreenList.h"
#include "IScreen3D.h"
#include "IScreenApp.h"

namespace GraphysX
{
	ScreenList::ScreenList(IScreenApp* game) 
		: m_screenApp(game){
	}
	ScreenList::~ScreenList(){
		destroy();
	}
	IScreen3D* ScreenList::moveNext(){
		IScreen3D* currentScreen = getCurrent();
		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getNextScreenIndex();
		}
		return getCurrent();
	}
	IScreen3D* ScreenList::movePrevious(){
		IScreen3D* currentScreen = getCurrent();
		if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
		}
		return getCurrent();
	}
	void ScreenList::destroy(){
		for (size_t i = 0; i < m_screens.size(); i++)
		{
			m_screens[i]->destroy();
		}
		m_screens.resize(0);
		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	}
	void ScreenList::setScreen(int nextScreen){
		m_currentScreenIndex = nextScreen;
	}
	void ScreenList::addScreen(IScreen3D* newScreen){
		newScreen->m_ScreenIndex = m_screens.size();
		m_screens.push_back(newScreen);
		newScreen->build();
		newScreen->setParentGraphApp(m_screenApp);
		newScreen->setGraphysX(m_screenApp->getGraphysX());
	}
	IScreen3D* ScreenList::getCurrent(){
		if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
		return m_screens[m_currentScreenIndex];
	}
}