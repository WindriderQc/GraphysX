#pragma once
#include "stdafx.h"
#include "Global.h"


class CLTV3DMoteur
{
private:
	CLTV3DMoteur(void);
	~CLTV3DMoteur(void);

public:
	void SetCameraPos(cTV_3DVECTOR campos, cTV_3DVECTOR camlookat);
	void GetCameraPos(void);
	void DrawTV3DAxis(void);
	void SetShowFPS(bool bEnabled);
	void StartRenderingBlock(void);
	void StopRenderingBlock(void);

	float GetTimeElapsed(void);
	

	// Fonctions de création et destruction du singleton
    static CLTV3DMoteur *getInstance ()
	{
		if (NULL == _singleton)
		  {
		    _singleton =  new CLTV3DMoteur;
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
	static CLTV3DMoteur *_singleton;
	CTVEngine* pTV;

	void InitCamera(void);

	CLGlobalVar* clGlobalVar;

public:
	void Init3DEngine(void);
	void ResetCamera(void);

	CTVCamera* pCamera;
	CTVScene* pTVScene;
};