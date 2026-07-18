// Classe pour la gestion de la webcam

// header
#include <stdio.h>
#include "CWebCam.hpp"

// Constructeur de class
CWebCam::CWebCam()
{ WCActive = FALSE; }

// Fonction de connection de la webcam
BOOL CWebCam::Connect(short largeur, short hauteur)
{
	// Connexion à la webcam
	hWnd_WC = capCreateCaptureWindow("handle", WS_CHILD | WS_VISIBLE, 0, 0, largeur, hauteur, hWnd, 1);

	// Vérifie la connection
	if(!capDriverConnect(hWnd_WC, 0)) // erreur
	{
		MessageBox(NULL, "Erreur lors de l'initialisation de la WebCam.\nReportez-vous à l'aide pour plus d'information.", "Erreur", MB_ICONERROR);
		return FALSE;
	}
	else
	{   
		hDC_WC = GetDC(hWnd_WC); // trouve de DC
		return TRUE;
	}
}

// Fonction qui affiche le preview
void CWebCam::AfficheWebCam()
{
	// Taille du preview = taille de la résolution
	capPreviewScale(hWnd_WC, TRUE);

	// Rafraichissement
	capPreviewRate(hWnd_WC, 15);

	// Affiche
	capPreview(hWnd_WC, 1);

	// Webcam active
	WCActive = TRUE;
}

// Fonction qui arrete l'affichage de la cam
void CWebCam::StopConnect()
{
	capDriverDisconnect(hWnd_WC);
	WCActive = FALSE;
}

// Fonction qui permet d'enregistrer une image
void CWebCam::CaptureImage(char* fichier)
{
	// on capture une seul image
	capCaptureSingleFrameOpen(hWnd_WC);
	capCaptureSingleFrame(hWnd_WC);
	capCaptureSingleFrameClose(hWnd_WC);
	capFileSaveDIB(hWnd_WC, fichier);
}

// Fonction qui sauvegarde l'image dans un fichier
void CWebCam::SaveImgFile(char *fichier)
{ capFileSaveDIB(hWnd_WC, fichier); }

// Fonction qui permet de selectionner la source de capture
BOOL CWebCam::ChooseSource()
{
	if(!capDlgVideoSource(hWnd_WC))
	{
		MessageBox(NULL, "Erreur lors de l'initialisation de la source d'aquisition...\nReportez-vous à l'aide pour plus d'information.", "Erreur", MB_ICONERROR);
		return FALSE;
	}
	else
		return TRUE;
}

// Fonction qui affiche des info sur les drivers
void CWebCam::DriverInfo()
{
	// var
	char driv_name[50];
	char driv_vers[50];
	char message[100];

	// si la cam est active
	if(IsActive())
	{
		capGetDriverDescription(1, driv_name, 50, driv_vers, 10); // info driver

		// si on a pas trouver de nom de driver
		if(strcmp(driv_name, "") == 0)
			sprintf(driv_name, "Information indisponible");

		// si on a pas trouver de version de driver
		if(strcmp(driv_vers, "") == 0)
			sprintf(driv_vers, "Information indisponible");

		sprintf(message, "Nom: %s\nVersion: %s", driv_name, driv_vers); // message
		MessageBox(NULL, message, "Information Driver", MB_ICONINFORMATION);
	}
	else
		MessageBox(NULL, "Aucune webcam n'est active...", "Erreur", MB_ICONERROR);
}

// Verifie si la webcam est active
BOOL CWebCam::IsActive()
{ return(WCActive); }

// début de l'enregistrement
void CWebCam::StartRecord(char *fichier)
{
	capDlgVideoCompression(hWnd_WC); // choix compression video
	capFileSetCaptureFile(hWnd_WC, fichier); // fichier de destination
	capCaptureSequence(hWnd_WC); // lance l'enregistrement
	WCActive = TRUE;
}

// fin de l'enregistrement
void CWebCam::StopRecord()
{ capCaptureStop(hWnd_WC); }