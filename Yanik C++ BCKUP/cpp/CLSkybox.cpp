#include "CLSkybox.h"

CLSkybox::CLSkybox(string SkyboxesPath, string SkyboxName)
{
mSkyboxCubeIcon = NULL;

pTVScene = new CTVScene();
pTVTexFactory = new CTVTextureFactory();
pTVAtmos = new CTVAtmosphere();

string Path = SkyboxesPath + SkyboxName + "\\CubeIconTex.bmp";
mSkyboxCubeIcon = pTVScene->CreateMeshBuilder("SkyboxCubeIcon");
mSkyboxCubeIcon->CreateBox(50,50,50, true);

// Ajouter le choix entre jpg ou bmp...
Path = SkyboxesPath + SkyboxName + "\\down.jpg";
texDown = pTVTexFactory->LoadTexture(Path.c_str(), "SkyBottom", -1, -1, cTV_COLORKEY_NO, true);
mSkyboxCubeIcon->SetTexture(texDown, 4);

Path = SkyboxesPath + SkyboxName + "\\up.jpg";
texUp = pTVTexFactory->LoadTexture(Path.c_str(),"SkyTop", -1, -1, cTV_COLORKEY_NO, true);
mSkyboxCubeIcon->SetTexture(texUp, 5);

Path = SkyboxesPath + SkyboxName + "\\left.jpg";
texLeft = pTVTexFactory->LoadTexture(Path.c_str(), "SkyLeft", -1, -1, cTV_COLORKEY_NO, true);
mSkyboxCubeIcon->SetTexture(texLeft, 0);

Path = SkyboxesPath + SkyboxName + "\\front.jpg";
texFront = pTVTexFactory->LoadTexture(Path.c_str(),"SkyFront", -1, -1, cTV_COLORKEY_NO, true);
mSkyboxCubeIcon->SetTexture(texFront, 2);

Path = SkyboxesPath + SkyboxName + "\\right.jpg";
texRight = pTVTexFactory->LoadTexture(Path.c_str(), "SkyRight", -1, -1, cTV_COLORKEY_NO, true);
mSkyboxCubeIcon->SetTexture(texRight, 1);

Path = SkyboxesPath + SkyboxName + "\\back.jpg";
texBack = pTVTexFactory->LoadTexture(Path.c_str(), "SkyBack", -1, -1, cTV_COLORKEY_NO, true);
mSkyboxCubeIcon->SetTexture(texBack, 3);

mSkyboxCubeIcon->Enable(false);
}
	
CLSkybox::~CLSkybox(void)
{

}

bool CLSkybox::IsAtmosSkySet(void)
{
	return(pTVAtmos->SkyBox_IsEnabled());

}


CTVMesh* CLSkybox::GetCubeIconMesh(void)
{
	return(mSkyboxCubeIcon);
}


void CLSkybox::Enable(float alpha)
{
pTVAtmos->SkyBox_Enable(true);
pTVAtmos->SkyBox_SetTexture(texFront, texBack, texLeft, texRight, texUp, texDown);
pTVAtmos->SkyBox_SetColor(1, 1, 1, alpha);
}

void CLSkybox::Render(void)
{
pTVAtmos->SkyBox_Render();
mSkyboxCubeIcon->Render();
}