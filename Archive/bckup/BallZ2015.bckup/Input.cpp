#include "StdAfx.h"
#include "input.h"


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

	stKeyboard.KeyUp = NULL;
	stKeyboard.KeyDown = NULL;
	stMouse.B1 = NULL;
	stMouse.B2 = NULL;
	stMouse.ButtonUp = NULL;
	stMouse.ButtonDown = NULL;
	stMouse.DeltaPosX = NULL;
	stMouse.DeltaPosY = NULL;

	stOldMouse.B1 = NULL;
	stOldMouse.B2 = NULL;
	stOldMouse.ButtonUp = NULL;
	stOldMouse.ButtonDown = NULL;

	bGamePadConnected = false;

	sngWalk = 0;
	sngStrafe = 0;
	sngRaise = 0;
	bQuit = false;
	
   keyboardLayout = GetKeyboardLayout((UINT)0);

   
	pTVGamePads = new CTVGameControllers();
	if (pTVGamePads->Count() != NULL) 
			bGamePadConnected = true;
		
	if(bGamePadConnected) 
	{
		bool test = pTVGamePads->Item(0)->Initialize();
		test = 0;
		test = pTVGamePads->Item(0)->IsConnected();  
		string sTest = pTVGamePads->Item(0)->GetProductName();
	}else{
		int i = 0;
		MSGBOX("GamePad not found","Input Init");
		}
	stGamePad.b0 = NULL;
	stGamePad.b1 = NULL;
	stGamePad.b2 = NULL;
	stGamePad.b3 = NULL;
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

	stOldMouse.PosX = stMouse.PosX;
	stOldMouse.PosY = stMouse.PosY;
	stOldMouse.B1 = stMouse.B1;
	stOldMouse.B2 = stMouse.B2;

	stMouse.ButtonUp = NULL;
	stMouse.ButtonDown = NULL;
	sngWalk = 0;
	sngStrafe = 0;
	sngRaise = 0;


	pTV3DInput->GetAbsMouseState(&stMouse.PosX, &stMouse.PosY, &stMouse.B1,
										&stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel); 
	pTV3DInput->GetMouseState(&stMouse.DeltaPosX, &stMouse.DeltaPosY, &stMouse.B1,
										&stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel); //A implementer : ctrl de la molette

	// Button UP
	if((stOldMouse.B1 != NULL) && (stMouse.B1 == NULL))
	{
		stMouse.ButtonUp = stOldMouse.B1;
	}
	else if((stOldMouse.B2 != NULL) && (stMouse.B2 == NULL))
	{
		stMouse.ButtonUp = stOldMouse.B2;
	}
	
	// Button Down
	if((stOldMouse.B1 == NULL) && (stMouse.B1 != NULL))
	{
		stMouse.ButtonDown = stMouse.B1;
	}
	else if((stOldMouse.B2 == NULL) && (stMouse.B2 != NULL))
	{
		stMouse.ButtonDown = stMouse.B2;
	}
	

////////////////////////////////////////
	//  Keyboard Section  //
////////////////////////////////////////
         /////////////////////////////////////////////////////////////////////
         // non-repeating commands
         //    (commands that should not continuously fire)
         /////////////////////////////////////////////////////////////////////

		stKeyboard.Pressedkey = cTV_FORCE_DWORD;  // Reset les flags.
		stKeyboard.Releasedkey = cTV_FORCE_DWORD;
         pTV3DInput->GetKeyBuffer(KEY_BUFFER, &BUFFER_COUNT);
		
         for (int i = 0; i < BUFFER_COUNT; i ++)
         {
			 // Set Pressed Key
			 if (KEY_BUFFER[i].Pressed != 0)
              {
				 stKeyboard.Pressedkey = KEY_BUFFER[i].Key;
              }
			 // Set released Key
			 if (KEY_BUFFER[i].Released != 0)
             {
				 stKeyboard.Releasedkey = KEY_BUFFER[i].Key;
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
            sngStrafe = 1;
         else if (KEY_PRESSED[cTV_KEY_RIGHTARROW] != 0)
            sngStrafe = -1;
         // move forward / move backward
         if (KEY_PRESSED[cTV_KEY_UPARROW] != 0)
            sngWalk = 1;			 
         else if (KEY_PRESSED[cTV_KEY_DOWNARROW] != 0)
            sngWalk = -1;
		 // move Unp / move Down
		if (KEY_PRESSED[cTV_KEY_Z] !=0)
	        sngRaise = -1;
		else if (KEY_PRESSED[cTV_KEY_X] !=0)
			sngRaise = 1;
	        
////////////////////////////////////////
//  GamePad Section  //
////////////////////////////////////////

		 CheckGamePadInput();
}
bool CLInput::IsMouseButtonPressed(int iButton)
{
	return(pTV3DInput->IsMouseButtonPressed(iButton));
}
void CLInput::CheckGamePadInput(void)
{
/*
Side not for the left and right analog sticks: All values are 5000 greater or less than. This is the analogs range of motion 
from 0 to 5000 and 0 to -5000. You can make it so if you have like 2500 if you want different speeds depending on how far you 
have the stick pushed but that’s up to you to decide.
Side not for the D-Pad or Directional Pad all values are 9000 and to do the next direction it would be 9000 + 9000. For any diagonal 
it would be 1/2. As you can see above in the code.
D-Pad Values up = 0 down = 18000 left = 27000 right = 9000 up right = 4500 right down 13500 down left 22500 up left = 31500 no press = -1
*/
	if(bGamePadConnected)
	{
			// Game pad 1 (PSX/PC DUal Stick 12 button paddles)
			pTVGamePads->Item(0)->Poll();
			joyState = pTVGamePads->Item(0)->GetControllerState();

			if (pTVGamePads->Item(0)->GetButtonState(0)) // this is 0 - 11 for button index numbers
				stGamePad.b0 = 1;
			else
				stGamePad.b0 = 0;
			if (pTVGamePads->Item(0)->GetButtonState(1)) // this is 0 - 11 for button index numbers
				stGamePad.b1 = 1;
			else
				stGamePad.b1 = 0;

			if (pTVGamePads->Item(0)->GetButtonState(2)) // this is 0 - 11 for button index numbers
				stGamePad.b2 = 1;
			else
				stGamePad.b2 = 0;
	
			if (pTVGamePads->Item(0)->GetButtonState(3)) // this is 0 - 11 for button index numbers
				stGamePad.b3 = 1;
			else
				stGamePad.b3 = 0;

			// test avec 4500/5500 pour éviter de la fluctuation selon la calibration de la manette ou mouvement trop petit
			// Left Analog Stick  // Y - > Up = 0, Down = 10000
			//                    // X - > Left=0, Right = 10000  
			if((4800 < joyState.X) && (joyState.X  < 5200))
				stGamePad.LeftX = NULL;
			else
				stGamePad.LeftX = (joyState.X -5000) / 5 ; // -5000 et divise par 50 pour avoir de -100 à 100 dans stGamePad,
	
			if((4800 < joyState.Y) && (joyState.Y < 5200))
				stGamePad.LeftY = NULL;
			else
				stGamePad.LeftY = (joyState.Y -5000) / 5 * -1; // * -1 pour inversé le sens

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
		}
}
 