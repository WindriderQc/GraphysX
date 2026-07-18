#pragma once
#include "GUI.h"

public ref class GUIIntro : public GUI
{
public:
	GUIIntro(Engine3DWrapper^ Tools3D, const std::string& scheme, const std::string& font, const std::string& MouseCursor, std::string PathToLayout) : GUI(Tools3D)
	{
		Init(scheme, font, MouseCursor, PathToLayout);
		LoadElements();
	};
	void LoadElements()
	{
		CEGUI::PushButton* TestButton = static_cast<CEGUI::PushButton*>(Tools3D->m_CEGUI->CreateWidget("AlfiskoSkin/Button", cTV_4DVECTOR(0.5f, 0.5f, 0.1f, 0.05f), cTV_4DVECTOR(0, 0, 0, 0), "testbutton"));
		TestButton->setText("Hello World");
	};



};

