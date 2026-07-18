#pragma once
#include "TV3DMoteur.h"
#include "StdAfx.h"
#include "Global.h"
#include "Area.h"
#include "Input.h"
#include "TV3DCegui.h"
#include "Clock.h"
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

	float SpeedFactor;
	bool bDEBUGmode;	

	
	HWND WindowHWND;
	UINT InputMSG;

protected:
	// Moteur 3D basé sur TrueVision3D
	CLTV3DMoteur* clTV3D;
	CLArea* clArea;

	CLInput* clInput;
	CTVCollisionResult* pCollision;
	CLTV3DCEGUI* clCEGUI;   

	CLGlobalVar* clGlobalVar;
		
	CLClock* clClock;


		
};
