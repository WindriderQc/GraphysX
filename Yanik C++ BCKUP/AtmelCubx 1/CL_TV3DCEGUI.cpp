#include "cl_tv3dcegui.h"
#include "Global.h"
#include "Screensaver.h"


#pragma warning(disable: 4005) // _RPTF0' : macro redefinition

// Initialisation du singleton à NULL
CL_TV3DCEGUI *CL_TV3DCEGUI::_singleton = NULL;


CLGlobalVar* clGlobalVar = CLGlobalVar::getInstance();
CLScreensaver* clScreensaver = CLScreensaver::getInstance();

int test;


CL_TV3DCEGUI::CL_TV3DCEGUI(void)
{
	pTVIntObj = new CTVInternalObjects();
	d3Device = NULL;
	test = 0;

	InitCEGUI();
}

CL_TV3DCEGUI::~CL_TV3DCEGUI(void)
{
delete(mGUIRenderer); mGUIRenderer = NULL;
//delete(mGUISystem); mGUISystem = NULL;

delete(pTVIntObj);  pTVIntObj = NULL;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Quit" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_QuitButtonPressed(const CEGUI::EventArgs& e)
{
	test = 1;
	PostQuitMessage(0);	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Options" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_OptionsBtnPressed(const CEGUI::EventArgs& e)
{	
	using namespace CEGUI;												// Use CeGUI namespace
	CEGUI::WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

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
	CEGUI::WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

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
	CEGUI::WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

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
	CEGUI::WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

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
	CEGUI::WindowManager& winMgr = WindowManager::getSingleton();		// we will use the WindowManager to get access to the widgets

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


void CL_TV3DCEGUI::InitCEGUI(void)
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

	// Create a DefaultWindow which we will attach all the widgets to.
	Window* mBackGroundWnd = WindowManager::getSingleton().loadWindowLayout("GUI\\CubX.layout");
		//(DefaultWindow*)winMgr.createWindow("DefaultWindow", "background_wnd");		// Create a window
	mGUISheet->addChildWindow(mBackGroundWnd);												// attach this to the 'real' root

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

	mNoteWindow = (FrameWindow*)WindowManager::getSingleton().getWindow("Root/NoteWnd");



/*
	// Quit button
	PushButton* btn = (PushButton*) winMgr.createWindow("TaharezLook/Button", "QuitButton");
	mBackGroundWnd->addChildWindow(btn);														// Add the button as a child
	btn->setText("Quit");															// Set text
	btn->setPosition(UVector2(UDim(0, 10), UDim(0,10)));							// Set position
	btn->setSize(UVector2(UDim(0, 50), UDim(0,20)));											// Set size
	btn->setInheritsAlpha(false);	// It doesn't inherites alpha then it will be always visible
	btn->subscribeEvent(PushButton::EventClicked, GUI_Callback_buttonPressed);//&GUI_Callback_buttonPressed);		// Add the "quit" callback for this button
	btn->setAlwaysOnTop(true);														// We want it allways on top

	// Alpha-slider
	Slider* slider = (Slider*) winMgr.createWindow("TaharezLook/Slider", "my slider");
	mBackGroundWnd->addChildWindow(slider);													// Add it as a child
	slider->setPosition(UVector2(UDim(0.97f,0), UDim(0.01f,0)));										// Set position
	slider->setSize(UVector2(UDim(0.02f,0), UDim(0.075f,0)));											// Set size
	slider->setAlwaysOnTop(true);													// We want it allways on top
	slider->setInheritsAlpha(false);												// It doesn't inherites alpha then it will be always visible
	slider->setCurrentValue(0.7f);													// Set current value
	slider->setClickStep(0.1f);														// Set clic step
	slider->subscribeEvent(Slider::EventValueChanged, GUI_Callback_slider);		// Add the callback which modify the alpha for all the GUIs
*/
	/*
	//
	// Build a window with some text and formatting options via radio buttons
	//
	FrameWindow* textwnd = (FrameWindow*) winMgr.createWindow("TaharezLook/FrameWindow", "TextWindow");
	sheet->addChildWindow(textwnd);													// Add it as a child
	textwnd->setPosition(Point(0.2f, 0.2f));										// Set position
	textwnd->setMaximumSize(Size(0.75f, 0.75f));									// Set max size
	textwnd->setMinimumSize(Size(0.1f, 0.1f));										// Set min size
	textwnd->setSize(Size(0.5f, 0.5f));												// Set current size
	textwnd->setCloseButtonEnabled(false);											// Disable close button
	textwnd->setText("A simple Window");											// Set title

	// Static text
	st = (StaticText*) winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Static");
	textwnd->addChildWindow(st);													// Add it as a child
	st->setPosition(Point(0.1f, 0.2f));												// Set position
	st->setSize(Size(0.5f, 0.6f));													// Set size

	// Static text
	st = (StaticText*) winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Group label 1");
	textwnd->addChildWindow(st);													// Add it as a child
	st->setPosition(Point(0.65f, 0.23f));											// Set position
	st->setSize(Size(0.35f, 0.05f));												// Set size
	st->setText("Horz. Formatting");												// Set text
	st->setFrameEnabled(false);														// disable frame
	st->setBackgroundEnabled(false);												// disable background

	// Static text
	st = (StaticText*) winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Group label 2");
	textwnd->addChildWindow(st);													// Add it as a child
	st->setPosition(Point(0.65f, 0.53f));											// Set position
	st->setSize(Size(0.35f, 0.05f));												// Set size
	st->setText("Vert. Formatting");												// Set text
	st->setFrameEnabled(false);														// disable frame
	st->setBackgroundEnabled(false);												// disable background

	// Static text
	st = (StaticText*) winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Box label");
	textwnd->addChildWindow(st);													// Add it as a child
	st->setPosition(Point(0.12f, 0.13f));											// Set position
	st->setSize(Size(0.35f, 0.05f));												// Set size
	st->setText("Formatted Output");												// Set text
	st->setFrameEnabled(false);														// disable frame
	st->setBackgroundEnabled(false);												// disable background

	// Word-wrap checkbox
	Checkbox* cb = (Checkbox*) winMgr.createWindow("TaharezLook/Checkbox", "TextWindow/CB1");
	textwnd->addChildWindow(cb);													// Add it as a child
	cb->setPosition(Point(0.65f, 0.13f));											// Set position
	cb->setSize(Size(0.35f, 0.05f));												// Set size
	cb->setText("Word Wrap");														// Set text
	cb->subscribeEvent(Checkbox::EventCheckStateChanged, &GUI_Callback_format);		// Add a callback

	// Radio button (group 1)
	RadioButton* rb = (RadioButton*) winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB1");
	textwnd->addChildWindow(rb);													// Add it as a child
	rb->setPosition(Point(0.65f, 0.3f));											// Set position
	rb->setSize(Size(0.35f, 0.05f));												// Set size
	rb->setGroupID(1);																// Set group ID 
	rb->setText("Left Aligned");													// Set text
	rb->subscribeEvent(RadioButton::EventSelectStateChanged, &GUI_Callback_format);	// Add a callback

	// Radio button (group 1)
	rb = (RadioButton*) winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB2");
	textwnd->addChildWindow(rb);													// Add it as a child		
	rb->setPosition(Point(0.65f, 0.35f));											// Set position
	rb->setSize(Size(0.35f, 0.05f));												// Set size
	rb->setGroupID(1);																// Set group ID 
	rb->setText("Right Aligned");													// Set text
	rb->subscribeEvent(RadioButton::EventSelectStateChanged, &GUI_Callback_format);	// Add a callback

	// Radio button (group 1)
	rb = (RadioButton*) winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB3");
	textwnd->addChildWindow(rb);													// Add it as a child
	rb->setPosition(Point(0.65f, 0.4f));											// Set position
	rb->setSize(Size(0.35f, 0.05f));												// Set size
	rb->setGroupID(1);																// Set group ID 
	rb->setText("Centred");															// Set text
	rb->subscribeEvent(RadioButton::EventSelectStateChanged, &GUI_Callback_format);	// Add a callback

	// Radio button (group 2)
	rb = (RadioButton*) winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB4");
	textwnd->addChildWindow(rb);													// Add it as a child
	rb->setPosition(Point(0.65f, 0.6f));											// Set position
	rb->setSize(Size(0.35f, 0.05f));												// Set size
	rb->setGroupID(2);																// Set group ID 
	rb->setText("Top Aligned");														// Set text
	rb->subscribeEvent(RadioButton::EventSelectStateChanged, &GUI_Callback_format);	// Add a callback

	// Radio button (group 2)
	rb = (RadioButton*) winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB5");
	textwnd->addChildWindow(rb);													// Add it as a child
	rb->setPosition(Point(0.65f, 0.65f));											// Set position
	rb->setSize(Size(0.35f, 0.05f));												// Set size
	rb->setGroupID(2);																// Set group ID 
	rb->setText("Bottom Aligned");													// Set text
	rb->subscribeEvent(RadioButton::EventSelectStateChanged, &GUI_Callback_format);	// Add a callback

	// Radio button (group 2)
	rb = (RadioButton*) winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB6");
	textwnd->addChildWindow(rb);													// Add it as a child
	rb->setPosition(Point(0.65f, 0.7f));											// Set position
	rb->setSize(Size(0.35f, 0.05f));												// Set size
	rb->setGroupID(2);																// Set group ID 
	rb->setText("Centred");															// Set text
	rb->subscribeEvent(RadioButton::EventSelectStateChanged, &GUI_Callback_format);	// Add a callback

	// Edit box for text entry
	Editbox* eb = (Editbox*) winMgr.createWindow("TaharezLook/Editbox", "TextWindow/Editbox1");
	textwnd->addChildWindow(eb);													// Add it as a child
	eb->setPosition(Point(0.05f, 0.85f));											// Set position
	eb->setMaximumSize(Size(1.0f, 0.04f));											// Set max size
	eb->setSize(Size(0.90f, 0.08f));												// Set current size
	eb->subscribeEvent(Window::EventTextChanged, &GUI_Callback_textChanged);		// Add a callback

	//
	// Controls are set up.  Install initial settings
	//
	((Checkbox*)	winMgr.getWindow("TextWindow/CB1"))->setSelected(true);
	((RadioButton*)	winMgr.getWindow("TextWindow/RB1"))->setSelected(true);
	((RadioButton*)	winMgr.getWindow("TextWindow/RB4"))->setSelected(true);
	
	winMgr.getWindow("TextWindow/Editbox1")->setText("Come on then, edit me!");
*/
}
void CL_TV3DCEGUI::Render(void)
{
CEGUI::System::getSingleton().renderGUI();   //  CEGUI RENDER
d3Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}


void CL_TV3DCEGUI::EnableNoteBloc(void)
{
	mNoteWindow->setProperty("Visible","true");
}