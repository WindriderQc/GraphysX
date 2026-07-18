#pragma once
#include "StdAfx.h"

#include <CEGUI.h> 
#include <CEGUIImageset.h> 
#include <CEGUISystem.h> 
#include <CEGUILogger.h> 
#include <CEGUISchemeManager.h> 
#include <CEGUIWindowManager.h> 
#include <CEGUIWindow.h> 
#include <RendererModules/directx9GUIRenderer/d3d9renderer.h> 
#include <elements/CEGUIPushButton.h> 




class CL_TV3DCEGUI
{
public:
	CL_TV3DCEGUI(void);
	~CL_TV3DCEGUI(void);

	void InitCEGUI(void);
	void Render(void);




CEGUI::Renderer* mGUIRenderer; 
CEGUI::System* mGUISystem; 
//CEGUI::Window* mEditorGuiSheet; 
CEGUI::Window*	background;
LPDIRECT3DDEVICE9 d3Device;

CTVInternalObjects* pTVIntObj;  // Pointer on TV Internal objects

};
