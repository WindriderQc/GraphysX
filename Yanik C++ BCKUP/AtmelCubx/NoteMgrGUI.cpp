#include "NoteMgrGUI.h"

CLNoteMgrGUI::CLNoteMgrGUI(void)
{
	using namespace CEGUI;
	DefaultWindow* mGUISheet = (DefaultWindow*)WindowManager::getSingleton().getWindow("gui_sheet");  // Get the CEGUI root window
	
	// Load the layout Window which we will attach all the widgets to.
	mLayoutWnd = WindowManager::getSingleton().loadWindowLayout("GUI\\NoteMgrGUI.layout");
	// attach the LayoutWindow to the root
	mGUISheet->addChildWindow(mLayoutWnd);	
	mLayoutWnd->setProperty("Visible","False");
	
	Init();

	
}

CLNoteMgrGUI::~CLNoteMgrGUI(void)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Callback for the "Quit" button
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GUI_Callback_QuitButtonPressed(const CEGUI::EventArgs& e)
{
	
	return true;
}


void CLNoteMgrGUI::Init(void)
{
	using namespace CEGUI;
	// Set Widgets event
	PushButton* mQuitBtn = (PushButton*)WindowManager::getSingleton().getWindow("NoteMgrGUIRoot/QuitBtn");
	mQuitBtn->subscribeEvent(PushButton::EventClicked, GUI_Callback_QuitButtonPressed);	// Add the "quit" callback for this button

	FrameWindow* mNoteWindow = (FrameWindow*)WindowManager::getSingleton().getWindow("NoteMgrGUIRoot/NoteWnd");


	// Load the THEButton Imageset that has the pictures for our button.
//	ImagesetManager::getSingleton().createImageset( "GUI\\BtnNext.imageset" ); 

}

void CLNoteMgrGUI::Enable(void)
{
mLayoutWnd->setProperty("Visible","True");
}
void CLNoteMgrGUI::Disable(void)
{
mLayoutWnd->setProperty("Visible","False");
}