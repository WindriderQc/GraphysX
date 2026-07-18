#include "stdafx.h"
#include "MainForm.h"



using namespace System;
using namespace System::Windows::Forms;


[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	BallZ2015::MainForm form;
	Application::Run(%form);
}

