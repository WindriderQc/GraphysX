#pragma once
#include "StdAfx.h"



class CLGUIVarController
{
public:
	CLGUIVarController(void);
	~CLGUIVarController(void);

	int PosX, PosY;
	float fMin, fMax, fValue;
	char* Name;
	float* ptrCtrlledVar;


};
