#ifndef __CAMERA_H__
#define __CAMERA_H__
#pragma once
#include "Input.h"

class CLCamera
{
public:
	cTV_3DVECTOR CamPos;
	cTV_3DVECTOR CamLookAt;
	bool bKeyboardNMouseCtrl;
	CTVCamera* pCamera;
	CTVMesh* pCameraAim;    

	void ResetCamera(void);
	void ManageCamCtrl(float fTimeElapsed);
	void SetChase(cTV_3DVECTOR EndPos, cTV_3DVECTOR LookAt, float Speed);

	// Fonctions de création et destruction du singleton
    static CLCamera *getInstance ()
	  {
	  if (NULL == _singleton)
      {
        _singleton =  new CLCamera;
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
	CLCamera(void);
	~CLCamera(void);
	static CLCamera *_singleton;
	
	CTVScene* pTVScene;
	CLInput* clInput;
	float fWalk;
	float fStrafe;
	float fRaise;
	float sngAngleY;
	float sngAngleX;
	float fMoveFactor;

};
#endif