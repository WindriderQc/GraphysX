#pragma once
#include "StdAfx.h"

//  Devrait faire une classe de base Skybox sans CubeIcon et une classe SkyboxWithIcon qui 
//  hérite de la classe de base...
class CLSkybox
{
public:
	CLSkybox(string SkyboxesPath, string SkyboxName, string NightSkyboxName);
	~CLSkybox(void);

	CTVMesh* GetCubeIconMesh(void);
	void EnableDay(float alpha);
	void EnableNight(float alpha);
	void Render(void);
	void RenderIcon(void);
	CTVMesh* mSkyboxCubeIcon;

	// Textures ID
	int texUp,  texDown, texLeft, texRight, texFront, texBack;
	int texNightUp,  texNightDown, texNightLeft, texNightRight, texNightFront, texNightBack;
	int texCubeIcon;

private:
	CTVScene* pTVScene;
	CTVTextureFactory* pTVTexFactory; 
	CTVAtmosphere* pTVAtmos; 

};