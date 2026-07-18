#include "StdAfx.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Clock.h"
#include "MeshBillboard.h"  // TODO: check pkoi l;es autre header sont dans le .h...  devrait etre ici
#include "Spline3D.h"
#include "Tools.h"


#define LAND_CHUNKS 8


namespace GraphysX
{
	cTV_3DVECTOR CLScene::Mouse3DPos = { 0, 0, 0 };

	CLScene::CLScene(void) //  TODO:    A rendre configurable!!!!!!!     Position Light, background, etc...
	{
		CLClock::Init(0,2000);
		CLTV3D::pTVScene.SetAutoTransColor(cTV_COLORKEY_USE_ALPHA_CHANNEL);// cTV_COLORKEY_MAGENTA 
		CLTV3D::pTVScene.SetBackgroundColor(0, 0, 0);// 0.0f, 0.3f, 0.6f); // We set background color.
		CLTV3D::pTVScene.SetRenderMode(cCONST_TV_RENDERMODE::cTV_SOLID);
		CLTV3D::pTVScene.SetShadeMode(cCONST_TV_SHADEMODE::cTV_SHADEMODE_GOURAUD); // cTV_SHADEMODE_PHONG);  // TODO: check what is best
		CLTV3D::pTVScene.SetShadowParameters(RGBA(0.0, 0.0, 0.0, 0.5), true);  // todo   tester l'effet... recup de vieux code
		

		/*
		pScene->LoadTexture("Media\\Winter\\up.jpg", -1, -1, "SkyTop");
		pScene->LoadTexture("Media\\Winter\\down.jpg", -1, -1, "SkyBottom");
		pScene->LoadTexture("Media\\Winter\\left.jpg", -1, -1, "SkyLeft");
		pScene->LoadTexture("Media\\Winter\\right.jpg", -1, -1, "SkyRight");
		pScene->LoadTexture("Media\\Winter\\front.jpg", -1, -1, "SkyFront");
		pScene->LoadTexture("Media\\Winter\\back.jpg", -1, -1, "SkyBack");
		//*/
		texDayUp = ResourceManager::getTexture("Sky\\LostValley\\lostvalley_up.bmp");
		texDayDown = ResourceManager::getTexture("Sky\\LostValley\\lostvalley_down.bmp"); 
		texDayLeft = ResourceManager::getTexture("Sky\\LostValley\\lostvalley_north.bmp"); 
		texDayRight = ResourceManager::getTexture("Sky\\LostValley\\lostvalley_south.bmp"); 
		texDayFront = ResourceManager::getTexture("Sky\\LostValley\\lostvalley_west.bmp"); 
		texDayBack = ResourceManager::getTexture("Sky\\LostValley\\lostvalley_east.bmp");
		/*
		texDayUp = pTVTexFactory->LoadTexture("media\\sky\\clearblue\\up.jpg","DaySkyTop", -1, -1, cTV_COLORKEY_NO, true);
		texDayDown = pTVTexFactory->LoadTexture("media\\sky\\clearblue\\down.jpg", "DaySkyBottom", -1, -1, cTV_COLORKEY_NO, true);
		texDayLeft = pTVTexFactory->LoadTexture("media\\sky\\clearblue\\left.jpg", "DaySkyLeft", -1, -1, cTV_COLORKEY_NO, true);
		texDayRight = pTVTexFactory->LoadTexture("media\\sky\\clearblue\\right.jpg", "DaySkyRight", -1, -1, cTV_COLORKEY_NO, true);
		texDayFront = pTVTexFactory->LoadTexture("media\\sky\\clearblue\\front.jpg","DaySkyFront", -1, -1, cTV_COLORKEY_NO, true);
		texDayBack = pTVTexFactory->LoadTexture("media\\sky\\clearblue\\back.jpg", "DaySkyBack", -1, -1, cTV_COLORKEY_NO, true);
		*/
		texNightUp = ResourceManager::getTexture("Sky\\clearnight\\up.jpg");
		texNightDown = ResourceManager::getTexture("Sky\\clearnight\\down.jpg");
		texNightLeft = ResourceManager::getTexture("Sky\\clearnight\\left.jpg");
		texNightRight = ResourceManager::getTexture("Sky\\clearnight\\right.jpg");
		texNightFront = ResourceManager::getTexture("Sky\\clearnight\\front.jpg");
		texNightBack = ResourceManager::getTexture("Sky\\clearnight\\back.jpg");

		texSun = ResourceManager::getTexture("Sky\\sun.jpg");

		CLTV3D::pTVAtmos.Sun_SetTexture(texSun);
		CLTV3D::pTVAtmos.Sun_SetBillboardSize(2);
		CLTV3D::pTVAtmos.Sun_Enable(true);

		iSunOrbitYOffset = 200;
		cTV_LIGHT SunLight;
		SunLight.type = cTV_LIGHT_DIRECTIONAL;
		SunLight.direction = Vector3(1.0, -1.0, 0);
		SunLight.ambient = TVColor(0.0, 0.0, 0.0, 1);
		SunLight.diffuse = TVColor(1.0, 1.0, 1.0, 1);
		SunLight.specular = TVColor(1.0, 1.0, 1.0, 1);
		SunLight.attenuation.x = 0;
		SunLight.attenuation.y = 0;
		SunLight.attenuation.z = 0;
		iIndiceSunlight = CLTV3D::pTVLightEngine.CreateLight(&SunLight, "sun");
		cTV_LIGHT MoonLight;
		MoonLight.type = cTV_LIGHT_DIRECTIONAL;
		MoonLight.direction = Vector3(1.0, -1.0, 0);
		MoonLight.ambient = TVColor(0.2, 0.2, 0.2, 1);
		MoonLight.diffuse = TVColor(0.2, 0.2, 0.2, 1);
		MoonLight.specular = TVColor(0.2, 0.2, 0.2, 1);
		MoonLight.attenuation.x = 0;
		MoonLight.attenuation.y = 0;
		MoonLight.attenuation.z = 0;
		iIndiceMoonlight = CLTV3D::pTVLightEngine.CreateLight(&MoonLight, "moon");




		CLTV3D::pTVLightEngine.SetGlobalAmbient(0, 0, 0);
		CLTV3D::pTVLightEngine.SetSpecularLighting(true);
		cTV_LIGHT light;
		// Create directional light
		light.type = cTV_LIGHT_DIRECTIONAL;
		light.direction = cTV_3DVECTOR(-1, -1, 1);
		light.diffuse = cTV_COLOR(1, 1, 1, 1);
		light.ambient = cTV_COLOR(1, 1, 1, 1);
		light.range = 10;
		addLight(light, "Default light", false, true, false);
		// create point light - could be used as the sun if the default light color is changed for day and night.
		light.type = cTV_LIGHT_POINT;
		light.position = Vector3(25, 10, -5);
		light.direction = cTV_3DVECTOR(-1, -1, 1);
		light.diffuse = cTV_COLOR(1, 1, 1, 1); // (1.0f, 0.94f, 0.86f, 1.0f);  // HALOGEN light color
		light.range = 100;
		defaultLight = addLight(light, "pointlight", true, true, true);
		
		clCamera.bKeyboardNMouseCtrl = true;

		// The land generation    //   ACTUALLY only used for the mouse picking
		pLand = new CTVLandscape();
		pLand = CLTV3D::pTVScene.CreateLandscape("Land");
		pLand->CreateEmptyTerrain(cTV_PRECISION_LOW, LAND_CHUNKS, LAND_CHUNKS, -LAND_CHUNKS * 256 / 2, 0, -LAND_CHUNKS * 256 / 2);
		// Generate the height of the land from the grayscale of the image.
		//pLand->GenerateTerrain("Media\\Heightmaps\\Track.jpg", cTV_PRECISION_LOW, 8, 8, -1024,-100, -1024, true);
		// We assign a texture to that land.
		//pLand->SetTexture(iDamierTex);
		pLand->Enable(false);
		
		

		cycleTexFire = loadFireCycleTex();



		//Physics
		World = NewtonCreate();
		std::cout << "Scene with Newton world created.  Newton Version : " << NewtonWorldGetVersion() << std::endl;
	}
	CLScene::~CLScene(void)
	{
		std::cout << "Will destroy all physic bodies" << std::endl;
//		int size = vecDynPrim.size();
		int i = 0;
		/*if  (size != 0)
		{
			for (CLDynamicMesh* iter : vecDynPrim) { // doit s'assurer que les Collision de physgeom soient toutes destroyed avant le Destroy Body
				if (iter)
				{
					std::cout << "deleting : " << i <<" - " << iter->stParam.name << std::endl;
					delete(iter); iter = nullptr;
					i++;
				}
			}
		}*/
		NewtonDestroyAllBodies(World);
		NewtonDestroy(World);

		CLTV3D::pTVScene.DestroyAllMeshes();
		CLTV3D::pTVScene.DestroyAllActors();   //  A mettre dans les manager associé ptete?
		CLTV3D::pTVScene.DestroyAllParticleSystems(); //  A mettre dans les manager associé ptete?
		CLTV3D::pTV.ReleaseAll();
		std::cout << "Scene destructed." << std::endl;
	}

	
	int CLScene::loadFireCycleTex()
	{
		CTVTextureFactory pTex;
		const int nbrTex = 50;
		int flameTex[nbrTex];
		std::string name;

		for (int i = 1; i <= nbrTex; i++)
		{
			if (i<10)
				name = "Flames\\256X256\\50 frames\\fire1_ 0" + Tools::to_string(i) + ".png";
			else
				name = "Flames\\256X256\\50 frames\\fire1_ " + Tools::to_string(i) + ".png";

			flameTex[i - 1] = ResourceManager::getTexture(name);

		}

		int fireCycleTex = pTex.CreateTextureCycle(nbrTex, flameTex, 15);
		return(fireCycleTex);
	}
	int CLScene::addLight(cTV_LIGHT light, char* name, bool meshmark, bool enable, bool castShadows)
	{
		int iLight = CLTV3D::pTVLightEngine.CreateLight(&light, name);
		CLTV3D::pTVLightEngine.EnableLight(iLight, enable);
		CLTV3D::pTVLightEngine.SetLightProperties(iLight, true, castShadows, false);  // ???   what is lightmapping? false?
		//Create a small sphere and put it in the same place as the light to give it a visual marker.
		if (meshmark)
		{
			CTVMesh *m_LightMark;
			m_LightMark = CLTV3D::pTVScene.CreateMeshBuilder("lightmark"); //  Mesh non gerer par le MeshFactory, valider que ca ne cause pas de bug.
			m_LightMark->CreateSphere(0.25f, 6, 6);
			m_LightMark->SetColor(RGBA(light.ambient.r, light.ambient.g, light.ambient.b, light.ambient.a));
			m_LightMark->SetPosition(light.position.x, light.position.y, light.position.z);
		}
		return(iLight);
	}
	cTV_3DVECTOR CLScene::getDefaultLightPos()
	{
		cTV_LIGHT light;
		CLTV3D::pTVLightEngine.GetLight(defaultLight, &light);
		return(light.position);
	}
	void CLScene::Render()
	{


	/*	if (clGlobalVar->GetRenderAtmos())
		{
			// Pre Render everything reflecting in the water
			clWater->StartReflectRender();
			clAtmos->pTVAtmos.Atmosphere_Render();
			clCubXScene->Render();
			clWater->StopReflectRender();
			clWater->StartRefractRender();
			clAtmos->pTVAtmos.Atmosphere_Render();
			clCubXScene->Render();
			clWater->StopRefractRender();

			//////// Start rendering block
			clTV3D->StartRenderingBlock();

			clAtmos->UpdateAndRender(TimeElapsed);
			clWater->Render();
		}
		else
		{
			//////// Start rendering block
			clTV3D->StartRenderingBlock();
		}
		*/


		UpdateRenderAtmos();

		CLTV3D::pTVScene.RenderAll(true);
		CLTV3D::pTVScene.FinalizeShadows();
		CLTV3D::DrawTV3DAxis();
		Draw3DLines(); 

		if (false)   // TODO: en mode debug, on voit un second physic floor inutile.......  beurk
		{
		for (auto& iter : vDynPrim) {
			iter->NewtonDebug();
			}
		}
	}
	void CLScene::Update(float fTimeElapsed)
	{
		CLClock::update(fTimeElapsed);
		CLTV3D::GetUserInput(); //Update Keyboard, Mouse and Controller buffers 
		clCamera.Control(fTimeElapsed, CLTV3D::stMouse.DeltaPosX, CLTV3D::stMouse.DeltaPosY);

		/*for (auto& iter : vEntity) {
			iter->update();
		}*/

		for (std::vector<CLEntity3D*>::iterator it = vEntity.begin(); it != vEntity.end();)  {
			(*it)->update();
			if ((*it)->isDead())  {
				delete * it;  
				it = vEntity.erase(it);
			}
		   else {
			  ++it;
		   }
		}

		if (pMeshClicked)
			pLand->SetPosition(-LAND_CHUNKS * 256 / 2, pMeshClicked->GetPosition().y, -LAND_CHUNKS * 256 / 2);  
		else
			pLand->SetPosition(-LAND_CHUNKS * 256 / 2, 0, -LAND_CHUNKS * 256 / 2);
		UpdateMouse3DPos();
		UpdatePhysics();
	}
	void CLScene::UpdateRenderAtmos()
	{
		lTimeOfDay = GraphysX::CLClock::getTimeOfDay();

		CLTV3D::pTVAtmos.SkyBox_Enable(true);
		CLTV3D::pTVAtmos.SkyBox_SetTexture(texNightFront, texNightBack, texNightLeft, texNightRight, texNightUp, texNightDown);
		CLTV3D::pTVAtmos.SkyBox_SetColor(1, 1, 1, 1);
		CLTV3D::pTVAtmos.SkyBox_Render();
		CLTV3D::pTVAtmos.SkyBox_SetTexture(texDayFront, texDayBack, texDayLeft, texDayRight, texDayUp, texDayDown);

		float fSunPositionY = float(SUN_ORBIT_RADIUS * sin(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5) + iSunOrbitYOffset);

		//Set the alpha according to the y position of the sun
		float  fAlpha = (fSunPositionY + SUN_ORBIT_RADIUS - iSunOrbitYOffset) / (SUN_ORBIT_RADIUS * 2);
		//strech the alpha 
		fAlpha *= 2;
		//alpha now ranges from 0 to 2, so cap it at 1 again
		//The purpose of this is to avoid stars being visible during the day (the brightness of the day sky is
		//consistant)
		if (fAlpha > 1)
			fAlpha = 1;

		CLTV3D::pTVAtmos.SkyBox_SetColor(1, 1, 1, fAlpha);
		CLTV3D::pTVAtmos.SkyBox_Render();
		CLTV3D::pTVAtmos.Sun_SetPosition(float(SUN_ORBIT_RADIUS * cos(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5)), fSunPositionY, 0);

		CLTV3D::pTVAtmos.SkyBox_Enable(false);
		CLTV3D::pTVAtmos.Atmosphere_Render();

		cTV_LIGHT SunLight;
		CLTV3D::pTVLightEngine.GetLight(iIndiceSunlight, &SunLight);
		SunLight.direction = Vector3(float(-cos(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5)), float(-sin(2 * M_PI * lTimeOfDay / MILLISEC_PER_DAY - M_PI * 0.5)), 0);
		float fBrightness = float(1.0 / (1 + pow(M_E, -(fAlpha - 0.6) / 0.05)));
		float fAmbient = fAlpha * 0.2F;

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
		CLTV3D::pTVLightEngine.SetLight(iIndiceSunlight, &SunLight);
	}
	

