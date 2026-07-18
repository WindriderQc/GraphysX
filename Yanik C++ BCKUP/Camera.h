#pragma once
#include "StdAfx.h"
#include "TV3D65.h"

class CCamera
{
private:
	CCamera(void);
	~CCamera(void);

public:
  // Interface publique
  void PositionCam(void);
	cTV_3DVECTOR CamPos; 
	cTV_3DVECTOR CamLookAt;

// Fonctions de création et destruction du singleton
  static CCamera *getInstance ()
  {
    if (NULL == TVCAMsingleton)
      {
       // std::cout << "creating singleton." << std::endl;
        TVCAMsingleton =  new CCamera;
      }
    return TVCAMsingleton;
  }

  static void kill ()
  {
    if (NULL != TVCAMsingleton)
      {
        delete TVCAMsingleton;
        TVCAMsingleton = NULL;
      }
  }


 
private:
		static CCamera *TVCAMsingleton;
		//static CTV3D65 *TVMoteur4Cam;
};

// Initialisation du singleton à NULL
CCamera *CCamera::TVCAMsingleton = NULL;
