#include ".\input.h"


CLInput *CLInput::_singleton = NULL;

CLInput::CLInput(void)
{
	pTV3DScene = new CTVScene(); 
	pTV3DInput = new CTVInputEngine();
	pTV3DInput->Initialize(true, true);

	for(int i=0; i<256;i++)
	{
	KEY_PRESSED[i] = NULL;
	}
	BUFFER_COUNT = 0;

	stKeyboard.KeyPressed1 = NULL;
	stKeyboard.KeyPressed2 = NULL;
	stKeyboard.KeyUp = NULL;
	stKeyboard.KeyDown = NULL;
	stMouse.B1 = NULL;
	stMouse.B2 = NULL;
	stMouse.ButtonUp = NULL;
	stMouse.ButtonDown = NULL;
	stMouse.DeltaPosX = NULL;
	stMouse.DeltaPosY = NULL;

	stOldKeyboard.KeyPressed1 = NULL;
	stOldKeyboard.KeyPressed2 = NULL;
	stOldKeyboard.KeyUp = NULL;
	stOldKeyboard.KeyDown = NULL;
	stOldMouse.B1 = NULL;
	stOldMouse.B2 = NULL;
	stOldMouse.ButtonUp = NULL;
	stOldMouse.ButtonDown = NULL;


	iPushBallX = 0;
	iPushBallZ = 0;
	sngWalk = 0;
	sngStrafe = 0;
	sngRaise = 0;
	bQuit = false;


	pOldMeshClicked = NULL;
	pMeshClicked = NULL;
	
    keyboardLayout = GetKeyboardLayout((UINT)0);



	pTVGamePads = new CTVGameControllers();
	bool test = pTVGamePads->Item(0)->Initialize();
	test = 0;
	test = pTVGamePads->Item(0)->IsConnected();  
	string sTest = pTVGamePads->Item(0)->GetProductName();
	stGamePad.b0 = NULL;
	stGamePad.b1 = NULL;
	stGamePad.LeftX = NULL;
	stGamePad.LeftY = NULL;
	stGamePad.RightX = NULL;
	stGamePad.RightY = NULL;
}
CLInput::~CLInput(void)
{
	delete(pTV3DInput);  pTV3DInput = NULL;
}

