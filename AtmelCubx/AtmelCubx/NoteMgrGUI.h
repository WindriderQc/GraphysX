#pragma once
#include "StdAfx.h"

#include <CEGUI.h> 
#include <CEGUIImageset.h> 
#include <CEGUIWindowManager.h> 
#include <CEGUIWindow.h> 




class CLNoteMgrGUI
{
public:
	CLNoteMgrGUI(void);
	~CLNoteMgrGUI(void);

	void Init(void);
	void Enable(void);
	void Disable(void);
	
	CEGUI::Window* GetRootWnd(void) { return(mLayoutWnd); }

private:
	CEGUI::Window* mLayoutWnd;

};
