#pragma once
#include "StdAfx.h"

#include <CEGUI.h> 
#include <CEGUIImageset.h> 
#include <CEGUISystem.h> 
#include <CEGUILogger.h> 
#include <CEGUISchemeManager.h> 
#include <CEGUIWindowManager.h> 
#include <CEGUIWindow.h> 
#include <..\RendererModules\directx9GUIRenderer\d3d9renderer.h>

#include "NoteMgrGUI.h"
#include "CarXGUI.h"

#define NOTEMGR_GUIMODE 2000
#define CARX_GUIMODE    2001



class CLTV3DCEGUI
{
private:
	CLTV3DCEGUI(void);
	~CLTV3DCEGUI(void);

public:
	void InitCEGUI(void);
	void Render(void);
	void EnableGUI(CEGUI::Window* mLayoutWnd);
	void DisableActiveGUI(void);
	void SelectGUI(int iGUI_MODE);

	
	// Get n Set Function
	LPDIRECT3DDEVICE9 Get3DDevice(void) { return(d3Device); }
	CEGUI::System* GetGUISystem(void) { return(mGUISystem); }

	
	// Fonctions de création et destruction du singleton
	static CLTV3DCEGUI *getInstance ()
	{
    if (NULL == _singleton)
      {
        _singleton =  new CLTV3DCEGUI;
      }
    return _singleton;
	}
	static void kill ()
	{
    if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
	}

private:
	static CLTV3DCEGUI *_singleton;

	CTVInternalObjects* pTVIntObj;  // Pointer on TV3D Internal objects
	LPDIRECT3DDEVICE9 d3Device;
	CEGUI::Renderer* mGUIRenderer;
	CEGUI::System* mGUISystem; 
	CEGUI::DefaultWindow* mGUISheet; 
	CEGUI::Window* mDefaultGUIWnd;
	
	CEGUI::Window* mActiveGUILayoutWnd;
	// GUI Modules
	CLNoteMgrGUI* clNoteMgrGUI;
	CLCarXGUI* clCarXGUI;

};