	void CLScene::mouseClick(int x, int y)
	{
		CTVCollisionResult* ColiRes = CLTV3D::pTVScene.MousePick(x, y);
		if (ColiRes->IsCollision())	{
			CTVMesh* tvmesh = ColiRes->GetCollisionMesh();
			if (tvmesh)
			{
				pOldMeshClicked = pMeshClicked;
				pMeshClicked = tvmesh;
			}
			else pMeshClicked = NULL;
		}
	}
	void CLScene::MouseHighlight()
	{
		sLine3D MouseTag;
		MouseTag.Start = Mouse3DPos;
		MouseTag.End = cTV_3DVECTOR(Mouse3DPos.x, Mouse3DPos.y + 1, Mouse3DPos.z);
		MouseTag.Color = RGBA_YELLOW;
		vecLine3D.push_back(MouseTag);
	}
	void CLScene::UpdateMouse3DPos() 
	{
		pLand->Enable(true);
		CTVCollisionResult* pMouseCollision = CLTV3D::pTVScene.MousePick(CLTV3D::stMouse.PosX, CLTV3D::stMouse.PosY, cTV_OBJECT_LANDSCAPE, cTV_TESTTYPE_ACCURATETESTING);
		if (pMouseCollision->IsCollision())
			Mouse3DPos = pMouseCollision->GetCollisionImpact();
		pLand->Enable(false);
	}
	cTV_3DVECTOR CLScene::getMouse3DPos()
	{
		return Mouse3DPos; 
	}
	void CLScene::Draw3DLines(void)
	{
		if (vecLine3D.size()){
			for each (sLine3D line in vecLine3D)
				CLTV3D::DrawTV3DLine(line.Start, line.End, line.Color);
		}
		vecLine3D.clear(); // clear the line buffer each frame
	}
	void CLScene::UpdatePhysics()
	{
		static bool bFirst = true;
		if (bFirst)
		{
			dResetTimer();
			m_microsecunds = dGetTimeInMicroseconds();
			bFirst = false;
		}

		//std::cout << "PhysIN: " << m_microsecunds << std::endl;
		dFloat timestepInSecunds = 1.0f / MAX_PHYSICS_FPS;
		unsigned64 timestepMicrosecunds = unsigned64(timestepInSecunds * 1000000.0f);
		unsigned64 currentTime = dGetTimeInMicroseconds();
		//std::cout << "current time: " << currentTime << std::endl;
		unsigned64 nextTime = currentTime - m_microsecunds;
		//std::cout << "Next    time: " << nextTime << "  /  microsecunds : " << m_microsecunds << std::endl;
		bool loop = false;
		while (nextTime >= timestepMicrosecunds)
		{
			loop = true;
			NewtonUpdate(World, timestepInSecunds);
			nextTime -= timestepMicrosecunds;
			m_microsecunds += timestepMicrosecunds;
			//std::cout << "Update  -  time: " << nextTime << "  /  " << timestepMicrosecunds << std::endl;
		}
		if (loop) {
			m_physicsTime = dFloat(dGetTimeInMicroseconds() - currentTime) / 1000000.0f;
			//std::cout << "Physic Time: " << m_physicsTime << std::endl;
			if (m_physicsTime >= (1.0f / MAX_PHYSICS_FPS)) {
				m_microsecunds = currentTime;
			}
		}
	//	std::cout << "PhysOUT: " << time << std::endl;
	}


