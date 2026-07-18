#include ".\input.h"

CInput::CInput() 
{
	bCAM_CTRL_ENABLED = 0;

	 
	pTV3DInput = new CTVInputEngine();
	pTV3DScene = new CTVScene();

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
	stMouse.B3 = NULL;
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
	stOldMouse.B3 = NULL;
	stOldMouse.ButtonUp = NULL;
	stOldMouse.ButtonDown = NULL;

	sngAngleX = 0; 
	sngAngleY = 0;
	sngWalk = 0;
	sngStrafe = 0;


	CamPos.x = 500;
	CamPos.y = 50;
	CamPos.z = -500;		
	CamLookAt.x = 0;
	CamLookAt.y = 0;
	CamLookAt.z = 0;

	pOldMeshClicked = NULL;
	pMeshClicked = NULL;

	InitTV3DInput();  // Init TV3D InputEngine   // Rajouter un check du résultat...
	

	

}

CInput::~CInput(void)
{
pTV3DInput = NULL;
}

bool CInput::InitTV3DInput(void)
{

if(pTV3DInput == NULL)
 {
 return false;
 }
pTV3DInput->Initialize(true, true);
return true;
} 

void CInput::GetInputState(void)
{
	stMouse.ButtonUp = NULL;
	stMouse.ButtonDown = NULL;
/*	stKeyboard.KeyDown = NULL;

//	pTV3DInput->GetKeyPressed(&stKeyboard.KeyPressed1, &stKeyboard.KeyPressed2);
	// Keup UP
	if((stOldKeyboard.KeyPressed1 != NULL) && (stKeyboard.KeyPressed1 == NULL))
		stKeyboard.KeyUp = stOldKeyboard.KeyPressed1;
	else if((stOldKeyboard.KeyPressed2 != NULL) && (stKeyboard.KeyPressed2 == NULL))
		stKeyboard.KeyUp = stOldKeyboard.KeyPressed2;
	// Key DOwn
	if((stOldKeyboard.KeyPressed1 == NULL) && (stKeyboard.KeyPressed1 != NULL))
		stKeyboard.KeyDown = stKeyboard.KeyPressed1;
	else if((stOldKeyboard.KeyPressed2 == NULL) && (stKeyboard.KeyPressed2 != NULL))
		stKeyboard.KeyDown = stKeyboard.KeyPressed2;
	*/


	pTV3DInput->GetAbsMouseState(&stMouse.PosX, &stMouse.PosY, &stMouse.B1,
										&stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel); 
	pTV3DInput->GetMouseState(&stMouse.DeltaPosX, &stMouse.DeltaPosY, &stMouse.B1,
										&stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel); // changer bidon pour de koi de vrai pour ctrl de la molette
			
	// Button UP
	if((stOldMouse.B1 != NULL) && (stMouse.B1 == NULL))
		stMouse.ButtonUp = stOldMouse.B1;
	else if((stOldMouse.B2 != NULL) && (stMouse.B2 == NULL))
		stMouse.ButtonUp = stOldMouse.B2;
	else if((stOldMouse.B3 != NULL) && (stMouse.B3 == NULL))
		stMouse.ButtonUp = stOldMouse.B3;
	// Button Down
	else if((stOldMouse.B1 == NULL) && (stMouse.B1 != NULL))
		stMouse.ButtonDown = stMouse.B1;
	else if((stOldMouse.B2 == NULL) && (stMouse.B2 != NULL))
		stMouse.ButtonDown = stMouse.B2;
	else if((stOldMouse.B3 == NULL) && (stMouse.B3 != NULL))
		stMouse.ButtonDown = stMouse.B3;
	


	// NO Button 
	if((stOldMouse.B1==NULL)&&(stMouse.B1==NULL)&&(stOldMouse.B2==NULL)&&(stMouse.B2==NULL)&&(stOldMouse.B3==NULL)&&(stMouse.B3==NULL))
		stMouse.ButtonDown = NULL;


	stOldKeyboard.KeyPressed1 = stKeyboard.KeyPressed1;
	stOldKeyboard.KeyPressed2 = stKeyboard.KeyPressed2;
	stOldMouse.PosX = stMouse.PosX;
	stOldMouse.PosY = stMouse.PosY;
	stOldMouse.B1 = stMouse.B1;
	stOldMouse.B2 = stMouse.B2;
	stOldMouse.B3 = stMouse.B3;

}


 
void CInput::GetUserKBInput(float pTimeElapsed)
{
 cCONST_TV_KEY key;

         /////////////////////////////////////////////////////////////////////
         // non-repeating commands
         //    (commands that should not continuously fire)
         /////////////////////////////////////////////////////////////////////

         pTV3DInput->GetKeyBuffer(KEY_BUFFER, &BUFFER_COUNT);

         for (int i = 0; i < BUFFER_COUNT; i ++)
         {
			 if (KEY_BUFFER[i].Pressed != 0)
            {
               key = KEY_BUFFER[i].Key;

               //""""""""""""""""""""""""""""""""""""""""""""""""
               // pause / end game

               if (key == cTV_KEY_F12)
               {
                  // ### pause game here
               }
               else if (key ==  cTV_KEY_ESCAPE)   //  Ne marche pas, le message n'est pas traité!!!!
               {
                 // If the ESC key is pressed stop the application.
				 PostQuitMessage(0);  //SendMessage(WindowHandle, WM_DESTROY, 0, 0); }
               }

               //""""""""""""""""""""""""""""""""""""""""""""""""
               // jump and attack

               // jump
               if (key == cTV_KEY_J)
               {
                  // ### jump here
               }

               // attack / fire main weapon / fire second weapon
               if (key == cTV_KEY_SPACE)
               {
                  // ### attack here
               }
               else if (key == cTV_KEY_A)
               {
                  // ### fire main weapon here
               }
               else if (key == cTV_KEY_B)
               {
                  // ### fire second weapon here
               }

               //""""""""""""""""""""""""""""""""""""""""""""""""
               // interact / show map / show options

               if (key == cTV_KEY_I)
               {
                  // ### interact here
               }
               else if (key == cTV_KEY_M)
               {
                  // ### show map here
               }
               else if (key == cTV_KEY_O)
               {
                  // ### show character options here
               }

               //""""""""""""""""""""""""""""""""""""""""""""""""
               // cheat mode

               if (key == cTV_KEY_C)
               {
                  // ### enable cheat mode here
               }
            }
         }

         //===================================================================

         /////////////////////////////////////////////////////////////////////
         // repeating commands
         //    (commands that should continuously fire)
         /////////////////////////////////////////////////////////////////////

         pTV3DInput->GetKeyPressedArray(KEY_PRESSED);

	if(bCAM_CTRL_ENABLED)
	{
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

	//Okay, now for the smothing of the movement... We checked
    //above if we were pressing a key. If so, then we updated the
    //movement variable to 1 (positive or negative). Here, we
    //lower this value until it get to 0. This method give us a
    //smoother camera movement. We start by updating the forward
    //and backward (walk) movement
		if(sngWalk > 0)
		{
        sngWalk = sngWalk - 0.05f;
        if(sngWalk < 0) sngWalk = 0;
		}
		if(sngWalk < 0)
		{
        sngWalk = sngWalk + 0.05f;
        if(sngWalk > 0) sngWalk = 0;
		}
	 //Now, we update the left and right (strafe) movement.
		if(sngStrafe > 0)
		{
        sngStrafe = sngStrafe - 0.05f;
        if(sngStrafe < 0) sngStrafe = 0;
		}
		if(sngStrafe < 0)
		{
        sngStrafe = sngStrafe + 0.05f;
        if(sngStrafe > 0) sngStrafe = 0;
		}
	
	
	//Update the vectors using the angles and positions.
	CamPos.x = CamPos.x + (cosf(sngAngleY) * sngWalk * pTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * pTimeElapsed);
	CamPos.z = CamPos.z + (sinf(sngAngleY) * sngWalk * pTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * pTimeElapsed);
	pTV3DScene->SetCamera(CamPos.x,CamPos.y,CamPos.z, CamLookAt.x, CamLookAt.y,CamLookAt.z);
	}
}