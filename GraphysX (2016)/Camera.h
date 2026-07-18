#pragma once
#include "TV3DMoteur.h"

namespace GraphysX
{
	class CLCamera
	{
	public:
		CLCamera(float fDegreesFOV = 60, float fFarPlane = 1000, float fNearPlane = 0.01f);
		~CLCamera(void);
		void Control(float fTimeElapsed, int DeltaPosX, int DeltaPosY);

		
		bool bKeyboardNMouseCtrl = false;
		float fWalk = 0;
		float fStrafe = 0;
		float fRaise = 0;
		float sngAngleY = 0;
		float sngAngleX = 0;
		float fMoveFactor = 0.08f;
		CTVCamera* pCamera;
		CTVMesh* pCameraAim;

		GRAPHYSX_API void SetChase(CTVMesh* mesh, cTV_3DVECTOR EndPos, cTV_3DVECTOR LookAtInMesh, float Speed);

	private:
		cTV_3DVECTOR CamPos = { -1, 4, -6 };
		cTV_3DVECTOR CamLookAt = { 0, 0, 0 };
	};
}