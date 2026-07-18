#include "StdAfx.h"
#include "Water.h"


CLWater::CLWater(float fWaterHeight, string sDistortionTex)
{
	pTVScene = new CTVScene();
	pTVTexFactory = new CTVTextureFactory();
	pTVGraphEffect = new CTVGraphicEffect();

	// Creates the reflection/refraction render surfaces, which will be used for water objects.
        pReflectRS = pTVScene->CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
        pReflectRS->SetBackgroundColor(RGBA(0, 0, 0.1906, 1));

        pRefractRS = pTVScene->CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
        pRefractRS->SetBackgroundColor(RGBA(0, 0, 0.1906, 1));

		int iDUDV = pTVTexFactory->LoadDUDVTexture(sDistortionTex.c_str(), "DUDV", -1, -1, 150); 

        pWaterMesh = pTVScene->CreateMeshBuilder("WaterMesh");
		pWaterMesh->AddFloor(iDUDV, -2048, -2048, 2048, 2048, fWaterHeight, 16, 16, true);
		pWaterMesh->SetBlendingMode(cTV_BLEND_ALPHA,-1);

        pWaterPlane = new cTV_PLANE(cTV_3DVECTOR(0, 1, 0), -fWaterHeight - 0.75f);// between .75 and like 1.3 are the best values
        pTVGraphEffect->SetWaterReflection(pWaterMesh, pReflectRS, pRefractRS, 0, pWaterPlane);
		pTVGraphEffect->SetWaterReflectionBumpAnimation(pWaterMesh,true,0.5,0.5);  //  les 0.5 sont la vitesse en x et y
}

CLWater::~CLWater(void)
{}
void CLWater::StartReflectRender(void)
{
  pReflectRS->StartRender();
}
void CLWater::StopReflectRender(void)
{
  pReflectRS->EndRender();
}
void CLWater::StartRefractRender(void)
{
  pRefractRS->StartRender();
}
void CLWater::StopRefractRender(void)
{
  pRefractRS->EndRender();
}
void CLWater::Render(void)
{
  pWaterMesh->Render();
}