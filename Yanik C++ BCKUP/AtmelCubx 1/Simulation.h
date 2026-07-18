#pragma once
#include "Screensaver.h"
#include "TV3DMoteur.h"
#include "StdAfx.h"
#include "Global.h"
#include "Area.h"
#include "Input.h"
#include "TV3DCegui.h"
#include "CarsManager.h"
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

	void GetMeshClicked(void);
	
	void ManageGlobalAction(void);
	void ManageBallAction(void);
	void ManagePlaneAction(void);
	void ManageCamAction(float pTimeElapsed);


	bool IsAtmelConnected;
	void SetAtmelConnectionDisplay(bool bActive);

	float SpeedFactor;
	bool bDEBUGmode;	
	bool bUSE_ODE_PHYSICS;

	
	HWND WindowHWND;
	UINT InputMSG;

protected:
	// Moteur 3D basé sur TrueVision3D
	CLTV3DMoteur* clTV3D;
	CLArea* clArea;

	CLInput* clInput;
	CTVCollisionResult* pCollision;
	CLTV3DCEGUI* clCEGUI;   
	
	CLScreensaver* clScreensaver;

	CLGlobalVar* clGlobalVar;

		
};
