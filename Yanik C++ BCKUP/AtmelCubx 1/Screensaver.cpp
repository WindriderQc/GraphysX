#include "Screensaver.h"


// Initialisation du singleton à NULL
CLScreensaver *CLScreensaver::_singleton = NULL;


CLScreensaver::CLScreensaver(void)
{

	pTVScene = new CTVScene();
	pTVCamera = pTVScene->GetCamera();

	CamPos.x = 0;
	CamPos.y = 0;
	CamPos.z = 0;

	bIsSaving = false;  //  ne sert a rien
	OriginMesh = NULL;
	fTriggerTime = 10000;
	fInactiveTime = 0;
}

CLScreensaver::~CLScreensaver(void)
{
}

void CLScreensaver::StartScreensaver(CTVMesh* RotateAround)
{
	OriginMesh = RotateAround;
	bIsSaving = true;
}

void CLScreensaver::RefreshScreensaver()
{
	pTVCamera->RotateAroundMesh(OriginMesh, true, 5, 1000, 500);

}

void CLScreensaver::StopScreensaver()
{
	OriginMesh = NULL;
	bIsSaving = false;
}

void CLScreensaver::RefreshInactiveTime(float TimeElapsed)
{
fInactiveTime = fInactiveTime + TimeElapsed; 
}

void CLScreensaver::ResetInactiveTime(void)
{
fInactiveTime = 0; 
}

bool CLScreensaver::IsTriggerTimeReached(void)
{
if(fInactiveTime > fTriggerTime)
	return(true);
else
	return(false);
}


void CLScreensaver::Update(bool bIsMouseMoving, float TimeElapsed, CTVMesh* Aim)
{
	if(!bIsMouseMoving)
		RefreshInactiveTime(TimeElapsed);
	
	else{
		ResetInactiveTime();
		StopScreensaver();
		}
	
	if(IsTriggerTimeReached())
		{
		if(GetOriginMesh() == NULL)
			StartScreensaver(Aim);
		
		RefreshScreensaver();
		}	
}