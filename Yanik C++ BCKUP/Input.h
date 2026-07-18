#pragma once

#include "StdAfx.h"


class CInput
{
public:
	CInput(void);
	~CInput(void);
	
	bool InitTV3DInput(void); //inits TV3D InputEngine
	void GetInputState(void);
	void GetUserKBInput(float pTimeElapsed);


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

	cTV_3DVECTOR CamPos; 
	cTV_3DVECTOR CamLookAt;

	CTVMesh* pMeshClicked;
	CTVMesh* pOldMeshClicked;

	bool bCAM_CTRL_ENABLED;

private:
		cTV_KEYDATA KEY_BUFFER[256];
		int BUFFER_COUNT;
		byte KEY_PRESSED[256];

		float sngAngleX; 
		float sngAngleY;
		float sngWalk;
		float sngStrafe;


		CTVInputEngine *pTV3DInput;
		CTVScene *pTV3DScene;

   
};
