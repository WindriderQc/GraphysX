#pragma once
///////////////////////////////////////////////////////////////////////////
//  
//  Simulation.h
//  Header for the Simulation Class
//  This class is used to control all the 3D scenes, input/output, physics, etc....
//
// Use:  
//		#include "Simulation.h"
//
//		Simulateur = new Simulation();
//		Simulateur->Initialize();  
// 
// in the loop:
//		Simulateur->animer();
//
// destroy:
//		delete Simulateur;
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Global.h"
#include "TV3DMoteur.h"
#include "Input.h"
#include "TV3DCegui.h"
#include "Area.h"


#include "CarsManager.h"
//#include "Screensaver.h"


#include <math.h>  //  ptete pas nécessaire



class Simulation
{
public:
	Simulation(void);
	~Simulation(void);
	// Méthode d'animation du simulateur
	void animer(void);   
	// Méthode d'initialisation du simulateur
	bool initialiser(void);
	
	
	void GetMeshClicked(void);
	void ManageGlobalIOAction(void);
	
	/*
	void ManageBallAction(void);
	void ManagePlaneAction(void);
	void ManageCamAction(float pTimeElapsed);
	*/

	float SpeedFactor;
	bool bDEBUGmode;	

	UINT InputMSG;

protected:
	CLGlobalVar* clGlobalVar;
	CLTV3DMoteur* clTV3D;  // Moteur 3D basé sur TrueVision3D
	CLInput* clInput;
	CLArea* clArea;

	
	CLTV3DCEGUI* clCEGUI;   
	
//	CLScreensaver* clScreensaver;
	CTVCollisionResult* pCollision;

	

		
};
