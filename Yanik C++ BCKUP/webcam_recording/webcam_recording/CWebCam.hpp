// définition de la classe

#ifndef __WEBCAM_HPP_INCLUDED__
#define __WEBCAM_HPP_INCLUDED__

// header
#include <windows.h>
#include <vfw.h>

// define
#define MAX_IMAGE 5 // Définit le max d'image en mémoire

// structure pour un pixel
struct Pixel
{
	// RGB
	unsigned char R;
	unsigned char G;
	unsigned char B;

	// Couleur
	int	Couleur;
};

// structure pour un bitmap
struct BitMap
{
	int largeur;
	int hauteur;
	Pixel** points;
};

// class de la webcam
class CWebCam
{
public:
	CWebCam();

	// Fonction qui gère la webcam
	BOOL Connect(short largeur, short hauteur);
	BOOL ChooseSource();
	BOOL IsActive();
	void AfficheWebCam();
	void StopConnect();
	void CaptureImage(char* fichier);
	void SaveImgFile(char *fichier);
	void DriverInfo();
	void StartRecord(char *fichier);
	void StopRecord();

	// Variable
	HWND hWnd;
	HWND hWnd_WC;
	HDC	hDC;

private:
	// Variable
	HDC	hDC_WC;
	BOOL WCActive;
};

#endif