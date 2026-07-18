#pragma once
#include "StdAfx.h"
#include "3DObject.h"

struct STMoveControl
	{
		float Power;
		bool Accel;
		bool Brake;
		float fSteerAngle;
		bool Handbrake;

		bool Up;
		bool Down;
		bool Left;
		bool Right;
	};


class CLBallZ
{
public:
	CLBallZ(void);
	~CLBallZ(void);


private:
	CL3DObject* clBallZShell;
	CL3DObject* clBallZCtrl;

protected:		

			STMoveControl stMoveControl;
public:
	CTVMesh* GetBallZMesh();
	cTV_3DVECTOR GetBallZPos();
	void SetBallZPos(cTV_3DVECTOR pos);
	void SetBallForce(STMoveControl stMoveControl);
	void ManageInput(void);
	
	
	void Render(void);


			//CTVScene* pTVScene;   
			//CTVCamera* pTVCamera;
			CTVInputEngine* pTVInput;			
			//CTVMathLibrary* pTVMaths;                 
			//CTVTextureFactory* pTVTexturefactory;     
			//CTVMaterialFactory* pTVMaterialfactory;                      
	        //CTVPhysics* pTVPhysics;

};
