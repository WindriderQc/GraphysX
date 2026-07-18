#pragma once

#include "TV3DMoteur.h"
#include "ODEMoteur.h"
#include "StdAfx.h"
//#include "resrc1.h"      // Pour permettre d'envoyer des messages a l'API windows

#include <math.h>


class Simulation
{
public:
	Simulation(void);
	~Simulation(void);
	// Méthode d'animation du simulateur
	void animer(void);   
	// Méthode d'initialisation du simulateur
	bool initialiser(HWND WindowHandle, char *srchpath);
	// Méthode qui positionne les Meshes de TV3D avec les Geoms de ODE
	void GetGeomsPosNMatrixNMoveMeshes(void);


	bool IsAtmelConnected;
	void SetAtmelConnectionDisplay(bool bActive);

	float SpeedFactor;
	bool bDEBUGmode;	

	
	HWND WindowHWND;
	UINT InputMSG;

protected:
	// Moteur 3D basé sur TrueVision3D
	CLTV3DMoteur* clTV3D;
	// Moteur physique basé sur la bibliothèque ODE
	ODEMoteur* clODEMoteur;
	

	

};
