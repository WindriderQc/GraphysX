#include "CLSkybox.h"

CLSkybox::CLSkybox(string SkyboxesPath, string SkyboxName)
{
mSkyboxCubeIcon = NULL;

pTVScene = new CTVScene();
pTVTexFactory = new CTVTextureFactory();
pTVAtmos = new CTVAtmosphere();

string Path = SkyboxesPath + SkyboxName + "\\CubeIconTex.bmp";
//texCubeIcon = pTVTexFactory->LoadTexture(Path,"CubeIcon", -1, -1, cTV_COLORKEY_NO, true);

mSkyboxCubeIcon = pTVScene->CreateMeshBuilder("SkyboxCubeIcon");
mSkyboxCubeIcon->CreateBox(100,100,100);
//mSkyboxCubeIcon->SetTexture(texCubeIcon);
mSkyboxCubeIcon->Enable(false);


// Ajouter le choix entre jpg ou bmp...
Path = SkyboxesPath + SkyboxName + "\\up.jpg";
texUp = pTVTexFactory->LoadTexture(Path.c_str(),"SkyTop", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\down.jpg";
texDown = pTVTexFactory->LoadTexture(Path.c_str(), "SkyBottom", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\left.jpg";
texLeft = pTVTexFactory->LoadTexture(Path.c_str(), "SkyLeft", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\right.jpg";
texRight = pTVTexFactory->LoadTexture(Path.c_str(), "SkyRight", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\front.jpg";
texFront = pTVTexFactory->LoadTexture(Path.c_str(),"SkyFront", -1, -1, cTV_COLORKEY_NO, true);
Path = SkyboxesPath + SkyboxName + "\\back.jpg";
texBack = pTVTexFactory->LoadTexture(Path.c_str(), "SkyBack", -1, -1, cTV_COLORKEY_NO, true);


}
	
CLSkybox::~CLSkybox(void)
{

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