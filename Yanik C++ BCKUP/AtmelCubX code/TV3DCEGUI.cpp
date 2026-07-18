#include "tv3dcegui.h"
#include "Global.h"

#pragma warning(disable: 4005) // _RPTF0' : macro redefinition

// Initialisation du singleton à NULL
CLTV3DCEGUI *CLTV3DCEGUI::_singleton = NULL;

CLGlobalVar* clGlobalVar = CLGlobalVar::getInstance();

CLTV3DCEGUI::CLTV3DCEGUI(void)
{
	d3Device = NULL;
	pTVIntObj = new CTVInternalObjects();
	
	mActiveGUILayoutWnd = NULL;
	InitCEGUI();
}

CLTV3DCEGUI::~CLTV3DCEGUI(void)
{
delete(mGUIRenderer); mGUIRenderer = NULL;
delete(pTVIntObj);  pTVIntObj = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Quit" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_QuitButtonPressed(const CEGUI::EventArgs& e)
{
	PostQuitMessage(0);	  //  Marche pas....   le message ne se rends pas...!?!?
	return true;
}


void CLTV3DCEGUI::InitCEGUI(void)
{

d3Device = pTVIntObj->GetDevice3D(); //  get device from TV3D

using namespace CEGUI; 

mGUIRenderer = new DirectX9Renderer(d3Device, 0); 
mGUISystem = new System(mGUIRenderer); 
Logger::getSingleton().setLogFilename("cegui.log", true); 
Logger::getSingleton().setLoggingLevel(CEGUI::Insane); 

// Load scheme and set up defaults
SchemeManager::getSingleton().loadScheme("GUI\\TaharezLook.scheme"); 
mGUISystem->setDefaultMouseCursor("TaharezLook", "MouseArrow"); 
FontManager::getSingleton().createFont("GUI\\Commonwealth-10.font"); 

WindowManager& winMgr = WindowManager::getSingleton();

	// We use a StaticImage as the root (the image is not loaded, then it's free)
	mGUISheet = (DefaultWindow*)winMgr.createWindow("TaharezLook/StaticImage", "gui_sheet");
	mGUISheet->setPosition(UVector2(UDim(0, 0), UDim(0,0)));											// set position
	mGUISheet->setSize(UVector2(UDim(1, 0), UDim(1,0)));// set size
	mGUISheet->setProperty("BackgroundEnabled", "false");  // disable standard background 
	mGUISheet->setProperty("FrameEnabled", "false");       // disable frame									
	
	System::getSingleton().setGUISheet(mGUISheet);									// install this as the root GUI sheet

	// A MODIFIER POUR FAIRE UNE CLASSE DEFAULT GUI DONT HERITERA LES AUTRE CLASSE GUI
	// Create a DefaultWindow which we will attach all the default widgets to.
	mDefaultGUIWnd = WindowManager::getSingleton().loadWindowLayout("GUI\\DefaultGUI.layout");
	// attach the window to the 'real' root
	mGUISheet->addChildWindow(mDefaultGUIWnd);					


	// Set Widgets event for DefaultGUI
	PushButton* mQuitBtn = (PushButton*)WindowManager::getSingleton().getWindow("QuitBtn");
	mQuitBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_QuitButtonPressed);	// Add the "quit" callback for this button


	// Load all needed GUI Modules             ****  devrait ptete etre caller seulement a l'utilisation
//	clNoteMgrGUI = new CLNoteMgrGUI();
	clCarXGUI = new CLCarXGUI();
	clSetupGUI = new CLSetupGUI("..\\debug\\GUI\\SetupGUI.layout");
}
void CLTV3DCEGUI::Render(void)
{
CEGUI::System::getSingleton().renderGUI();   //  CEGUI RENDER
d3Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}


void CLTV3DCEGUI::EnableGUI(CEGUI::Window* mLayoutWnd)
{
	mDefaultGUIWnd->setProperty("Visible", "False");
	mLayoutWnd->setProperty("Visible","True");
	mActiveGUILayoutWnd = mLayoutWnd;
}

void  CLTV3DCEGUI::DisableActiveGUI(void)
{
	/*	
	if(mActiveGUILayoutWnd == NULL)
		MessageBoxA(NULL, "No GUI loaded, nothing to disable...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);
	else
		mActiveGUILayoutWnd->setProperty("Visible","False");
	*/
	
	if(mActiveGUILayoutWnd != NULL)
		mActiveGUILayoutWnd->setProperty("Visible","False");
	mDefaultGUIWnd->setProperty("Visible", "True");
	mActiveGUILayoutWnd = NULL;
}

void CLTV3DCEGUI::SelectGUI(int iGUI_MODE)
{
	DisableActiveGUI();

	switch(iGUI_MODE)
	{
	
	case NOTEMGR_GUIMODE:
//			EnableGUI(clNoteMgrGUI->GetRootWnd());	
			break;


	case CARX_GUIMODE:
			EnableGUI(clCarXGUI->GetRootWnd());	
			break;
	case SETUP_GUIMODE:
			EnableGUI(clSetupGUI->GetRootWnd());	
			break;
	default:
			EnableGUI(mDefaultGUIWnd);
			break;

	}
}