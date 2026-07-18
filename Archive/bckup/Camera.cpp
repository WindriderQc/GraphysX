#include "StdAfx.h"
#include "Camera.h"

CLCamera *CLCamera::_singleton = NULL;

CLCamera::CLCamera(void) 
	: CamPos(cTV_3DVECTOR(10, 7, -20)), CamLookAt(cTV_3DVECTOR(0,0,0)), bKeyboardNMouseCtrl(false), fMoveFactor(0.08f),
	fWalk(0),fStrafe(0),fRaise(0),sngAngleY(0),sngAngleX(0)
{
	pCamera = pTVScene->GetCamera();
	pCamera->SetViewFrustum(60, 20000, 1.0f);// 0.01f);
	pCamera->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);	

	// Mesh qui peux etre mis a la meme position qu'un autre.  En fesant suivre CameraAim par la camera, ceci 
	// permet de faire rotationner l'objet que l'on regarde sans que la camera ne suive la rotation.
	pTVScene = new CTVScene();
	pCameraAim = pTVScene->CreateMeshBuilder("CameraAim");
	pCameraAim->CreateBox(1, 1, 1, false);

	clInput = CLInput::getInstance();
}
CLCamera::~CLCamera(void)
{
	if(pCamera){ delete(pCamera); pCamera = NULL; }
	if(pTVScene){ delete(pTVScene); pTVScene = NULL; }
}
void CLCamera::SetChase(cTV_3DVECTOR EndPos, cTV_3DVECTOR LookAtInMesh, float fSpeed)
{
	pCamera->ChaseCamera(pCameraAim, &EndPos, &LookAtInMesh, fSpeed, true);
}
void CLCamera::ManageCamCtrl(float fTimeElapsed)
{
	if (bKeyboardNMouseCtrl == true)
	{
		if(clInput->sngWalk) fWalk = clInput->sngWalk*fMoveFactor;      
		if(clInput->sngStrafe) fStrafe = clInput->sngStrafe*fMoveFactor;  
		if(clInput->sngRaise) fRaise = clInput->sngRaise*fMoveFactor;    
		
		/************************
		* Keyboard Ctrl Section *
		*************************/
		//Okay, now for the smothing of the movement... We checked above if we were pressing a key. If so, then we updated the
	    //movement variable to 1 (positive or negative). Here, we lower this value until it get to 0. This method give us a
	    //smoother camera movement. We start by updating the forward and backward (walk) movement
			if(fWalk > 0)
			{
	        fWalk = fWalk - 0.05f; 
	        if(fWalk < 0) fWalk = 0;
			}
			if(fWalk < 0)
			{
	        fWalk = fWalk + 0.05f;
	        if(fWalk > 0) fWalk = 0;
			}
		 //Now, we update the left and right (strafe) movement.
			if(fStrafe > 0)
			{
	        fStrafe = fStrafe - 0.05f;
	        if(fStrafe < 0) fStrafe = 0;
			}
			if(fStrafe < 0)
			{
	        fStrafe = fStrafe + 0.05f;
	        if(fStrafe > 0) fStrafe = 0;
			}
		 //Now, we update the up and down (Raise) movement.
			if(fRaise > 0)
			{
	        fRaise = fRaise - 0.05f;
	        if(fRaise < 0) fRaise = 0;
			}
			if(fRaise < 0)
			{
	        fRaise = fRaise + 0.05f;
	        if(fRaise > 0) fRaise = 0;
			}
	
		/**********************
		* Mouse Ctrl Section  *
		***********************/
		float tmpMouseX = (float)clInput->stMouse.DeltaPosX;
		float tmpMouseY = (float)clInput->stMouse.DeltaPosY;
		//From the mouse return values, we update the camera angles
		//by adding or substracting the mouse return value.
		sngAngleX = sngAngleX - (tmpMouseY / 100.0f);
		sngAngleY = sngAngleY - (tmpMouseX / 100.0f);
		
		//We will add a simple check, so we can't look up at more
		//than 80 degrees nor down than -80 degrees.
		if(sngAngleX > 1.3f) sngAngleX = 1.3f;
		if(sngAngleX < -1.3f) sngAngleX = -1.3f;
		
		//Update the vectors using the angles and positions.
		CamPos.x = CamPos.x + (cosf(sngAngleY) * fWalk * fTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * fStrafe * fTimeElapsed);
		CamPos.y = CamPos.y + (fRaise * fTimeElapsed);  // A vérifier si l'équation est OK
		CamPos.z = CamPos.z + (sinf(sngAngleY) * fWalk * fTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * fStrafe * fTimeElapsed);
		CamLookAt.x = CamPos.x + cosf(sngAngleY);
		CamLookAt.y = CamPos.y + tanf(sngAngleX);
		CamLookAt.z = CamPos.z + sinf(sngAngleY);

		pCamera->SetCamera(CamPos.x,CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);
	}
}