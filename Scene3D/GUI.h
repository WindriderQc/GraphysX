#pragma once
#include "Wrapper3D.h"

public ref class GUI
{
public:
	GUI(Engine3DWrapper^ tools3d);
	virtual ~GUI(void) {};

	void Init(const std::string& scheme, const std::string& font, const std::string& MouseCursor, std::string PathToLayout);  // Set value to "" for default INIT
	//void LoadElements(){};
	void ShowGUI(bool show);

	CEGUI::Window* GetRootWnd(void) { return(mLayoutWnd); }
	

protected:
	Engine3DWrapper^ Tools3D;
	CEGUI::Window* mLayoutWnd;
	CEGUI::Window* mRootWindow;
};


