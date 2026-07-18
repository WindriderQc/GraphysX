#pragma once
#include "StdAfx.h"

class CLScreensaver
{
private:
	CLScreensaver(void); 
	~CLScreensaver(void); 

public:
	void StartScreensaver(CTVMesh* RotateAround);
	void RefreshScreensaver();
	void StopScreensaver();
	void RefreshInactiveTime(float TimeElapsed);
	void ResetInactiveTime();
	bool IsTriggerTimeReached(void);
	
	void Update(bool bIsMouseMoving, float TimeElapsed, CTVMesh* Aim);


	void     SetTriggerTime(float fTime) { fTriggerTime = fTime;  }
	CTVMesh* GetOriginMesh()             { return(OriginMesh); }
	bool     IsSaving()			    	 { return(bIsSaving); }

  // Fonctions de création et destruction du singleton
  static CLScreensaver *getInstance ()
  {
    if (NULL == _singleton)
      {
        _singleton =  new CLScreensaver;
      }
     return _singleton;
  }
  static void kill ()
  {
    if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
  }




private:	
	CTVCamera* pTVCamera;
	CTVScene* pTVScene;
	cTV_3DVECTOR CamPos;

	float fInactiveTime;
	float fTriggerTime;
	CTVMesh* OriginMesh;

	bool bIsSaving;


	static CLScreensaver *_singleton;
};