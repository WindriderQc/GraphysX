#include "Camera.h"

CCamera::CCamera(void)
{
//	TVMoteur4Cam = CTV3D65::getInstance (); 
	
	CamPos.x = 1000;
	CamPos.y = 250;
	CamPos.z = -4000;		
	CamLookAt.x = 0;
	CamLookAt.y = 100;
	CamLookAt.z = 0;
}

CCamera::~CCamera(void)
{
}
 void CCamera::PositionCam(void)
 {
	// TVMoteur4Cam->tvScene->SetCamera(CamPos.x = 1000, CamPos.y = 250, CamPos.z = -4000, 
	//								CamLookAt.x = 0, CamLookAt.y = 100, CamLookAt.z = 0);
 
 }