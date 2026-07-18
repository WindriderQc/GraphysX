#include "StdAfx.h"
#include "CLSkybox.h"

CLSkybox::CLSkybox(string SkyboxesPath, string SkyboxName, string NightSkyboxName)
{
mSkyboxCubeIcon = NULL;
string Path;
pTVScene = new CTVScene();
pTVTexFactory = new CTVTextureFactory();
pTVAtmos = new CTVAtmosphere();

// Ajouter le choix entre jpg ou bmp...
//  Day Skybox
Path = SkyboxesPath + SkyboxName + "\\down.jpg";
texDown = pTVTexFactory->LoadTexture(Path.c_str(), "SkyBottom", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\up.jpg";
texUp = pTVTexFactory->LoadTexture(Path.c_str(),"SkyTop", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\left.jpg";
texLeft = pTVTexFactory->LoadTexture(Path.c_str(), "SkyLeft", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\front.jpg";
texFront = pTVTexFactory->LoadTexture(Path.c_str(),"SkyFront", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\right.jpg";
texRight = pTVTexFactory->LoadTexture(Path.c_str(), "SkyRight", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\back.jpg";
texBack = pTVTexFactory->LoadTexture(Path.c_str(), "SkyBack", -1, -1, cTV_COLORKEY_NO, true);

//  Creates the Cube Icon and sets textures
mSkyboxCubeIcon = pTVScene->CreateMeshBuilder("SkyboxCubeIcon");
mSkyboxCubeIcon->CreateBox(50,50,50, true);
mSkyboxCubeIcon->SetTexture(texDown, 4);
mSkyboxCubeIcon->SetTexture(texUp, 5);
mSkyboxCubeIcon->SetTexture(texLeft, 0);
mSkyboxCubeIcon->SetTexture(texFront, 2);
mSkyboxCubeIcon->SetTexture(texRight, 1);
mSkyboxCubeIcon->SetTexture(texBack, 3);
mSkyboxCubeIcon->Enable(false);

//  Night Skybox
Path = SkyboxesPath + NightSkyboxName + "\\down.jpg";
texNightDown = pTVTexFactory->LoadTexture(Path.c_str(), "NightSkyBottom", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + NightSkyboxName + "\\up.jpg";
texNightUp = pTVTexFactory->LoadTexture(Path.c_str(),"NightSkyTop", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + NightSkyboxName + "\\left.jpg";
texNightLeft = pTVTexFactory->LoadTexture(Path.c_str(), "NightSkyLeft", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + NightSkyboxName + "\\front.jpg";
texNightFront = pTVTexFactory->LoadTexture(Path.c_str(),"NightSkyFront", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + NightSkyboxName + "\\right.jpg";
texNightRight = pTVTexFactory->LoadTexture(Path.c_str(), "NightSkyRight", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + NightSkyboxName + "\\back.jpg";
texNightBack = pTVTexFactory->LoadTexture(Path.c_str(), "NightSkyBack", -1, -1, cTV_COLORKEY_NO, true);
}

CLSkybox::~CLSkybox(void)
{}

CTVMesh* CLSkybox::GetCubeIconMesh(void)
{
	return(mSkyboxCubeIcon);
}

void CLSkybox::EnableDay(float alpha)
{
pTVAtmos->SkyBox_Enable(true);
pTVAtmos->SkyBox_SetTexture(texFront, texBack, texLeft, texRight, texUp, texDown);
pTVAtmos->SkyBox_SetColor(1, 1, 1, alpha);
}

void CLSkybox::EnableNight(float alpha)
{
pTVAtmos->SkyBox_Enable(true);
pTVAtmos->SkyBox_SetTexture(texNightFront, texNightBack, texNightLeft, texNightRight, texNightUp, texNightDown);
pTVAtmos->SkyBox_SetColor(1, 1, 1, alpha);
}

void CLSkybox::Render(void) 
{
pTVAtmos->SkyBox_Render();
}

void CLSkybox::RenderIcon(void) 
{
mSkyboxCubeIcon->Render();
}