void CLInput::GetUserInput(void)
{
////////////////////////////////////////
	//  Mouse Section  //
////////////////////////////////////////

//	clCEGUI = CLTV3DCEGUI::getInstance();
	stOldMouse.PosX = stMouse.PosX;
	stOldMouse.PosY = stMouse.PosY;
	stOldMouse.B1 = stMouse.B1;
	stOldMouse.B2 = stMouse.B2;

	stMouse.ButtonUp = NULL;
	stMouse.ButtonDown = NULL;


	pTV3DInput->GetAbsMouseState(&stMouse.PosX, &stMouse.PosY, &stMouse.B1,
										&stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel); 
	pTV3DInput->GetMouseState(&stMouse.DeltaPosX, &stMouse.DeltaPosY, &stMouse.B1,
										&stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel); //A implementer : ctrl de la molette

	
	//clCEGUI->GetGUISystem()->injectMousePosition(float(stMouse.PosX), float(stMouse.PosY));
	//bool injectMouseLeaves(void);  // Sert a signaler que la sourie a sortie de notre window
	

	// Button UP
	if((stOldMouse.B1 != NULL) && (stMouse.B1 == NULL))
	{
		stMouse.ButtonUp = stOldMouse.B1;
//		clCEGUI->GetGUISystem()->injectMouseButtonUp(CEGUI::LeftButton);
	}
	else if((stOldMouse.B2 != NULL) && (stMouse.B2 == NULL))
	{
		stMouse.ButtonUp = stOldMouse.B2;
	//	clCEGUI->GetGUISystem()->injectMouseButtonUp(CEGUI::RightButton);
	}
	

	// Button Down
	if((stOldMouse.B1 == NULL) && (stMouse.B1 != NULL))
	{
		stMouse.ButtonDown = stMouse.B1;
//		clCEGUI->GetGUISystem()->injectMouseButtonDown(CEGUI::LeftButton);
	}
	else if((stOldMouse.B2 == NULL) && (stMouse.B2 != NULL))
	{
		stMouse.ButtonDown = stMouse.B2;
//		clCEGUI->GetGUISystem()->injectMouseButtonDown(CEGUI::RightButton);
	}
	


////////////////////////////////////////
	//  Keyboard Section  //
////////////////////////////////////////


 cCONST_TV_KEY Pressedkey;
 cCONST_TV_KEY Releasedkey;

         /////////////////////////////////////////////////////////////////////
         // non-repeating commands
         //    (commands that should not continuously fire)
         /////////////////////////////////////////////////////////////////////

         pTV3DInput->GetKeyBuffer(KEY_BUFFER, &BUFFER_COUNT);
		
         for (int i = 0; i < BUFFER_COUNT; i ++)
         {
			 if (KEY_BUFFER[i].Pressed != 0)
              {
                Pressedkey = KEY_BUFFER[i].Key;
//				clCEGUI->GetGUISystem()->injectKeyDown(Pressedkey);
				// Transfer to ASCII
                //char c = ScancodeToASCII(Pressedkey);  // Tester pour voir si ScanToASCII retourne 0 si c'Est pas un Char
                //if (c != (char)0)
//                   clCEGUI->GetGUISystem()->injectChar(c); 
				
				
				/*
				switch(Pressedkey)
				{
				case cTV_KEY_RETURN:
						clCEGUI->GetGUISystem()->injectKeyDown(Pressedkey);
						break;
				case cTV_KEY_BACKSPACE:
						clCEGUI->GetGUISystem()->injectKeyDown(Pressedkey);
						break;
				case cTV_KEY_RIGHTSHIFT:
						clCEGUI->GetGUISystem()->injectKeyDown(Pressedkey);
						break;
				case cTV_KEY_LEFTSHIFT:
						clCEGUI->GetGUISystem()->injectKeyDown(Pressedkey);
						break;

				default:
						// Transfer to ASCII
                       char c = ScancodeToASCII(Pressedkey);  // Tester pour voir si ScanToASCII retourne 0 si c'Est pas un Char
                       if (c != (char)0)
                          clCEGUI->GetGUISystem()->injectChar(c); 
					   break;
				
				}
				*/

                //""""""""""""""""""""""""""""""""""""""""""""""""
                // pause / end game
				if (Pressedkey ==  cTV_KEY_ESCAPE)   //  Ne marche pas, le message n'est pas traité!!!!
				 {
				  bQuit = true;
				 }
               }
			 
			 // Set released Key
			 if (KEY_BUFFER[i].Released != 0)
             {
			 Releasedkey = KEY_BUFFER[i].Key;
//			 clCEGUI->GetGUISystem()->injectKeyUp(Releasedkey);
			 }
         }

         //===================================================================

         /////////////////////////////////////////////////////////////////////
         // repeating commands
         //    (commands that should continuously fire)
         /////////////////////////////////////////////////////////////////////

         pTV3DInput->GetKeyPressedArray(KEY_PRESSED);

	
         // turn left / turn right
         if (KEY_PRESSED[cTV_KEY_LEFTARROW] != 0)
         {
           sngStrafe = 1;
         }
         else if (KEY_PRESSED[cTV_KEY_RIGHTARROW] != 0)
         {
            sngStrafe = -1;
         }
         // move forward / move backward
         if (KEY_PRESSED[cTV_KEY_UPARROW] != 0)
         {
            sngWalk = 1;			 
         }
         else if (KEY_PRESSED[cTV_KEY_DOWNARROW] != 0)
         {
            sngWalk = -1;
         }

		if (KEY_PRESSED[cTV_KEY_Z] !=0)
          {
           sngRaise = -1;
          }
		else if (KEY_PRESSED[cTV_KEY_X] !=0)
          {
		   sngRaise = 1;
          }


		 //""""""""""""""""""""""""""""""""""""""""
		 //  Ball Control
		 if (KEY_PRESSED[cTV_KEY_A])
		 {
			iPushBallX = -1; 
		 }
		 else if (KEY_PRESSED[cTV_KEY_S])
		 {
			iPushBallZ = -1;
		 }
		 else if (KEY_PRESSED[cTV_KEY_D])
		 {
			iPushBallX = 1;
		 }
		 else if (KEY_PRESSED[cTV_KEY_W])
		 {
			iPushBallZ = 1;
		 }

}
bool CLInput::IsMouseButtonPressed(int iButton)
{
	return(pTV3DInput->IsMouseButtonPressed(iButton));
}