	// Entity creation 
	void CLScene::ClearScene()
	{
		std::cout << "Clear Scene";
		pMeshClicked = NULL;
		
		for (auto& iter : vEntity) {  // TODO : serait bien si les entoty a la destruction s'enlevait du vector, et s'ajoutait a la creation mais...   si les object s'enleve a la destruction, bug ici... car double delete
			delete(iter); iter = nullptr;
			std::cout << ".";	
		}
		vEntity.clear();  // TODO : check si le clear est pertinent...  semble que oui sinon au prochain CLScene-update, ca crash a iter->update
		/*while (!CLEntity3D::vecEntity.empty())
		{
			delete (*CLEntity3D::vecEntity.begin());
		}*/
		/*auto vectorCopy = vEntity;
		int i = 0;
		for (auto& iter : vectorCopy) {
			std::cout << i << " : " << iter->name << std::endl;
			i++;
			delete iter;
		}*/
		std::cout << "All entities deleted" << std::endl;
	}
	void CLScene::createFire(cTV_3DVECTOR pos)  //  TODO: create Billboard function et mettre create fire dans helper func
	{
		CLMeshBillboard* billboard = new CLMeshBillboard(STMeshParam("", eGeometry::NON_PRIMITIVE, pos, { 2, 2, 1 }), cycleTexFire);
		vEntity.push_back(billboard);
	}

