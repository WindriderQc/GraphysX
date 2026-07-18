#include "Camera.h"

CCamera::CCamera(void)
{
	pTVScene = new CTVScene();
	
	CamPos.x = 1000;
	CamPos.y = 250;
	CamPos.z = -4000;		
	CamLookAt.x = 0;
	CamLookAt.y = 100;
	CamLookAt.z = 0;
}

CCamera::~CCamera(void)
{
	//   Pas sur si c'Est a faire....  a vérifier
	delete(pTVScene);
	pTVScene = NULL;

}
 void CCamera::PositionCam(void)
 {
	pTVScene->SetCamera(CamPos.x = 1000, CamPos.y = 250, CamPos.z = -4000, 
						CamLookAt.x = 0, CamLookAt.y = 100, CamLookAt.z = 0);
 
 }