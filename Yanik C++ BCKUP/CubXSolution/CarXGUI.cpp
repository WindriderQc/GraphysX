#include "StdAfx.h"
#include "CarXGUI.h"


CLCarXGUI::CLCarXGUI(void)
{
	using namespace CEGUI;
	DefaultWindow* mGUISheet = (DefaultWindow*)WindowManager::getSingleton().getWindow("gui_sheet");  // Get the CEGUI root window
	
	// Load the layout Window which we will attach all the widgets to.
	mLayoutWnd = WindowManager::getSingleton().loadWindowLayout("GUI\\CarXGUI.layout");
	// attach the LayoutWindow to the root
	mGUISheet->addChildWindow(mLayoutWnd);	
	mLayoutWnd->setProperty("Visible","False");
	
	Init();

}

CLCarXGUI::~CLCarXGUI(void)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Launch" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_LaunchButtonPressed(const CEGUI::EventArgs& e)
{
	//  TROUVER COMMENT ENVOYÉ L'INFO DE CONFIG AU CARSMANAGER
	CLArea::getInstance()->Load3DScene(CAR_SCENE);
	
	return true;
}



void CLCarXGUI::Init(void)
{
	using namespace CEGUI;
	// Set Widgets event
	PushButton* mLaunchBtn = (PushButton*)WindowManager::getSingleton().getWindow("CarX/Welcome/bLaunch");
	mLaunchBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_LaunchButtonPressed);	// Add the "Launch" callback for this button

	
}