	CLSpline3D*  CLScene::add3DSpline(Spline spline, bool isVisible)
	{
		
		CLSpline3D* spline3D = new CLSpline3D(this, spline);
		return(spline3D);

	}
	CLDynamicMesh* CLScene::addDynamicMesh( STMeshParam& stGeom, ePHYSMAT physMat,std::string texName, float masse)
	{
		vDynPrim.push_back(new CLDynamicMesh(World, stGeom, physMat, masse));
		vEntity.push_back(vDynPrim.back());
		if (texName != "")  
			vDynPrim.back()->mesh->SetTexture(ResourceManager::getTexture(texName));
		return(vDynPrim.back());
	}
	PushPlate* CLScene::addPushplate(std::string texName, STMeshParam& stGeom, ePHYSMAT physMat, cTV_3DVECTOR forcepush, cTV_3DVECTOR forcepull, float masse)
	{
		vEntity.push_back(new PushPlate(World, stGeom, &forcepush.x, &forcepull.x, 0.1f));
		if (texName != "")
			((PushPlate*)vEntity.back())->mesh->SetTexture(ResourceManager::getTexture(texName));
		return((PushPlate*)vEntity.back());
	}
	CLMesh3DText* CLScene::addMesh3DText(char* text, char* fontname, float fontsize, STMeshParam& stParam, float extrude)
	{
		CLMesh3DText* text3D = new CLMesh3DText(text, fontname, fontsize, stParam, extrude);
		vEntity.push_back(text3D);
		text3D->loadMesh();
		return(text3D);
	}
	Bullet* CLScene::shotBullet(std::string texName, cTV_3DVECTOR pos, cTV_3DVECTOR dir, float offset, float diameter, ePHYSMAT physMat, float speed, float masse, int lifetime)
	{
		
		CTVMathLibrary math;
		cTV_3DVECTOR tmp, nDir;

		STMeshParam param("Bullet", eGeometry::SPHERE, pos, { diameter, diameter, diameter });
		nDir = math.VNormalize(&dir);
		math.TVVec3Scale(&tmp, &nDir, offset);
		param.pos = param.pos + tmp;  // pour éviter que la balle soit creer dans le player et l'affecte de sa physique
		param.speed = speed;
		Bullet* bullet = new Bullet(World, param, nDir, lifetime, masse, physMat);
		if (texName != "")	bullet->mesh->SetTexture(ResourceManager::getTexture(texName));
		vDynPrim.push_back(bullet);
		vEntity.push_back(bullet);
		std::cout << "Bullet created: " << bullet->getNbrBullets() << std::endl;
		return(bullet);
	}
	CLKinematicPrimitive* CLScene::addKinematicPrimitive(std::string texName, STMeshParam& stGeom, ePHYSMAT physMat)
	{
		vKinPrim.push_back(new CLKinematicPrimitive(World, stGeom, ePHYSMAT::DEF_PHYSMAT));
		vEntity.push_back(vKinPrim.back());
		if (texName != "")  
			vKinPrim.back()->mesh->SetTexture(ResourceManager::getTexture(texName));
		return(vKinPrim.back());
	}
	CLMeshPrimitive* CLScene::addMeshPrimitive(std::string texName, STMeshParam& stGeom)
	{
		//vEntity.push_back(std::make_unique<CLMeshPrimitive>(stGeom));
		CLMeshPrimitive* prim = new CLMeshPrimitive(stGeom);
		vEntity.push_back(prim);
		mapMeshPrim.insert(std::make_pair(prim->mesh, prim));
		if (texName != "")
			prim->mesh->SetTexture(ResourceManager::getTexture(texName));
		return(prim);
	}
	Human* CLScene::addSphereHuman(STMeshParam& stGeom, float masse, int tex)
	{
		//vDynPrim.push_back();
		vEntity.push_back(new Human(World, stGeom, masse, tex));
		return((Human*)vEntity.back());
	}
	CLMeshBillboard* CLScene::addMeshBillboard(STMeshParam& stGeom, std::string texName)
	{
		CLMeshBillboard* billboard = new CLMeshBillboard(stGeom, ResourceManager::getTexture(texName));
		vEntity.push_back(billboard);
		billboard->loadMesh();
		return(billboard);
	}
	// Creation helper function
	
