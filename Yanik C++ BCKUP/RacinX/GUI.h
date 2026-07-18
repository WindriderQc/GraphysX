#pragma once
#include "StdAfx.h"

#include <CEGUI.h> 
#include <CEGUIImageset.h> 
#include <CEGUIWindowManager.h> 
#include <CEGUIWindow.h> 


class CLGUI
{

public:
	CLGUI(string PathToLayout)
		{
		using namespace CEGUI;
		// Get the CEGUI root window
		DefaultWindow* mGUISheet = (DefaultWindow*)WindowManager::getSingleton().getWindow("gui_sheet");  
		// Load the layout Window which we will attach all the widgets to.
		mLayoutWnd = WindowManager::getSingleton().loadWindowLayout(PathToLayout);
		// attach the LayoutWindow to the root
		mGUISheet->addChildWindow(mLayoutWnd);	
		mLayoutWnd->setProperty("Visible","False");
		};
	virtual ~CLGUI(void)
		{};

	CEGUI::Window* GetRootWnd(void) { return(mLayoutWnd); }

protected:
	CEGUI::Window* mLayoutWnd;
};
