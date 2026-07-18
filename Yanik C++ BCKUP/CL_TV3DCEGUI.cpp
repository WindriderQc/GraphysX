#include ".\cl_tv3dcegui.h"



int test;


CL_TV3DCEGUI::CL_TV3DCEGUI(void)
{
	pTVIntObj = new CTVInternalObjects();
	d3Device = NULL;
}

CL_TV3DCEGUI::~CL_TV3DCEGUI(void)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Quit" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_buttonPressed(const CEGUI::EventArgs& e)
{
	test =1;
	PostQuitMessage(0);	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the slider (changes alpha for the GUIs)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static	bool GUI_Callback_slider(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;																// Use CeGUI namespace
	Slider* s = (Slider*) ((const WindowEventArgs&)e).window;							// we know it's a slider
	float	val = s->getCurrentValue();													// get value from slider 
	WindowManager::getSingleton().getWindow("root_wnd")->setAlpha(val);					// set the value as the current alpha
	return true;																		// indicate the event was handled here
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback on text-format changed
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static	bool GUI_Callback_format(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;																// Use CeGUI namespace
	CEGUI::WindowManager& winMgr = WindowManager::getSingleton();						// we will use the WindowManager to get access to the widgets

	// Get pointers to all the widgets we need to access
	const RadioButton* rb1 = (const RadioButton*) winMgr.getWindow("TextWindow/RB1");	// Get radio button
	const RadioButton* rb2 = (const RadioButton*) winMgr.getWindow("TextWindow/RB2");	// Get radio button
	const RadioButton* rb3 = (const RadioButton*) winMgr.getWindow("TextWindow/RB3");	// Get radio button
	const RadioButton* rb4 = (const RadioButton*) winMgr.getWindow("TextWindow/RB4");	// Get radio button
	const RadioButton* rb5 = (const RadioButton*) winMgr.getWindow("TextWindow/RB5");	// Get radio button
	const RadioButton* rb6 = (const RadioButton*) winMgr.getWindow("TextWindow/RB6");	// Get radio button
	const Checkbox*    cb1 = (const Checkbox*)	  winMgr.getWindow("TextWindow/CB1");	// Get checkbox

	// And also the static text for which we will set the formatting options
//	StaticText*  st  = (StaticText*) winMgr.getWindow("TextWindow/Static");
/*
	// Handle vertical formatting settings
		 if(rb4->isSelected())		st->setVerticalFormatting(StaticText::TopAligned);
	else if(rb5->isSelected())		st->setVerticalFormatting(StaticText::BottomAligned);
	else if(rb6->isSelected())		st->setVerticalFormatting(StaticText::VertCentred);

	// Handle horizontal formatting settings
	bool wrap = cb1->isSelected();
		 if(rb1->isSelected())		st->setHorizontalFormatting(wrap ? StaticText::WordWrapLeftAligned	: StaticText::LeftAligned);
	else if(rb2->isSelected())		st->setHorizontalFormatting(wrap ? StaticText::WordWrapRightAligned : StaticText::RightAligned);
	else if(rb3->isSelected())		st->setHorizontalFormatting(wrap ? StaticText::WordWrapCentred		: StaticText::HorzCentred);
*/
	// event was handled
	return true;
}



void CL_TV3DCEGUI::InitCEGUI(void)
{

//extern LPDIRECT3DDEVICE9* d3Device;

	d3Device = pTVIntObj->GetDevice3D(); //  get device from TV3D

using namespace CEGUI; 

mGUIRenderer = new CEGUI::DirectX9Renderer(d3Device, 0); 
mGUISystem = new CEGUI::System(mGUIRenderer); 
CEGUI::Logger::getSingleton().setLogFilename("cegui.log", true); 
CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Insane); 

	// Load scheme and set up defaults
CEGUI::SchemeManager::getSingleton().loadScheme("GUI\\GUItutorial24.scheme"); 
mGUISystem->setDefaultMouseCursor("TaharezLook", "MouseArrow"); 
FontManager::getSingleton().createFont("Commonwealth-10.font"); 

WindowManager& winMgr = WindowManager::getSingleton();

 /*
mEditorGuiSheet = (DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root"); 
System::getSingleton().setGUISheet(mEditorGuiSheet ); 
*/
/*
FrameWindow* wnd = (FrameWindow*)winMgr.createWindow("TaharezLook/FrameWindow", "Demo Window"); 
mEditorGuiSheet->addChildWindow(wnd); 
wnd->setPosition(Point(0.25f, 0.25f)); 
wnd->setSize(Size(0.5f, 0.5f)); 
wnd->setMaximumSize(Size(1.0f, 1.0f)); 
wnd->setMinimumSize(Size(0.1f, 0.1f)); 
wnd->setText("Hello World!"); 
*/
 
	// We use a StaticImage as the root (the image is not loaded, then it's free)
	background = (Window*) winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
	background->setPosition(UVector2(UDim(0, 0), UDim(0,0)));											// set position
	background->setSize(UVector2(UDim(1, 0), UDim(1,0)));												// set size
	//background->setFrameEnabled(false);												// disable frame
	//background->setBackgroundEnabled(false);										// disable standard background
	System::getSingleton().setGUISheet(background);									// install this as the root GUI sheet

	// Create a DefaultWindow which we will attach all the widgets to.
	CEGUI::Window* sheet = winMgr.createWindow("DefaultWindow", "root_wnd");		// Create a window
	background->addChildWindow(sheet);												// attach this to the 'real' root

	//
	// widget initialisation
	//

	// Quit button
	PushButton* btn = (PushButton*) winMgr.createWindow("TaharezLook/Button", "QuitButton");
	sheet->addChildWindow(btn);														// Add the button as a child
	btn->setText("Quit");															// Set text
	btn->setPosition(UVector2(UDim(0, 10), UDim(0,10)));							// Set position
	btn->setSize(UVector2(UDim(0, 50), UDim(0,20)));											// Set size
	btn->setInheritsAlpha(false);													// It doesn't inherites alpha then it will be always visible
	btn->subscribeEvent(PushButton::EventClicked, GUI_Callback_buttonPressed);		// Add the "quit" callback for this button
	btn->setAlwaysOnTop(true);														// We want it allways on top

	// Alpha-slider
	Slider* slider = (Slider*) winMgr.createWindow("TaharezLook/Slider", "my slider");
	sheet->addChildWindow(slider);													// Add it as a child
	slider->setPosition(UVector2(UDim(0.97f,0), UDim(0.01f,0)));										// Set position
	slider->setSize(UVector2(UDim(0.02f,0), UDim(0.075f,0)));											// Set size
	slider->setAlwaysOnTop(true);													// We want it allways on top
	slider->setInheritsAlpha(false);												// It doesn't inherites alpha then it will be always visible
	slider->setCurrentValue(0.7f);													// Set current value
	slider->setClickStep(0.1f);														// Set clic step
	slider->subscribeEvent(Slider::EventValueChanged, &GUI_Callback_slider);		// Add the callback which modify the alpha for all the GUIs

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
}