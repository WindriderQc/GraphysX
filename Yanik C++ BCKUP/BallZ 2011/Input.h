#pragma once
#include "StdAfx.h"
//#include "TV3DCegui.h"


class CLInput
{
private:
	CLInput(void);
	~CLInput(void);

public:
	void GetUserInput(void);
	void ResetFlags(void);
	bool IsMouseButtonPressed(int iButton);
	char ScancodeToASCII(cCONST_TV_KEY scancode);

	void CheckGamePadInput(void);

	struct STKeyboard
	{
	int KeyPressed1;
	int KeyPressed2;
	int KeyUp;
	int KeyDown;
	}stKeyboard, stOldKeyboard;
	
	struct STMouse
	{
		int PosX;
		int DeltaPosX;
		int PosY;
		int DeltaPosY;
		bool B1;
		bool B2;
		bool B3;
		bool B4;
		int ButtonUp;
		int ButtonDown;
		int Wheel;		 //  sera eventuellement pour le controle de la molette
	}stMouse, stOldMouse;

	struct STGamePad
	{
		float LeftX;
		float LeftY;
		float RightX;
		float RightY;
		bool b0;
		bool b1;
		bool b2;
		bool b3;	 
	}stGamePad;

	
	CTVMesh* pMeshClicked;
	CTVMesh* pOldMeshClicked;

	int iPushBallX, iPushBallZ;
	float sngWalk;
	float sngStrafe;
	float sngRaise;
	bool bQuit;

	// Fonctions de création et destruction du singleton
    static CLInput *getInstance ()
	  {
	  if (NULL == _singleton)
      {
        _singleton =  new CLInput;
      }
	  return _singleton;
	 }

    static void kill ()
	{
	  if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
	 }

private:
		static CLInput *_singleton;
		cTV_KEYDATA KEY_BUFFER[256];
		int BUFFER_COUNT;
		byte KEY_PRESSED[256];
		HKL keyboardLayout;
		CTVInputEngine *pTV3DInput;
		CTVScene *pTV3DScene;
		//CLTV3DCEGUI* clCEGUI; 
		cTV_JOYSTATE joyState;  //  Pourrait etre mis public et remplacer la structure STGAMEPAD
		CTVGameControllers* pTVGamePads;
};
