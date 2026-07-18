#include "StdAfx.h"
#include "Atmosphere.h"
 
/************************************
// Atmosphere Class
/************************************/ 
 
CLAtmosphere::CLAtmosphere(STSkybox stSkyBoxDay, STSkybox stSkyboxNight)
{
	pTVAtmos = new CTVAtmosphere();
	pTVTexFactory = new CTVTextureFactory();
	pTVLights = new CTVLightEngine();	
	clGlobalVar = CLGlobalVar::getInstance();

	if (stSkyBoxDay.SkyBack)
	{
	stSkyDay = stSkyBoxDay;
	stSkyNight = stSkyboxNight;
	}
	/*else{
		// My cube face ordering is either screwed up, or TV3D has the wrong names for them... too lazy to investigate
		pTVAtmos->SkyBox_SetTexture(
		pTVTexFactory->LoadTexture("Media\\Sky\\Clouds_NegX.dds"), pTVTexFactory->LoadTexture("Media\\Sky\\Clouds_PosX.dds"),
		pTVTexFactory->LoadTexture("Media\\Sky\\Clouds_NegZ.dds"), pTVTexFactory->LoadTexture("Media\\Sky\\Clouds_PosZ.dds"),
		pTVTexFactory->LoadTexture("Media\\Sky\\Clouds_PosY.dds"), pTVTexFactory->LoadTexture("Media\\Sky\\Clouds_NegY.dds"));
	}*/
	

	texSun = pTVTexFactory->LoadTexture("media\\sky\\sun.jpg", "Sun", -1, -1, cTV_COLORKEY_NO, true);

	pTVAtmos->Sun_SetTexture(texSun);
    pTVAtmos->Sun_SetBillboardSize(2);
    pTVAtmos->Sun_Enable(true);

	cTV_LIGHT SunLight;
	SunLight.type = cTV_LIGHT_DIRECTIONAL;
    SunLight.direction = Vector3(1.0, -1.0, 0);
    SunLight.ambient = TVColor(0.0, 0.0, 0.0, 1);
    SunLight.diffuse = TVColor(1.0, 1.0, 1.0, 1);
    SunLight.specular = TVColor(1.0, 1.0, 1.0, 1);
	SunLight.attenuation.x = 0;
    SunLight.attenuation.y = 0;
    SunLight.attenuation.z = 0;
    iIndiceSunlight = pTVLights->CreateLight(&SunLight, "sun");

    iSunOrbitYOffset = 200;

	cTV_LIGHT MoonLight;
    MoonLight.type = cTV_LIGHT_DIRECTIONAL;
    MoonLight.direction = Vector3(1.0f, -1.0f, 0);
    MoonLight.ambient = TVColor(0.2f, 0.2f, 0.2f, 1);
    MoonLight.diffuse = TVColor(0.2f, 0.2f, 0.2f, 1);
    MoonLight.specular = TVColor(0.2f, 0.2f, 0.2f, 1);
    MoonLight.attenuation.x = 0;
    MoonLight.attenuation.y = 0;
    MoonLight.attenuation.z = 0;
    iIndiceMoonlight = pTVLights->CreateLight(&MoonLight, "moon");

	pTVAtmos->SkyBox_SetDepthWrite(false);
	//pTVAtmos->SkyBox_SetScale(1, 1, 1);
	pTVAtmos->SkyBox_Enable(true);
}
CLAtmosphere::~CLAtmosphere(void)
{
	delete(pTVAtmos);  pTVAtmos = NULL;
	delete(pTVTexFactory);  pTVTexFactory = NULL;
	if (pTVLights){delete(pTVLights);  pTVLights = NULL; }
}
int CLAtmosphere::GetSunOrbitYOffset(void)
{
return(iSunOrbitYOffset);
}
void CLAtmosphere::SetSunOrbitYOffset(int Offset)
{
iSunOrbitYOffset = Offset;
}
void CLAtmosphere::UpdateAndRender(long lTimeOfDay, float fFixedAlpha)  
{	
		
		pTVAtmos->SkyBox_Enable(true);
		pTVAtmos->SkyBox_SetTexture(stSkyNight.SkyFront, stSkyNight.SkyBack, stSkyNight.SkyLeft,stSkyNight.SkyRight, stSkyNight.SkyTop, stSkyNight.SkyBottom);
		pTVAtmos->SkyBox_SetColor(1, 1, 1, 1);
		pTVAtmos->SkyBox_Render();
		
		//  REVOIR PKOI FAIRE LE SETTEXTURE AUSSI SOUVENT
		pTVAtmos->SkyBox_SetTexture(stSkyDay.SkyFront, stSkyDay.SkyBack, stSkyDay.SkyLeft,stSkyDay.SkyRight, stSkyDay.SkyTop, stSkyDay.SkyBottom);
		
		float fAlpha;
		if (fFixedAlpha = -1){
			float fSunPositionY = float(SUN_ORBIT_RADIUS * sin(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5) + iSunOrbitYOffset);
			float fSunPositionX = float(SUN_ORBIT_RADIUS * cos(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5));
			pTVAtmos->Sun_SetPosition(fSunPositionX, fSunPositionY, 0);
			//Set the alpha according to the y position of the sun
			fAlpha = (fSunPositionY + SUN_ORBIT_RADIUS - iSunOrbitYOffset) / (SUN_ORBIT_RADIUS * 2);
			//strech the alpha 
			fAlpha *= 2;
			//fAlpha now ranges from 0 to 2, so cap it at 1 again
			//The purpose of this is to avoid stars being visible during the day (the brightness of the day sky is consistant)
			if (fAlpha > 1)
				fAlpha = 1;
		}
		else fAlpha = fFixedAlpha;
		
		clGlobalVar->fDayNightAlpha = fAlpha;
		pTVAtmos->SkyBox_SetColor(1, 1, 1, fAlpha);
		pTVAtmos->SkyBox_Render();
        pTVAtmos->SkyBox_Enable(false);  // recommander avant render atmosphere pour limiter les artifacts
		// mettre les autre effets...  sunglare, etc...
		pTVAtmos->Atmosphere_Render();
		
		//  Adjust the sunlight properties corresponding with the sun's position
		cTV_LIGHT SunLight;
		pTVLights->GetLight(iIndiceSunlight,&SunLight);
        SunLight.direction = Vector3(float(-cos(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5)), float(-sin(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5)), 0);
        float fBrightness = float(1.0 / (1 + pow(M_E, -(fAlpha - 0.6) / 0.05)));
        float fAmbient  = fAlpha * 0.2F;
        SunLight.ambient.r = fAmbient;
        SunLight.ambient.g = fAmbient;
        SunLight.ambient.b = fAmbient;
        SunLight.ambient.a = fAmbient;
        SunLight.diffuse.r = fBrightness;
        SunLight.diffuse.g = fBrightness;
        SunLight.diffuse.b = fBrightness;
        SunLight.diffuse.a = fBrightness;
        SunLight.specular.r = fBrightness;
        SunLight.specular.g = fBrightness;
        SunLight.specular.b = fBrightness;
        SunLight.specular.a = fBrightness;
        pTVLights->SetLight(iIndiceSunlight, &SunLight);

}

