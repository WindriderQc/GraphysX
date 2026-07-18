#pragma once
#include "StdAfx.h"
#include "SlideObject.h"
#include "Anneau.h"
//#include "Atmosphere.h"
//#include "Animation.h"

class CLLevel
{
public:
	CLLevel(void);
	~CLLevel(void);  
	
private:
	int iLevelValue;
	CLSlideObject* clSlideObject;
	
	CLAnneau* clAnneaux;
	cTV_3DVECTOR VecPosAnneaux[100];
	
	CTVPhysics* pTVPhysics;
	
public:
	void Render(void);
	void LoadLevel(char* PathToSlide, int level);
	void LoadRings(void);
	CTVMesh* GetSlideMesh(void);
	int GetLevelValue(void);
	int GetSlidePhysicBody(void);
	
};