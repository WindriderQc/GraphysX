// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"

#pragma comment(lib, "..\\Deps\\Lib\\TV3D65")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2Main")
#pragma comment(lib, "..\\Deps\\Lib\\CEGUIBase-0_d")
#pragma comment(lib, "..\\Deps\\Lib\\CEGUIDirect3D9Renderer-0_d")
//Newton Lib
/*#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\newton\\Debug\\newton_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\core\\Debug\\Core_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\dJointLibrary\\Debug\\dJointLibrary_d")
#pragma comment(lib,"..\\Deps\\Lib\\dMath_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\physics\\Debug\\physics_d")
#pragma comment(lib,"..\\Deps\\Lib\\pthread_d")
#pragma comment(lib,"..\\Deps\\Lib\\dContainers_d")*/
#pragma comment(lib,"..\\Deps\\Lib\\newton_d")
#pragma comment(lib,"..\\Deps\\Lib\\Core_d")
#pragma comment(lib,"..\\Deps\\Lib\\dJointLibrary_d")
#pragma comment(lib,"..\\Deps\\Lib\\dMath_d")
#pragma comment(lib,"..\\Deps\\Lib\\physics_d")
#pragma comment(lib,"..\\Deps\\Lib\\pthread_d")
#pragma comment(lib,"..\\Deps\\Lib\\dContainers_d")

#pragma comment(lib,"..\\Deps\\Lib\\fmod_vc")


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