	CLMeshPrimitive* CLScene::addPrimSphere(std::string name, cTV_3DVECTOR pos, float radius, std::string textureName)// TODO: ajouter la possibilité de setter le nombre de slice.  car good pour planet mais les bullet sont trop high res
	{
		STMeshParam stParam(name, eGeometry::SPHERE, pos, Vector3(radius, radius, radius));
		return(addMeshPrimitive(textureName, stParam));
	}  
	CLMeshPrimitive* CLScene::addPrimCube(std::string name, cTV_3DVECTOR pos, cTV_3DVECTOR scale, std::string textureName)
	{
		STMeshParam stParam(name, eGeometry::CUBE, pos, scale);
		return(addMeshPrimitive(textureName, stParam));
	}
	CLMeshPrimitive* CLScene::addPrimCylinder(std::string name, cTV_3DVECTOR pos, float radius, float height, std::string textureName)
	{
		STMeshParam stParam(name, eGeometry::CYLINDER, pos, Vector3(radius, height, radius));
		return(addMeshPrimitive(textureName, stParam));
	}
	
	// method used to configure new Physic Material
	
	void CLScene::setPhysmatElasticity(ePHYSMAT mat1, ePHYSMAT mat2, float elast)
	{
		NewtonMaterialSetDefaultElasticity(World, mat1, mat2, elast);
	}
	void CLScene::setPhysmatFriction(ePHYSMAT mat1, ePHYSMAT mat2, float staticFric, float kineticFric)
	{
		NewtonMaterialSetDefaultFriction(World, ResourceManager::getPhysmat(mat1, World), ResourceManager::getPhysmat(mat2, World), staticFric, kineticFric);
	}
	void CLScene::setPhysmatCollisionCallback(ePHYSMAT mat1, ePHYSMAT mat2, void* collisionCallback)
	{
		NewtonMaterialSetCollisionCallback(World, ResourceManager::getPhysmat(mat1, World), ResourceManager::getPhysmat(mat2, World), NULL, NULL, (NewtonContactsProcess)collisionCallback);
	}
	void CLScene::setMaterialToPhysEntity(NewtonBody* body, ePHYSMAT material)
	{
		NewtonBodySetMaterialGroupID(body, ResourceManager::getPhysmat(material, World));
	}
	
