#pragma once
#include "StdAfx.h"

class CLWater
{
public:
	CLWater(float fWaterHeight, string sDistortionTex);
	~CLWater(void);

		void StartReflectRender(void);
		void StopReflectRender(void);
		void StartRefractRender(void);
		void StopRefractRender(void);
		void Render(void);

protected:
		CTVScene* pTVScene;
		CTVTextureFactory* pTVTexFactory; 

		CTVGraphicEffect* pTVGraphEffect;
		CTVRenderSurface* pReflectRS; 
		CTVRenderSurface* pRefractRS;
		CTVMesh* pWaterMesh;
		cTV_PLANE* pWaterPlane;



};