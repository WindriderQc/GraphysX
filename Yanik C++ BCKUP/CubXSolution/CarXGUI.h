#pragma once
#include "StdAfx.h"
#include "Area.h"

#include <CEGUI.h> 
#include <CEGUIImageset.h> 
#include <CEGUIWindowManager.h> 
#include <CEGUIWindow.h> 


//  CREER UNE CLASSE GUI pour en faire hériter les autres...
class CLCarXGUI
{
public:
	CLCarXGUI(void);
	~CLCarXGUI(void);

	void Init(void);
	
	CEGUI::Window* GetRootWnd(void) { return(mLayoutWnd); }

private:
	CEGUI::Window* mLayoutWnd;
};
