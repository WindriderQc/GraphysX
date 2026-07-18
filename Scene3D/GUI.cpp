#include "stdafx.h"
#include "GUI.h"


GUI::GUI(Engine3DWrapper^ tools3d)
{
	Tools3D = tools3d;
}

void GUI::Init(const std::string& scheme, const std::string& font, const std::string& MouseCursor, std::string PathToLayout)
{
	mRootWindow = Tools3D->m_CEGUI->getContext()->getRootWindow(); 

	if (scheme == "") Tools3D->m_CEGUI->LoadScheme("AlfiskoSkin.scheme");
	else Tools3D->m_CEGUI->LoadScheme(scheme);

	if (font == "") Tools3D->m_CEGUI->SetFont("DejaVuSans-10");
	else Tools3D->m_CEGUI->SetFont(font);

	if (MouseCursor == "")	Tools3D->m_CEGUI->SetMouseCursor("AlfiskoSkin/MouseArrow"); 
	else Tools3D->m_CEGUI->SetMouseCursor(MouseCursor);

	if (!(PathToLayout == "")){
		mLayoutWnd = Tools3D->m_CEGUI->LoadLayout(PathToLayout); // Load the layout Window which we will attach all the widgets to.
		mRootWindow->addChild(mLayoutWnd); 
		mLayoutWnd->setProperty("Visible", "False");
	} 
	
	//LoadElements();
}

void GUI::ShowGUI(bool show)
{
	if (show)
		mLayoutWnd->setProperty("Visible", "True");
	else
		mLayoutWnd->setProperty("Visible", "False");
}


/*
#include "SetupGUI.h"

#include "Area.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "SelectSkybox" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_SelectSkyboxButtonPressed(const CEGUI::EventArgs& e)
{
//  TROUVER COMMENT ENVOYÉ L'INFO DE CONFIG AU CARSMANAGER
CLArea::getInstance()->Load3DScene(MODULE_LOADING_SCENE);
return true;
}


void CLSetupGUI::Init(void)
{
using namespace CEGUI;
// Set Widgets event
PushButton* mLaunchBtn = (PushButton*)WindowManager::getSingleton().getWindow("SetupWnd/SkyboxBtn");
mLaunchBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_SelectSkyboxButtonPressed);
}

*/