char CLInput::ScancodeToASCII(cCONST_TV_KEY scancode) 
{
 char ResultChar = NULL;
 UINT virtualKey = MapVirtualKeyEx((UINT)scancode, (UINT)1, keyboardLayout);
 ToAsciiEx(virtualKey, (UINT)scancode, KEY_PRESSED, (LPWORD)&ResultChar, (UINT)0, keyboardLayout);
 return (ResultChar);
}
void CLInput::ResetFlags(void)
{	
iPushBallX = 0; 
iPushBallZ = 0;
iPushBallX = 0;
iPushBallZ = 0;

sngWalk = 0; 
sngStrafe = 0; 
sngRaise = 0;  
}



void CLInput::CheckGamePadInput(void)
{
    // Game pad 1 (PSX/PC DUal Stick 12 button paddles)
	pTVGamePads->Item(0)->Poll();
    joyState = pTVGamePads->Item(0)->GetControllerState();

    if (pTVGamePads->Item(0)->GetButtonState(1)) // this is 0 - 11 for button index numbers
		stGamePad.b1 = 1;
	else
		stGamePad.b1 = 0;


	 if (pTVGamePads->Item(0)->GetButtonState(0)) // this is 0 - 11 for button index numbers
		stGamePad.b0 = 1;
	else
		stGamePad.b0 = 0;


	// Left Analog Stick  // Y - > Up = 0, Down = 10000
	//                    // X - > Left=0, Right = 10000  
	
	
	// test avec 4500/5500 pour éviter de la fluctuation selon la calibration de la manette ou mouvement trop petit

	if((4000 < joyState.X) && (joyState.X  < 6000))
		stGamePad.LeftX = NULL;
	else
		stGamePad.LeftX = (joyState.X -5000) / 50 ; // -5000 et divise par 100 pour avoir de -100 à 100 dans stGamePad,
	
	if((4000 < joyState.Y) && (joyState.Y < 6000))
		stGamePad.LeftY = NULL;
	else
		stGamePad.LeftY = (joyState.Y -5000) / 50 * -1;// * -1 pour inversé le sens


	//Right Analog Stick  // RotationZ - > Left = 0, Right = 1000
	//                    // Z - > Up, Down
	if((4000 < joyState.rotationz) && (joyState.rotationz  < 6000))
		stGamePad.RightY = NULL;
	else
		stGamePad.RightY = (joyState.rotationz -5000) / 50* -1; // * -1 pour inversé le sens
	if((4000 < joyState.Z) && (joyState.Z < 6000))
		stGamePad.RightX = NULL;
	else
		stGamePad.RightX = (joyState.Z -5000) / 50;

    //if pTVGamePad->GetControllerStateAdvanced.x < 5000
	//if pTVGamePad->GetControllerStateAdvanced.x > 5000
    //if pTVGamePad->GetControllerStateAdvanced.y > 5000
    //if pTVGamePad->GetControllerStateAdvanced.y > 5000
	//if pTVGamePad->GetControllerStateAdvanced.rotationz < 5000
	//if pTVGamePad->GetControllerStateAdvanced.rotationz > 5000
	//if pTVGamePad->GetControllerStateAdvanced.z > 5000
	//if pTVGamePad->GetControllerStateAdvanced.z > 50000   

	/*	' Directional Pad
        
        Select Case pTVGamePad->GetControllerStateAdvanced.PointOfView0
            Case -1
                No Press
            Case 0
                up
            Case 9000
                right
            Case 18000
               	down
            Case 27000
                left
            Case 4500
                up right
            Case 13500
                right down
            Case 22500
                down left
            Case 31500
                left up
        End Select
 */
}
 