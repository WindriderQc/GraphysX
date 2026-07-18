// Template.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"
#include "Simulation.h"

// suppression de warning a la compilation
#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier
#pragma warning(disable: 4311) //  pointer truncation from 'char *' to 'int'



#define MAX_LOADSTRING 100

HWND formHWND;
bool bDoLoop;

// définition des classe utilisées
// Notre plateforme de simulation
Simulation * Simulateur = 0;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void Unload(void);

void AppPath(char* PathOfFile, char* ret_Path)
{							
	char* found = strrchr(PathOfFile, '\\');
	if(!found)
	{
		// check with '/' path format
		found = strrchr(PathOfFile, '/');
		if(!found)
		{
			// no path herre it's just a file.
			// so just blank output
			ret_Path[0] = 0;
		}
		else
		{
			// copy just a part of the string
			int size = (int)found - (int)PathOfFile + 1;
			strncpy(ret_Path, PathOfFile, size);
			ret_Path[size] = 0;
		}

	}
	else
	{
		// copy just a part of the string
		int size = (int)found - (int)PathOfFile + 1;
		strncpy(ret_Path, PathOfFile, size);
		ret_Path[size] = 0;
	}		
}


// A method for Processing Mesages that will be used in the loop.


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEMPLATE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEMPLATE);

	char path[256];
	char srchpath[256];
	HMODULE Module = (HMODULE)hInstance;
	GetModuleFileName(Module,LPWCH(path),255);  //  PKOI donne juste "C"????
	//AppPath(path,srchpath);	  // a checker marche bizz je crois  
	//char* fullpath = "C:\\Users\\beayani001\\My Documents\\Visual Studio 2005\\Projects\\AtmelCubx\\debug";
	char* fullpath = "E:\\Documents and Settings\\KG\\Bureau\\AtmelCubx\\debug\\";
	strcpy(srchpath,fullpath);

	// Création de notre simulateur
	Simulateur = new Simulation();
	Simulateur->initialiser(formHWND, srchpath);


	// Setup the boolean for the loop.
	bDoLoop = true;

	// Main message loop:
	while (bDoLoop) 
	{
		// Render Loop
		if(GetFocus() == formHWND)
		{
			Simulateur->animer();

			
		} else {
			// So we dont process the messages to many times if we are not rendering.
			Sleep(100);
		}
		
		BOOL peek = PeekMessage(&msg, NULL, 0, 0, TRUE);

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	Unload();
	return (int) msg.wParam;
}
void Unload(void)
{
/*if(cSerialWnd.IsOpen())
	{
	cSerialWnd.Close();  // Close the serial port
	}
*/	
delete Simulateur;


}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEMPLATE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable
	
   //TCHAR szTitle1[100] = "CubX Control Interface with TV3D6.5   V0.2a - C++ by YB";
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);
	
   // Store the Created Window's hWnd in a glboal HWND variable.
   formHWND = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_DESTROY:
		// Stop the loop.
		bDoLoop = false;		

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