/************************************
// Water Class
/************************************/

CLWater::CLWater(void)
{
	pTVScene = new CTVScene();
	pTVTexFactory = new CTVTextureFactory();
	pTVGraphEffect = new CTVGraphicEffect();

	// Creates the reflection/refraction render surfaces, which will be used for water objects.
        pReflectRS = pTVScene->CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
        pReflectRS->SetBackgroundColor(RGBA(0, 0, 0.1906f, 1));

        pRefractRS = pTVScene->CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
        pRefractRS->SetBackgroundColor(RGBA(0, 0, 0.1906f, 1));

        int iDUDV = pTVTexFactory->LoadDUDVTexture("Media\\distortiontexture.dds", "DUDV", -1, -1, 150); 

        pWaterMesh = pTVScene->CreateMeshBuilder("WaterMesh");
        WaterHeight = -50;
		pWaterMesh->AddFloor(iDUDV, -2048, -2048, 2048, 2048, WaterHeight, 16, 16, true);
		pWaterMesh->SetBlendingMode(cTV_BLEND_ALPHA,-1);

        pWaterPlane = new cTV_PLANE(cTV_3DVECTOR(0, 1, 0), -WaterHeight - 0.75f);// between .75 and like 1.3 are the best values
        pTVGraphEffect->SetWaterReflection(pWaterMesh, pReflectRS, pRefractRS, 0, pWaterPlane);
		pTVGraphEffect->SetWaterReflectionBumpAnimation(pWaterMesh,true,0.5,0.5);  //  les 0.5 sont la vitesse en x et y

}
CLWater::~CLWater(void)
{
}
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

/************************************
// Land Class
/************************************/

CLLand::CLLand(string HeightmapPath, string TexturePath, float fWidth, float fHeight, cTV_3DVECTOR Pos, const NewtonWorld* World)  // Rajouter la pos et le width/Height
{
	//pTVPhysics = new CTVPhysics();
	pTVScene = new CTVScene();
	pTVTexFactory = new CTVTextureFactory();
	pTVMaterialfactory = new CTVMaterialFactory();

	pTVLand = pTVScene->CreateLandscape("Ground");
	pTVLand->SetAffineLevel(cTV_AFFINE_LOW);
	
	pTVLand->GenerateTerrain(HeightmapPath.c_str(), cTV_PRECISION_BEST, int(fWidth), int(fHeight) ,0,0,0,true);
	
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
	//iLandPhysicBody = pTVPhysics->CreateStaticTerrainBody(pTVLand); 
	//Collision	=	NewtonCreateBox(World, 100, 1, 100, 0, NULL);
	//Collision	=	NewtonCreateConvexHullFromMesh(World,(const NewtonMesh*)
	//NewtonMX = (NewtonMatrix*)&(pTVLand->GetMatrix());
	//LandPhysicBody = NewtonCreateDynamicBody (World, Collision,(float*)NewtonMX); // crash si on scale le mesh avant, a cause de la gestion des 3 dernier float de la matrix
	//NewtonDestroyCollision (Collision);

}
CLLand::~CLLand(void)
{
}
void CLLand::Render(void)
{
pTVLand->Render();
}
void CLLand::SetPos(cTV_3DVECTOR pos)
{
pTVLand->SetPosition(pos.x, pos.y, pos.z);
}