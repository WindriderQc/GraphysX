#pragma once
#include "StdAfx.h"
#include "GUI.h"





class CLSetupGUI : public CLGUI
{
public:
	CLSetupGUI(string PathToLayout) : CLGUI(PathToLayout)
	{};
	~CLSetupGUI(void)
	{};

	void Init(void);  
};