	// Physic Joints creation
	
	CL6DOFJoint* CLScene::Create6DOFJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, dVector LimitsMinAngle, dVector LimitsMaxAngle)
	{
		CL6DOFJoint* cl6DOFJoint = new CL6DOFJoint(ParentBody, ChildBody, LimitsMinAngle, LimitsMaxAngle);
		return(cl6DOFJoint);
	}
	CLSlidingJoint* CLScene::CreateSlidingJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit)
	{
		CLSlidingJoint* clSlidingJoint = new CLSlidingJoint(ParentBody, ChildBody, limitsMinDist, limitsMaxDist, MinAngularLimit, MaxAngularLimit);
		return(clSlidingJoint);
	}






	CLWater::CLWater(void)
	{
		pTVScene = new CTVScene();
		pTVTexFactory = new CTVTextureFactory();
		pTVGraphEffect = new CTVGraphicEffect();

		// Creates the reflection/refraction render surfaces, which will be used for water objects.
		pReflectRS = pTVScene->CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
		pReflectRS->SetBackgroundColor(RGBA(0, 0, 0.1906, 1));

		pRefractRS = pTVScene->CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
		pRefractRS->SetBackgroundColor(RGBA(0, 0, 0.1906, 1));

		int iDUDV = pTVTexFactory->LoadDUDVTexture("Media\\distortiontexture.dds", "DUDV", -1, -1, 150); //distortiontexture.dds

		pWaterMesh = pTVScene->CreateMeshBuilder("WaterMesh");
		WaterHeight = 85;
		pWaterMesh->AddFloor(iDUDV, -2048, -2048, 2048, 2048, WaterHeight, 16, 16, true);
		pWaterMesh->SetBlendingMode(cTV_BLEND_ALPHA, -1);

		pWaterPlane = new cTV_PLANE(cTV_3DVECTOR(0, 1, 0), -WaterHeight - 0.75f);// between .75 and like 1.3 are the best values
		pTVGraphEffect->SetWaterReflection(pWaterMesh, pReflectRS, pRefractRS, 0, pWaterPlane);
		pTVGraphEffect->SetWaterReflectionBumpAnimation(pWaterMesh, true, 0.5, 0.5);  //  les 1 sont la vitesse en x et y

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
}