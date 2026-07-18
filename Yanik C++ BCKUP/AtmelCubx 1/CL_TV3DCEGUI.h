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




class CL_TV3DCEGUI
{
private:
	CL_TV3DCEGUI(void);
	~CL_TV3DCEGUI(void);

public:
	void InitCEGUI(void);
	void Render(void);
	void EnableNoteBloc(void);
	
	// Get n Set Function
	LPDIRECT3DDEVICE9 Get3DDevice(void) { return(d3Device); }
	CEGUI::System* GetGUISystem(void) { return(mGUISystem); }

	
	// Fonctions de création et destruction du singleton
	static CL_TV3DCEGUI *getInstance ()
	{
    if (NULL == _singleton)
      {
        _singleton =  new CL_TV3DCEGUI;
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
	CEGUI::Renderer* mGUIRenderer;
	CEGUI::System* mGUISystem; 
	CEGUI::DefaultWindow* mGUISheet; 
	LPDIRECT3DDEVICE9 d3Device;


	CEGUI::FrameWindow* mNoteWindow;

	CTVInternalObjects* pTVIntObj;  // Pointer on TV Internal objects
	
	static CL_TV3DCEGUI *_singleton;
};
