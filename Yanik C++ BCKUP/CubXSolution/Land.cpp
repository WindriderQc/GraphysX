#include "StdAfx.h"
#include "Land.h"

CLLand::CLLand(string HeightmapPath, string TexturePath, float fWidth, float fHeight, cTV_3DVECTOR Pos)  // Rajouter la pos et le width/Height
{
	pTVPhysics = new CTVPhysics();
	pTVScene = new CTVScene();
	pTVTexFactory = new CTVTextureFactory();
	pTVMaterialfactory = new CTVMaterialFactory();

	pTVLand = pTVScene->CreateLandscape("Ground");
	pTVLand->SetAffineLevel(cTV_AFFINE_LOW);
	
	pTVLand->GenerateTerrain(HeightmapPath.c_str(), cTV_PRECISION_LOWEST, int(fWidth), int(fHeight) ,0,0,0,true);
	pTVLand->SetLightingMode(cTV_LIGHTING_MANAGED);
	
	texGround = pTVTexFactory->LoadTexture(TexturePath.c_str(), "Ground", -1, -1, cTV_COLORKEY_NO, true);
	//pTVLand->ExpandTexture(texGround,0,0,-1,-1,false);
	pTVLand->SetTexture(texGround,-1);
	pTVLand->SetTextureScale(10, 10);
	//Create Materials
    matLand = pTVMaterialfactory->CreateMaterial("land");
    pTVMaterialfactory->SetSpecular(matLand, 0.1f, 0.1f, 0.1f, 1.0f);     
	pTVLand->SetMaterial(matLand ,-1);
	
	
	pTVLand->SetPosition(Pos.x,Pos.y, Pos.z); 
	iLandPhysicBody = pTVPhysics->CreateStaticTerrainBody(pTVLand); 

}


CLLand::~CLLand(void)
{
}

void CLLand::Render(void)
{
pTVLand->Render();
}


int CLLand::GetPhysicBody(void)
{
return(iLandPhysicBody);
}

void CLLand::SetPos(cTV_3DVECTOR pos)
{
pTVLand->SetPosition(pos.x, pos.y, pos.z);
}