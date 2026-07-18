#include "StdAfx.h"
#include "Scene.h"

CLScene::CLScene(void) : CLSceneTools()
{
	World = NewtonCreate();

	// Init Light and Material
	cTV_3DVECTOR VecDir, VecPos;
	pTVLightEngine = new CTVLightEngine();
	pTVLightEngine->SetGlobalAmbient(0, 0, 0);
	cTV_LIGHT* light = new cTV_LIGHT();
	light->direction = cTV_3DVECTOR(-0.5f, -1, 0.25f);
	light->diffuse = cTV_COLOR(1, 1, 1, 1);
	light->ambient = cTV_COLOR(1, 1, 1, 1);
	light->range = 1000;
	light->type = cTV_LIGHT_DIRECTIONAL;
	iMainLight = pTVLightEngine->CreateLight(light, "Default light");
	pTVLightEngine->EnableLight(iMainLight, true);
	//iMainLight = pLights->CreatePointLight(&VecPos, 0.4, 0.4, 0.4, 50, "GlobalLight", 1.0);
	//pLights->SetSpecularLighting(true);
	pTVLightEngine->SetLightProperties(iMainLight, true, true, false);
}
CLScene::~CLScene(void)
{
	NewtonDestroyAllBodies(World);
	NewtonDestroy(World);  
	clTV3D->pTVScene->DestroyAllMeshes();
}