#ifndef __TV3DINTERFACE_H__
#define __TV3DINTERFACE_H__
#pragma once

#include "TV3DMoteur.h"
#include "Input.h"
#include "MeshFactory.h"
#include "Realisateur.h"

#include <vcclr.h>

class TV3DInterface
{
public:
	 TV3DInterface(HWND TVScreenHWND, string PathString);
	 ~TV3DInterface();

	 //void Setup(HWND TVScreenHWND, string PathString);
	 void UpdateLoop();
	 void Cleanup();
	 bool AppStillIdle();


	 CLTV3DMoteur* clTV3D;      CLTV3DMoteur* GetTV3D();  
	 CLInput* clInput;          CLInput* GetInput();              
	 CLXMLData* clXMLData;      CLXMLData* GetclXMLData();
	 MeshFactory* clMeshFactory; MeshFactory* GetMeshFactory();
	 CLGlobalVar* clGlobalVar; 	CLGlobalVar* GetGlobalVar();


	 CLRealisateur* Realisateur; CLRealisateur* GetRealisateur();
	
};





#endif