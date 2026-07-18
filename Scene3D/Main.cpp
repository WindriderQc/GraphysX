#include "stdafx.h"
#include "Wrapper3D.h"

//#include <GraphysX\GraphysXdef.h>
#pragma comment(lib, "..\\Deps\\Lib\\GraphysX")
#pragma comment(lib, "..\\Deps\\Lib\\TV3D65")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2Main")
#pragma comment(lib, "..\\Deps\\Lib\\CEGUIBase-0_d")
#pragma comment(lib, "..\\Deps\\Lib\\CEGUIDirect3D9Renderer-0_d")
//Newton Lib
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\newton\\Debug\\newton_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\core\\Debug\\Core_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\dJointLibrary\\Debug\\dJointLibrary_d")
#pragma comment(lib,"..\\Deps\\Lib\\dMath_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\physics\\Debug\\physics_d")
#pragma comment(lib,"..\\Deps\\Lib\\pthread_d")
#pragma comment(lib,"..\\Deps\\Lib\\dContainers_d")

using namespace System;

void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Tools::DevConsoleCreate();
	//Tools::console con(40, 40);

	GraphysXWrap::GraphAppWrapper App;
	App.run();


//	system("PAUSE");   // requiert d'appuyer sur un touche dans la console pour terminer l'application. permet de voir les dernier msg sur la console
//	cin.ignore(); // // requiert d'appuyer sur Enter dans la console pour terminer l'application. permet de voir les dernier msg sur la console
}


