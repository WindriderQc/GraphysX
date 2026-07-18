#include "StdAfx.h"
#include "Camera.h"
#include "TV3DMoteur.h"

namespace GraphysX
{
	CLCamera::CLCamera(float fDegreesFOV, float fFarPlane, float fNearPlane) {
		
		CTVScene pTVScene;

		pCamera = pTVScene.GetCamera();
		pCamera->SetViewFrustum(fDegreesFOV, fFarPlane, fNearPlane);
		pCamera->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);

		// Mesh qui peux etre mis a la meme position qu'un autre.  En fesant suivre CameraAim par la camera, ceci 
		// permet de faire rotationner l'objet que l'on regarde sans que la camera ne suive la rotation.
		pCameraAim = pTVScene.CreateMeshBuilder("CameraAim");
		pCameraAim->CreateBox(1, 1, 1);
		pCameraAim->Enable(false);
	}
	CLCamera::~CLCamera(void)
	{}
	void CLCamera::SetChase(CTVMesh* mesh, cTV_3DVECTOR EndPos, cTV_3DVECTOR LookAtInMesh, float fSpeed)
	{
		//using the camera aim mesh to avoid the camera to be affeted by all followed object rotation
		cTV_3DVECTOR aim = mesh->GetPosition();
		cTV_3DVECTOR rot = mesh->GetRotation();
		pCameraAim->SetPosition(aim.x, aim.y, aim.z);
		pCameraAim->SetRotation(0, rot.y, 0);
		pCamera->ChaseCamera(pCameraAim, &EndPos, &LookAtInMesh, fSpeed, true);
	}
	void CLCamera::Control(float fTimeElapsed, int DeltaPosX, int DeltaPosY)
	{
		if (bKeyboardNMouseCtrl)
		{
			// Stafe left / right
			if (CLTV3D::stKeyboard.KEY_PRESSED[cTV_KEY_LEFTARROW])        fStrafe = fMoveFactor;
			else if (CLTV3D::stKeyboard.KEY_PRESSED[cTV_KEY_RIGHTARROW]) 	fStrafe = -fMoveFactor;
			// move forward / backward
			if (CLTV3D::stKeyboard.KEY_PRESSED[cTV_KEY_UPARROW] != 0)     	fWalk = fMoveFactor;
			else if (CLTV3D::stKeyboard.KEY_PRESSED[cTV_KEY_DOWNARROW] != 0)	fWalk = -fMoveFactor;
			// move Up / Down
			if (CLTV3D::stKeyboard.KEY_PRESSED[cTV_KEY_RIGHTCONTROL] != 0)   	
				fRaise = -fMoveFactor;
			else if (CLTV3D::stKeyboard.KEY_PRESSED[cTV_KEY_RIGHTSHIFT] != 0)  fRaise = fMoveFactor;

			/************************
			* Keyboard Ctrl Section *
			*************************/
			//Okay, now for the smothing of the movement... We checked above if we were pressing a key. If so, then we updated the
			//movement variable to 1 (positive or negative). Here, we lower this value until it get to 0. This method give us a
			//smoother camera movement. We start by updating the forward and backward (walk) movement
			if (fWalk > 0){
				fWalk = fWalk - 0.05f;
				if (fWalk < 0) fWalk = 0;
			}
			if (fWalk < 0){
				fWalk = fWalk + 0.05f;
				if (fWalk > 0) fWalk = 0;
			}
			//Now, we update the left and right (strafe) movement.
			if (fStrafe > 0){
				fStrafe = fStrafe - 0.05f;
				if (fStrafe < 0) fStrafe = 0;
			}
			if (fStrafe < 0){
				fStrafe = fStrafe + 0.05f;
				if (fStrafe > 0) fStrafe = 0;
			}
			//Now, we update the up and down (Raise) movement.
			if (fRaise > 0){
				fRaise = fRaise - 0.05f;
				if (fRaise < 0) fRaise = 0;
			}
			if (fRaise < 0){
				fRaise = fRaise + 0.05f;
				if (fRaise > 0) fRaise = 0;
			}

			/**********************
			* Mouse Ctrl Section  *
			***********************/
			float tmpMouseX = (float)DeltaPosX;
			float tmpMouseY = (float)DeltaPosY;
			//From the mouse return values, we update the camera angles
			//by adding or substracting the mouse return value.
			sngAngleX = sngAngleX - (tmpMouseY / 100.0f);
			sngAngleY = sngAngleY - (tmpMouseX / 100.0f);

			//We will add a simple check, so we can't look up at more
			//than 80 degrees nor down than -80 degrees.
			if (sngAngleX > 1.3f) sngAngleX = 1.3f;
			if (sngAngleX < -1.3f) sngAngleX = -1.3f;

			//Update the vectors using the angles and positions.
			CamPos.x = CamPos.x + (cosf(sngAngleY) * fWalk * fTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * fStrafe * fTimeElapsed);
			CamPos.y = CamPos.y + (fRaise * fTimeElapsed);  // TODO: A vérifier si l'équation est OK
			CamPos.z = CamPos.z + (sinf(sngAngleY) * fWalk * fTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * fStrafe * fTimeElapsed);
			CamLookAt.x = CamPos.x + cosf(sngAngleY);
			CamLookAt.y = CamPos.y + tanf(sngAngleX);
			CamLookAt.z = CamPos.z + sinf(sngAngleY);

			pCamera->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);
		}
	}
}