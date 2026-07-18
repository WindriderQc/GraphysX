#pragma once
#include "StdAfx.h"
#include "SlideObject.h"
#include "3DObject.h"

#define PI 3.141592653
#define ROLL_FACTOR 0.9
#define PITCH_UP_FACTOR 0.4
#define PITCH_DOWN_FACTOR 0.5

class CLFlightXScene
{
public:
	CLFlightXScene(void);
	~CLFlightXScene(void);

	void Render(void);

	cTV_3DVECTOR GetPlanePos(void);
	CTVMesh* GetPlaneMesh(void);
	void ManagePlaneInput(float ElapsedTime);


	cTV_3DQUATERNION  QRoll; 
	cTV_3DQUATERNION  QYaw;
	cTV_3DQUATERNION  QPitch; 
	cTV_3DQUATERNION  QCurrent;


private:

		CLSlideObject* clPipe;
		CL3DObject* clPlane;

		CTVScene* pTVScene;   
	    //CTVTextureFactory* pTVTexturefactory;     
		//CTVMaterialFactory* pTVMaterialfactory;                      
	   // CTVPhysics* pTVPhysics;
		CTVInputEngine*	pTVInput;
		CTVMathLibrary* pTVMaths;

};
