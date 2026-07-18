#include "tv3dcegui.h"
#include "Global.h"
#include "Screensaver.h"

#pragma warning(disable: 4005) // _RPTF0' : macro redefinition

// Initialisation du singleton à NULL
CLTV3DCEGUI *CLTV3DCEGUI::_singleton = NULL;

CLGlobalVar* clGlobalVar = CLGlobalVar::getInstance();
CLScreensaver* clScreensaver = CLScreensaver::getInstance();



CLTV3DCEGUI::CLTV3DCEGUI(void)
{
	d3Device = NULL;
	pTVIntObj = new CTVInternalObjects();
	
	mActiveGUILayoutWnd = NULL;
	InitCEGUI();
}

CLTV3DCEGUI::~CLTV3DCEGUI(void)
{
delete(clNoteMgrGUI); clNoteMgrGUI = NULL;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Options" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_OptionsBtnPressed(const CEGUI::EventArgs& e)
{	
	using namespace CEGUI;												// Use CeGUI namespace
	WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

	FrameWindow* w = (FrameWindow*)winMgr.getWindow("SetupWindow");	    // Get the setup window pointer
	w->setProperty("Visible","true");									// Show the Window
	clGlobalVar->SetCEGUIWindowsOpened(true);
	return true;		
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Features" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_FeaturesBtnPressed(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;											// Use CeGUI namespace
	WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

	FrameWindow* w = (FrameWindow*)winMgr.getWindow("FeaturesCfg");	    // Get the setup window pointer
	w->setProperty("Visible","true");									// Show the Window
	clGlobalVar->SetCEGUIWindowsOpened(true);
	return true;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Screensaver" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_ScreensaverBtnPressed(const CEGUI::EventArgs& e)
{
	clGlobalVar->SetScreensaverEnabled(true);

	using namespace CEGUI;											// Use CeGUI namespace
	WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

	FrameWindow* w = (FrameWindow*)winMgr.getWindow("FeaturesCfg");	    // Get the setup window pointer
	w->setProperty("Visible","false");	
	clGlobalVar->SetCEGUIWindowsOpened(false);

	return true;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Screensaver" TimeSelect
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_TimeSelect(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;											
	WindowManager& winMgr = WindowManager::getSingleton();		
	Combobox* w = (Combobox*)winMgr.getWindow("FeaturesCfg/TimeSelect");	    
	
	ListboxItem* Item = w->getSelectedItem();

	size_t Index = w->getItemIndex(Item);
	
	switch(Index)
	{
	case 0:
		clScreensaver->SetTriggerTime(1000);  // 1 sec
		break;
	case 1:
		clScreensaver->SetTriggerTime(2000);  // 2 sec
		break;
	case 2:
		clScreensaver->SetTriggerTime(5000);  // 5 sec
		break;
	case 3:
		clScreensaver->SetTriggerTime(10000);  // 10 sec
		break;
	case 4:
		clScreensaver->SetTriggerTime(15000);  // 15 sec
		break;
	case 5:
		clScreensaver->SetTriggerTime(30000);  // 30 sec
		break;
	}

	return true;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "SetupWindow" close event
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool GUI_Callback_SetupWindowClosed(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;												// Use CeGUI namespace
	WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

	FrameWindow* w = (FrameWindow*) ((const WindowEventArgs&)e).window;	// we know it's a window    // Get the setup window pointer
	w->setProperty("Visible","false");									// Hide the Window
	clGlobalVar->SetCEGUIWindowsOpened(false);
	
	const Checkbox*    cb1 = (const Checkbox*)winMgr.getWindow("ChkBoxShowFPS");	// Get checkbox
	const Checkbox*    cb2 = (const Checkbox*)winMgr.getWindow("ChkBox3DAxis");		// Get checkbox
	const Checkbox*    cb3 = (const Checkbox*)winMgr.getWindow("ChkBoxEnableAtmos");// Get checkbox

	if(cb1->isSelected())  
		clGlobalVar->SetShowFPS(true);
	else clGlobalVar->SetShowFPS(false);  
	
	if(cb2->isSelected())  
		clGlobalVar->SetRender3DAxis(true);
	else clGlobalVar->SetRender3DAxis(false);
	
	if(cb3->isSelected())  
		clGlobalVar->SetRenderAtmos(true);
	else clGlobalVar->SetRenderAtmos(false);

	return true;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "FeaturesWindow" close event
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool GUI_Callback_FeaturesWindowClosed(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;												// Use CeGUI namespace
	WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

	FrameWindow* w = (FrameWindow*) ((const WindowEventArgs&)e).window;	// we know it's a window    // Get the setup window pointer
	w->setProperty("Visible","false");									// Hide the Window
	clGlobalVar->SetCEGUIWindowsOpened(false);

	return true;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the slider (changes alpha for the GUIs)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static	bool GUI_Callback_AlphaSlider(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;																// Use CeGUI namespace
	Slider* s = (Slider*) ((const WindowEventArgs&)e).window;							// we know it's a slider
	float	val = s->getCurrentValue();													// get value from slider 
	WindowManager::getSingleton().getWindow("Root")->setAlpha(val);			     		// set the value as the current alpha
	return true;																		// indicate the event was handled here
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
	// Create a DefaultWindow which we will attach all the widgets to.
	mDefaultGUIWnd = WindowManager::getSingleton().loadWindowLayout("GUI\\CubX.layout");
	// attach the window to the 'real' root
	mGUISheet->addChildWindow(mDefaultGUIWnd);					


	// Load all needed GUI Modules
	clNoteMgrGUI = new CLNoteMgrGUI();
	clCarXGUI = new CLCarXGUI();

	
	//
	// widget initialisation
	//

	// Set Widgets event
	PushButton* mQuitBtn = (PushButton*)WindowManager::getSingleton().getWindow("QuitBtn");
	mQuitBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_QuitButtonPressed);	// Add the "quit" callback for this button
	
	Slider* mAlphaSlider = (Slider*)WindowManager::getSingleton().getWindow("AlphaSlider");
	mAlphaSlider->subscribeEvent(Slider::EventValueChanged, GUI_Callback_AlphaSlider);	// Add the callback which modify the alpha for all the GUIs

	PushButton* mOptionBtn = (PushButton*)WindowManager::getSingleton().getWindow("OptionsBtn");
	mOptionBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_OptionsBtnPressed);	

	PushButton* mFeaturesBtn = (PushButton*)WindowManager::getSingleton().getWindow("FeaturesBtn");
	mFeaturesBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_FeaturesBtnPressed);

	PushButton* mScreensaverBtn = (PushButton*)WindowManager::getSingleton().getWindow("FeaturesCfg/SetScreenSaverBtn");
	mScreensaverBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_ScreensaverBtnPressed);
	
	Combobox* mSelectTime = (Combobox*)WindowManager::getSingleton().getWindow("FeaturesCfg/TimeSelect");
	ListboxTextItem* item;
	item = new ListboxTextItem("1 sec", 0);
	mSelectTime->addItem(item);
	item = new ListboxTextItem("2 sec", 1);
	mSelectTime->addItem(item);
	item = new ListboxTextItem("5 sec", 2);
	mSelectTime->addItem(item);
	item = new ListboxTextItem("10 sec", 3);
	mSelectTime->addItem(item);
	item = new ListboxTextItem("15 sec", 4);
	mSelectTime->addItem(item);
	item = new ListboxTextItem("30 sec", 5);
	mSelectTime->addItem(item);

	mSelectTime->setReadOnly(true);
	mSelectTime->subscribeEvent( Combobox::EventMouseClick, GUI_Callback_TimeSelect);

	FrameWindow* mOptionsWindow = (FrameWindow*)WindowManager::getSingleton().getWindow("SetupWindow");
	mOptionsWindow->subscribeEvent(FrameWindow::EventCloseClicked, GUI_Callback_SetupWindowClosed);	

	FrameWindow* mFeaturesWindow = (FrameWindow*)WindowManager::getSingleton().getWindow("FeaturesCfg");
	mFeaturesWindow->subscribeEvent(FrameWindow::EventCloseClicked, GUI_Callback_FeaturesWindowClosed);

	


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
	if(mActiveGUILayoutWnd == NULL)
		MessageBoxA(NULL, "No GUI loaded, nothing to disable...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);
	else
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
			EnableGUI(clNoteMgrGUI->GetRootWnd());	
			break;


	case CARX_GUIMODE:
			EnableGUI(clCarXGUI->GetRootWnd());	
			break;
	
	default:
			break;

	}
}