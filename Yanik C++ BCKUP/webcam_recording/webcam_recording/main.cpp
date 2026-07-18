#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "CWebCam.hpp"

// var globale
HINSTANCE hInst; // HINSTANCE de l'app
HWND hWnd_fen; // HWND de la fenetre
HDC hDC; // zone client
CWebCam WebCam; // objet WebCam
char szFile[256];

// tache qui lance l'enregistrement
DWORD WINAPI ThreadRecord(LPVOID param)
{
	// en cours d'utilisation ?
	if(WebCam.IsActive())
		return FALSE;

	// connexion a la cam
	if(!WebCam.Connect(640, 480))
		return FALSE;

	// démarrage de l'enregistrement
	WebCam.StartRecord(szFile);
}

// dialog de sauvegarde de la video
bool SaveFile()
{
	OPENFILENAME ofn;
	ZeroMemory(szFile, sizeof(szFile));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFilter = "Fichier AVI\0*.avi\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = NULL;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Sauvegarder la video sous...";
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = NULL;
	ofn.nFileExtension = NULL;
	ofn.lpstrDefExt = "*.avi";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	if(!GetSaveFileName(&ofn))
		return FALSE;

	return TRUE;
}

// fonction de la fenetre
LRESULT CALLBACK MainProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
	hWnd_fen = hWnd;

	switch(mes)
	{
		// message destruction fenetre
		case WM_DESTROY:
			PostQuitMessage(0); // on quit
			return 0;

		case WM_COMMAND:
		{
			switch(wParam)
			{
				// enregistrement
				case ID_CAPTURE_RECORD:
					// anulation
					if(!SaveFile())
						return FALSE;

					// lancer dans une tache pour optimisation
					CreateThread(NULL, 0, ThreadRecord , 0, 0, NULL);
					return TRUE;

				// menu Fichier->Quitter
				case ID_FICHIER_QUITTER:
					// cam active ?
					if(WebCam.IsActive())
						WebCam.StopConnect();

					// fin
					PostQuitMessage(0);
					return 0;

				// creation d'un aperçu
				case ID_CAPTURE_APERCU:
					// cam active ?
					if(WebCam.IsActive())
						return FALSE;
					
					// connexion a la cam
					if(!WebCam.Connect(640, 480))
						return FALSE;
					
					// affichage cam
					WebCam.AfficheWebCam();
					return TRUE;

				// arret de la capture
				case ID_CAPTURE_STOP:
					// cam active?
					if(!WebCam.IsActive())
						return FALSE;
					
					// totale
					WebCam.StopRecord();
					WebCam.StopConnect();
					return TRUE;

				// menu ? ->Info Driver
				case ID_INFO_DRIVER:
					WebCam.DriverInfo();
					return TRUE;
			}
		}

		default:
			return DefWindowProc(hWnd, mes, wParam, lParam);
	}
}

// fonction WinMain (point d'entrée du programme)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// definition variable
	WNDCLASSEX fenetre; // class de notre fenetre
	HWND f_hWnd; // HWND de la fenetre
	MSG msg; // variable de message

	hInstance = hInst;

	// definition de notre class
	fenetre.cbSize = sizeof(WNDCLASSEX);
	fenetre.style = CS_HREDRAW | CS_VREDRAW;
	fenetre.lpfnWndProc = MainProc;
	fenetre.cbClsExtra = 0;
	fenetre.cbWndExtra = 0;
	fenetre.hInstance = hInstance;
	fenetre.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	fenetre.hCursor = LoadCursor(NULL, IDC_ARROW);
	fenetre.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+2);
	fenetre.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	fenetre.lpszClassName = "std";
	fenetre.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&fenetre); // enregistrement de la class

	// creation de la fenetre
	f_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "std", "ImagiNet", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(f_hWnd, SW_SHOW);

	SetFocus(f_hWnd); // donne focus

	// Donne les handle
	WebCam.hWnd = f_hWnd;
	hDC = GetDC(f_hWnd);
	WebCam.hDC = hDC;

	// reception des messages
	while(GetMessage(&msg, NULL, 0, 0) == TRUE)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0; // fin app
}