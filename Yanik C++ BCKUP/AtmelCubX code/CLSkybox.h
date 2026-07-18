#pragma once
#include "StdAfx.h"


class CLSkybox
{
public:
	CLSkybox(string SkyboxesPath, string SkyboxName);
	~CLSkybox(void);

	CTVMesh* GetCubeIconMesh(void);
	void Enable(float alpha);
	void Render(void);
	CTVMesh* mSkyboxCubeIcon;


	
	int texUp,  texDown, texLeft, texRight, texFront, texBack, texCubeIcon;
private:
	CTVScene* pTVScene;
	CTVTextureFactory* pTVTexFactory; 
	CTVAtmosphere* pTVAtmos; 

};