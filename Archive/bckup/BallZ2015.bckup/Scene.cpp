#include "StdAfx.h"
#include "Scene.h"

CLScene::CLScene(void)
	: m_microsecunds(0)
	, m_physicsTime(0.0f)
	, m_physicsUpdate(true)
	, m_reEntrantUpdate(false)
{

	clSceneStaff = new CLSceneStaff();
	// Init Light and Material
	cTV_3DVECTOR VecDir, VecPos;
	cTV_LIGHT* light = new cTV_LIGHT();
	light->direction = cTV_3DVECTOR(-0.5f, -1, 0.25f);
	light->diffuse = cTV_COLOR(1, 1, 1, 1);
	light->ambient = cTV_COLOR(1, 1, 1, 1);
	light->range = 1000;
	light->type = cTV_LIGHT_DIRECTIONAL;
	
	pTVLightEngine = new CTVLightEngine();
	pTVLightEngine->SetGlobalAmbient(0, 0, 0);
	iMainLight = pTVLightEngine->CreateLight(light, "Default light");
	pTVLightEngine->EnableLight(iMainLight, true);
	//iMainLight = pTVLightEngine->CreatePointLight(&VecPos, 0.4, 0.4, 0.4, 50, "GlobalLight", 1.0);
	//pTVLightEngine->SetSpecularLighting(true);
	pTVLightEngine->SetLightProperties(iMainLight, true, true, false);

	/*CTVMesh            *m_LightMark; //We're going to create a little sphere to represent our light.
	//Create a small sphere and put it in the same place as the light to give it a visual marker.
	m_LightMark = clSceneStaff->clTV3D->pTVScene->CreateMeshBuilder("cube");
	m_LightMark->CreateSphere(0.25f, 4, 4);
	m_LightMark->SetPosition(5, 15, -10);*/

	World = NewtonCreate();
	dResetTimer();
	m_microsecunds = dGetTimeInMicroseconds();
	// get the default material ID
	defaultPhysMatID = NewtonMaterialGetDefaultGroupID(World);
	// set default material properties                                    ///   VALEUR A METTRE EN PARAM A LA CREATION DE LA SCENE?
	NewtonMaterialSetDefaultSoftness(World, defaultPhysMatID, defaultPhysMatID, 0.05f);  
	NewtonMaterialSetDefaultElasticity(World, defaultPhysMatID, defaultPhysMatID, 0.4f);
	NewtonMaterialSetDefaultCollidable(World, defaultPhysMatID, defaultPhysMatID, 1);
	NewtonMaterialSetDefaultFriction(World, defaultPhysMatID, defaultPhysMatID, 1.0f, 0.5f);
}
CLScene::~CLScene(void)
{
	NewtonDestroyAllBodies(World);
	NewtonDestroy(World);  
	clSceneStaff->clTV3D->pTVScene->DestroyAllMeshes();
	//clSceneStaff->clTV3D->pTVScene->DestroyAllActors();   //  A mettre dans les manager associé ptete?
	//clSceneStaff->clTV3D->pTVScene->DestroyAllParticleSystems(); //  A mettre dans les manager associé ptete?
	delete(clSceneStaff);
}

void CLScene::UpdatePhysics(float timestep)
{

	dFloat timestepInSecunds = 1.0f / MAX_PHYSICS_FPS;
	unsigned64 timestepMicrosecunds = unsigned64(timestepInSecunds * 1000000.0f);

	unsigned64 currentTime = dGetTimeInMicroseconds();
	unsigned64 nextTime = currentTime - m_microsecunds;
	int loops = 0;

	while ((nextTime >= timestepMicrosecunds) && (loops < MAX_PHYSICS_LOOPS)) {
		loops++;

		// run the newton update function
		if (!m_reEntrantUpdate) {
			m_reEntrantUpdate = true;
			if (m_physicsUpdate) {
				// update the physics world
				NewtonUpdate(World, timestepInSecunds);
			}
			m_reEntrantUpdate = false;
		}

		nextTime -= timestepMicrosecunds;
		m_microsecunds += timestepMicrosecunds;
	}

	if (loops) {
		m_physicsTime = dFloat(dGetTimeInMicroseconds() - currentTime) / 1000000.0f;

		if (m_physicsTime >= MAX_PHYSICS_LOOPS * (1.0f / MAX_PHYSICS_FPS)) {
			m_microsecunds = currentTime;
		}
	}
}