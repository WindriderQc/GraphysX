#include "StdAfx.h"
#include "Scene.h"
#include "Entity3D.h"
#include "GraphysXdef.h"

#include "ResourceManager.h"
#include "Clock.h"
#include "MeshBillboard.h"  // TODO: check pkoi l;es autre header sont dans le .h...  devrait etre ici
#include "Spline3D.h"
#include "Tools.h"
#include "HiResTimer.h"
#include "TV3DMoteur.h"

#include "ParticleEffect.h"
#include "ZoneTrigger.h"





#define LAND_CHUNKS 8
#define TIMESTEP_IN_SECONDS 1 / MAX_PHYSICS_FPS


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
		CLTV3D::pTVScene.SetShadowParameters(RGBA(0.0, 0.0, 0.0, 0.5), true);  // TODO :   tester l'effet... recup de vieux code
		
		clSky.setCurrentSky(&clSky.loadSkybox("LostValley", true));
		//clSky.setDefaultSky();
		//clSky.setDayNightEnable(false);
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
		
		cycleTexFire = loadFireCycleTex(); // TODO : assurement devrait pas etre ici....   

		//Physics
		World = NewtonCreate();
		loadPhysmat();
	
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

	void CLScene::createDefaultLighting()
	{
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

		clSky.UpdateRenderAtmos();
		
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
	void CLScene::Update()
	{
		float fTimeElapsed = CLTV3D::getTimeElapsed();
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
		static unsigned64 m_microsecunds = 0;
		static bool bFirst = true;
		if (bFirst)
		{		
			Tools::resetTimer();
			m_microsecunds = Tools::getTimeInMicroseconds();
			bFirst = false;
		}
		unsigned64 timestepMicrosecunds = unsigned64(TIMESTEP_IN_SECONDS * 1000000);
		unsigned64 currentTime = Tools::getTimeInMicroseconds();
		unsigned64 nextTime = currentTime - m_microsecunds;
		bool loop = false;
		while (nextTime >= timestepMicrosecunds)
		{
			loop = true;
			NewtonUpdate(World, TIMESTEP_IN_SECONDS);
			nextTime -= timestepMicrosecunds;
			m_microsecunds += timestepMicrosecunds;
		}
		if (loop) {
			unsigned64 m_physicsTime = (Tools::getTimeInMicroseconds() - currentTime) / 1000000;
			if (m_physicsTime >= TIMESTEP_IN_SECONDS) {
				m_microsecunds = currentTime;
			}
		}
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

		stSceneParam.Entity3DList.clear();

		Physics::ClearTriggers();
		ParticleEffects::ClearParticleSystems();

		std::cout << "All entities deleted" << std::endl;
	}
	void CLScene::createFire(cTV_3DVECTOR pos)  //  TODO: create Billboard function et mettre create fire dans helper func
	{
		STEntityParam param(eObjet::BILLBOARD, eGeometry::NON_PRIMITIVE, "fire", pos, { 2, 2, 1 });
		CLMeshBillboard* b = new CLMeshBillboard(param, cycleTexFire);
		b->loadMesh();
		vEntity.push_back(b);
	}
	CLSpline3D*  CLScene::add3DSpline(Spline spline, bool isVisible)
	{
		CLSpline3D* spline3D = new CLSpline3D(this, spline);
		return(spline3D);
	}
	CLDynamicMesh* CLScene::addDynamicMesh( STEntityParam& stGeom)
	{
		vDynPrim.push_back(new CLDynamicMesh(World, stGeom));
		vEntity.push_back(vDynPrim.back());
		if (stGeom.texName != "")
			vDynPrim.back()->mesh->SetTexture(ResourceManager::getTexture(stGeom.texName));
		return(vDynPrim.back());
	}
	PushPlate* CLScene::addPushplate(STEntityParam& stGeom, cTV_3DVECTOR forcepush, cTV_3DVECTOR forcepull)
	{
		vEntity.push_back(new PushPlate(World, stGeom, &forcepush.x, &forcepull.x));
		if (stGeom.texName != "")
			((PushPlate*)vEntity.back())->mesh->SetTexture(ResourceManager::getTexture(stGeom.texName));
		return((PushPlate*)vEntity.back());
	}
	CLMesh3DText* CLScene::addMesh3DText(char* text, char* fontname, float fontsize, STEntityParam& stParam, float extrude)
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

		STEntityParam param(eObjet::PHYSPRIMITIVE, eGeometry::SPHERE, "Bullet", pos, { diameter, diameter, diameter });
		param.texName = texName;
		param.physMat = physMat;
		param.masse = masse;
		nDir = math.VNormalize(&dir);
		math.TVVec3Scale(&tmp, &nDir, offset);
		param.pos = param.pos + tmp;  // pour éviter que la balle soit creer dans le player et l'affecte de sa physique
		param.speed = speed;
		Bullet* bullet = new Bullet(World, param, nDir, lifetime);
		if (texName != "")	bullet->mesh->SetTexture(ResourceManager::getTexture(texName));
		vDynPrim.push_back(bullet);
		vEntity.push_back(bullet);
		std::cout << "Bullet created: " << bullet->getNbrBullets() << std::endl;
		return(bullet);
	}
	CLKinematicPrimitive* CLScene::addKinematicPrimitive(STEntityParam& stGeom)
	{
		vKinPrim.push_back(new CLKinematicPrimitive(World, stGeom));
		vEntity.push_back(vKinPrim.back());
		if (stGeom.texName != "")  
			vKinPrim.back()->mesh->SetTexture(ResourceManager::getTexture(stGeom.texName));
		return(vKinPrim.back());
	}
	CLMeshPrimitive* CLScene::addMeshPrimitive(STEntityParam& stGeom)
	{
		//vEntity.push_back(std::make_unique<CLMeshPrimitive>(stGeom));
		CLMeshPrimitive* prim = new CLMeshPrimitive(stGeom);
		
		vEntity.push_back(prim);
		mapMeshPrim.insert(std::make_pair(prim->mesh, prim));
		if (stGeom.texName != "")
			prim->mesh->SetTexture(ResourceManager::getTexture(stGeom.texName));
		return(prim);
	}
	Human* CLScene::addSphereHuman(STEntityParam& stGeom, int cycleTex)
	{
		//vDynPrim.push_back();
		vEntity.push_back(new Human(World, stGeom, cycleTex));
		return((Human*)vEntity.back());
	}
	CLMeshBillboard* CLScene::addMeshBillboard(STEntityParam& stGeom)
	{
		CLMeshBillboard* billboard = new CLMeshBillboard(stGeom, ResourceManager::getTexture(stGeom.texName));
		vEntity.push_back(billboard);
		billboard->loadMesh();
		return(billboard);
	}
	
	CLActor* CLScene::createCubX(STEntityParam& stGeom)
	{
		vActors.push_back(new CLActor(stGeom, NULL)); //CubX Rotating as test model
		vEntity.push_back(vActors.back());
		vActors.back()->tvActor->SetTexture(NULL, -1);
		return (vActors.back());
	}
	// Creation helper function
	CLMeshPrimitive* CLScene::addPrimSphere(std::string name, cTV_3DVECTOR pos, float radius, std::string textureName)// TODO: ajouter la possibilité de setter le nombre de slice.  car good pour planet mais les bullet sont trop high res
	{
		STEntityParam stParam(eObjet::PRIMITIVE, eGeometry::SPHERE, name, pos, Vector3(radius, radius, radius));
		stParam.texName = textureName;
		return(addMeshPrimitive( stParam));
	}  
	CLMeshPrimitive* CLScene::addPrimCube(std::string name, cTV_3DVECTOR pos, cTV_3DVECTOR scale, std::string textureName)
	{
		STEntityParam stParam(eObjet::PRIMITIVE, eGeometry::CUBE,name,  pos, scale);
		stParam.texName = textureName;
		return(addMeshPrimitive(stParam));
	}
	CLMeshPrimitive* CLScene::addPrimCylinder(std::string name, cTV_3DVECTOR pos, float radius, float height, std::string textureName)
	{
		STEntityParam stParam(eObjet::PRIMITIVE,  eGeometry::CYLINDER,name, pos, Vector3(radius, height, radius));
		stParam.texName = textureName;
		return(addMeshPrimitive(stParam));
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
		CL6DOFJoint* cl6DOFJoint = new CL6DOFJoint(ParentBody, ChildBody, LimitsMinAngle, LimitsMaxAngle);  // TODO:  new...  never deleted?  avoirt un vector de Joint comme les primitive? joint = entity?
		return(cl6DOFJoint);
	}
	CLSlidingJoint* CLScene::CreateSlidingJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit)
	{
		CLSlidingJoint* clSlidingJoint = new CLSlidingJoint(ParentBody, ChildBody, limitsMinDist, limitsMaxDist, MinAngularLimit, MaxAngularLimit);
		return(clSlidingJoint);
	}



	/////////////////////////////////////////

	//   BUILDER

	///////////////////////////////////////

	void CLScene::loadPhysmat() {

		ResourceManager::getPhysmat(BALL, World);  // TODO:  pas super clean...   l'utilisation et creation de nouveau MAT devrait etre plus OOP...
		ResourceManager::getPhysmat(HUMAN, World);
		ResourceManager::getPhysmat(FINISH, World);
		ResourceManager::getPhysmat(GROUND, World);
		ResourceManager::getPhysmat(WALL, World);

		setPhysmatElasticity(WALL, BALL, 0.3f);
		setPhysmatFriction(WALL, BALL, 1.1f, 0.8f);

		std::cout << "  - Physics Materials loaded - Qty : " << sizeof(ResourceManager::nbrMaterial()) << std::endl;
	}

	void CLScene::BuildScene(STSceneParam Scene2Build)
	{
	//	Scene2Build.setPhysicWorld(World);

		int nbrObj = 0;
		CLMeshPrimitive* newMesh;
		CLActor* newActor;
		for each(STEntityParam param in Scene2Build.Entity3DList)
		{
			newMesh = nullptr;
			newActor = nullptr;
			size_t nbr = sizeof(ePhysmat_names) / sizeof(ePhysmat_names[0]);   // Attribue le default physmat si la valeur dans le XML est hors de l'enum.
			if ((param.physMat < 0) || (param.physMat >= (int)nbr))  param.physMat = DEF_PHYSMAT;

			switch (param.type)
			{
			case eObjet::PRIMITIVE:
			{
				newMesh = addMeshPrimitive(param); // xmlToParam(eGeometry::CUBE, Obj));
			}
			case eObjet::PHYSPRIMITIVE:
			{
				newMesh = addDynamicMesh(param);
			}
			case eObjet::BILLBOARD:
			{
				newMesh = addMeshBillboard(param);
				break;
			}
			case eObjet::PHYSICCUSTOM:
			{
				newMesh = addDynamicMesh(param);
				break;
			}
			default:
				std::cout << "SceneBuilder - " << param.type << "Object Type non reconnu - Must add to BuildScene()" << std::endl;
				/*System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + Obj->Type, L"Object Type non reconnu - Must add to BuildScene()",
				System::Windows::Forms::MessageBoxButtons::OK);*/
				break;
			}


			if (newActor) {
				newActor->entityParam.setTVActor(newActor->tvActor);  // TODO : useless, newActor is never used
			}
			nbrObj++;
			std::cout << " Object #" << nbrObj << " loaded." << std::endl;
		}
	}
	cTV_2DVECTOR CLScene::BuildASCIIScene(std::string asciiFilePath)
	{
		Tools::ASCIIFile asciiFile;
		vector<string> _pLevelData = asciiFile.load(asciiFilePath);
		asciiFile.print();

		float Line = 0;
		float Col = 0;
		int offX = 0; // changer pour etre calculé sur moitié de longueur d'une ligne et moitié Nbr ligne
		int offY = 0;
		dFloat angle = 60.0f * 3.1415592f / 180.0f;

		std::vector<CLEntity3D*> newEntities;

		if (_pLevelData.size() >= 1)
		{
			for each(string line in _pLevelData)
			{
				for each(char tile in line)
				{
					newEntities.clear();
					cTV_3DVECTOR pos(0.5f + Col + offX, 0.5f, 0.5f + Line + offY);

					switch (tile)
					{
					case '#':
					case 'Z':
					case 'z':
					{
						STEntityParam param(eObjet::PHYSPRIMITIVE, eGeometry::CUBE, "Wall " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line), pos);
						param.physMat = ePHYSMAT::WALL;
						switch (tile)
						{
						case '#':
							param.texName = "objet39.jpg";
							break;
						case 'Z':
							param.texName = "Grass.jpg";
							break;
						case 'z':
							param.texName = "Podium.jpg"; //   TODO : Add the normal map to this...
							break;
						}
						newEntities.push_back(addDynamicMesh(param));
						break;
					}

					case '@': {
						stSceneParam.setPlayerPos(Col, 0.5f, Line);
						break;
					}
					case '.': {
						//m_graphysX->clMeshFactory.CreateSphere(pTVTexFac->GetTextureByName("TwoWaySign"), pos, cTV_3DVECTOR(0.5, 0.5, 0.5), "Tile", true);
						break;
					}
					case 'C': {
						STEntityParam param(eObjet::CUSTOM, eGeometry::NON_PRIMITIVE, "CubeX " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line), pos);
						createCubX(param)->tvActor->SetPosition(pos.x, pos.y + 5, pos.z);  
						break;
					}
					case 'c': {
						CreateChain(cTV_3DVECTOR(pos.x, pos.y + 0.25f, pos.z), 0.25f, angle, 0.5f, "3D_Spheres.jpg");
						break;
					}

					case 'F':
					case 'f':
					case 'H':
					case 'h':
					{
						STEntityParam param(eObjet::PHYSPRIMITIVE, eGeometry::CYLINDER, "tower " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line), pos);
						param.scale = { 0.2f, 2, 0.2f };
						param.physMat = ePHYSMAT::WALL;
						param.color = RGBA_BLUE;
						newEntities.push_back(addMeshPrimitive(param));
						//newMesh->SetPPLShader("..\\StockRoom\\ball_Normal.png",m_scene->getDefaultLightPos(), "pplShader", 0.025f);
						switch (tile)
						{
						case 'F':
							newEntities.back()->entityParam.getTVMesh()->SetColor(RGBA_RED, true);
							stSceneParam.fS.x = pos.x;
							stSceneParam.fS.y = pos.y;
							stSceneParam.fS.z = pos.z;
							break;
						case 'f':
							newEntities.back()->entityParam.getTVMesh()->SetColor(RGBA_RED, true);
							stSceneParam.fF.x = pos.x;
							stSceneParam.fF.y = pos.y;
							stSceneParam.fF.z = pos.z;
							break;
						case 'H':
							stSceneParam.hS.x = pos.x;
							stSceneParam.hS.y = pos.y;
							stSceneParam.hS.z = pos.z;
							break;
						case 'h':
							stSceneParam.hF.x = pos.x;
							stSceneParam.hF.y = pos.y;
							stSceneParam.hF.z = pos.z;
							break;
						}
						break;
					}
					case 'R': {
						stSceneParam.ringPosList.push_back(cTV_3DVECTOR(pos.x, pos.y - 0.15f, pos.z));
						break;
					}
					case 's': {
						STEntityParam param(eObjet::PHYSPRIMITIVE, eGeometry::CUBE, "Wall " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line), pos);
						param.physMat = ePHYSMAT::WALL;
						param.texName = "Zack.jpg";
						newEntities.push_back(addDynamicMesh(param));
						int partText = ResourceManager::getTexture("particle.dds");  // TODO: n'est pas supprimé au clear scene...
						ParticleEffects::ParticleEffect Part1(CLTV3D::pTVScene.CreateParticleSystem("Part1"), cTV_EMITTER_BILLBOARD, cTV_EMITTERSHAPE_POINT, &cTV_3DVECTOR(pos.x, pos.y, pos.z), 64, true);
						Part1.Init(partText, 100.0f, 5, 2, &cTV_COLOR(1, 0, 1, 1), true);
						ParticleEffects::ParticleEffect Part2(CLTV3D::pTVScene.CreateParticleSystem("Part2"), cTV_EMITTER_BILLBOARD, cTV_EMITTERSHAPE_POINT, &cTV_3DVECTOR(pos.x, pos.y, pos.z), 64, true);
						Part2.Init(partText, 100.0f, 5, 2, &cTV_COLOR(0, 0.1f, 1, 1), true);
						break;
					}
							  // section piston
					case '0':
					case '3':
					case '6':
					case '9':
					{
						STEntityParam stPar(eObjet::PHYSPRIMITIVE, eGeometry::CUBE, "PistonBar " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line), pos, { 5.0f, 0.1f, 0.1f });
						stPar.color = RGBA_DARKGRAY;
						switch (tile)
						{
						case '0':
							stPar.rot = { 0, 270, 90 };
							break;
						case '3':
							stPar.rot = { 0, 0, 90 };
							break;
						case '6':
							stPar.rot = { 0, 90, 90 };
							break;
						case '9':
							stPar.rot = { 0, 180, 90 };
							break;
						}
						newEntities.push_back(addMeshPrimitive(stPar));
						CTVMathLibrary math;
						cTV_3DVECTOR forcepush = { 50, 0, 0 };
						cTV_3DVECTOR forcepull = { -1, 0, 0 };
						math.TVVec3Rotate(&forcepush, &forcepush, stPar.rot.y, stPar.rot.z, stPar.rot.x);
						math.TVVec3Rotate(&forcepull, &forcepull, stPar.rot.y, stPar.rot.z, stPar.rot.x);

						STEntityParam plateParam(eObjet::PHYSPRIMITIVE, eGeometry::CUBE, "piston " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line), stPar.pos);
						plateParam.scale = { 0.25f, 0.95f, 0.95f };
						plateParam.rot = stPar.rot;
						plateParam.masse = 0.1f;
						PushPlate* testPlate = addPushplate(plateParam, forcepush, forcepull);
						newEntities.push_back(testPlate);
						testPlate->setLimits(-0.5f, 0.5f, 0, 0);

						stPar.name = "trigger " + Tools::floatToStr(Col) + "." + Tools::floatToStr(Line);
						stPar.geom = eGeometry::CUBE;
						stPar.pos = { pos.x, pos.y, pos.z };
						stPar.scale = { 0.75f, 0.95f, 0.75f };
						stPar.enable = false;
						CLMeshPrimitive* trigger = addMeshPrimitive(stPar);
						newEntities.push_back(trigger);
						Physics::ZoneTrigger* pushPlateTrg = new Physics::ZoneTrigger();  // TODO:  un delete de tous les trigger??  le new devrait etre dans le Pushplate...
						pushPlateTrg->Init(trigger, &testPlate->bTrigger);
						break;
					}
					default:
					{
						cout << "Unrecognized Symbol in ASCII file." << endl;
						//System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + tile->Type, L"Object Type non reconnu - Must add to BuildScene()",
						//System::Windows::Forms::MessageBoxButtons::OK);
						break;
					}
					}
					if (newEntities.size())
					{
						for each(CLEntity3D* entity in newEntities)
						{
							stSceneParam.Entity3DList.push_back(entity->entityParam);
						}
					}

					/*//Ajoute un cube par tile comme floor
					STEntityParam stParam("FloorTile", eGeometry::CUBE, Vector3(pos.x, pos.y-1, pos.z), { 1, 1, 1 });
					newMesh =m_scene->addDynamicMesh("twoway.jpg", stParam, ePHYSMAT::WOOD, 0);
					Scene2Build.ObjList.Add(MeshToObj3D(newMesh));*/
					Col++;
				}
				Line++;
				Col = 0;
			}
			if (stSceneParam.mapSize.x < Col) stSceneParam.mapSize.x = Col;  // if new map is larger then scene, update scene
			if (stSceneParam.mapSize.y < Line) stSceneParam.mapSize.y = Line;
			return(cTV_2DVECTOR(Col, Line));  //  return the length of created map 
		}
		else{
			cout << "ASCII Error - File empty" << endl;
			return(cTV_2DVECTOR(0, 0));
		}
	}
	void CLScene::addFloor(int mapSize, std::string floorTextureName)
	{
		// Create Floor
		STEntityParam param(eObjet::PHYSPRIMITIVE, eGeometry::PLANE, "floor"
			, { float(mapSize / 2), -0.05f, float(mapSize / 2) }
			, { float(mapSize / 2), 2.0f/*0.1f*/, float(mapSize / 2) }  //  TODO: revoir l'épaisseur...
		);
		param.physMat = ePHYSMAT::GROUND;
		param.texName = floorTextureName;

		CLDynamicMesh* floor = addDynamicMesh(param);
	}
	void CLScene::createSmoke()  {
		int partText = ResourceManager::getTexture("particle.dds");
		ParticleEffects::ParticleEffect Part1(CLTV3D::pTVScene.CreateParticleSystem("Part1"), cTV_EMITTER_BILLBOARD, cTV_EMITTERSHAPE_POINT, &cTV_3DVECTOR(0, 0, 0), 64, true);
		Part1.Init(partText, 100.0f, 5, 2, &cTV_COLOR(1, 0, 1, 1), true);
		ParticleEffects::ParticleEffect Part2(CLTV3D::pTVScene.CreateParticleSystem("Part2"), cTV_EMITTER_BILLBOARD, cTV_EMITTERSHAPE_POINT, &cTV_3DVECTOR(0, 0, 0), 64, true);
		Part2.Init(partText, 100.0f, 5, 2, &cTV_COLOR(0, 0.1f, 1, 1), true);
	}
	void CLScene::CreateVoieLactee() {
		//EARTH
		addPrimSphere("earth", { -10, 15, 10 }, 6.0f, "Galaxy\\earthgood.jpg")->mesh->RotateZ(23);

		//ResourceManager::getTexture("Galaxy\\EarthCloudsMap.png", cTV_COLORKEY_BLACK);
		CLMeshPrimitive* EarthClouds = addPrimSphere("clouds", { -10, 15, 10 }, 6.1f, "Galaxy\\EarthClouds.jpg");
		EarthClouds->mesh->SetAlphaTest(true, 255);
		EarthClouds->mesh->SetBlendingMode(cTV_BLEND_COLOR); // cTV_BLEND_ALPHA  cTV_BLEND_COLOR

		addPrimSphere("Moon", { -8, 14, 12 }, 2.0f, "Galaxy\\MoonMap.jpg");
		addPrimSphere("Mars", { -40, 15, 10 }, 12.0f, "Galaxy\\MarsMap.jpg");
		addPrimSphere("Venus", { -70, 15, 10 }, 4.0f, "Galaxy\\VenusMap.jpg");
	}
	void CLScene::CreateChain(cTV_3DVECTOR Pos, float fScale, const float angle, float masse, std::string texName)
	{
		CLDynamicMesh* clBase, *cl1, *cl2;
		STEntityParam param(eObjet::PHYSPRIMITIVE, eGeometry::CUBE, "base", Pos, { fScale, fScale, fScale });
		param.color = RGBA_BLACK;
		clBase = addDynamicMesh(param);
		clBase->mesh->SetMaterial(ResourceManager::getMaterial(eMATERIAL::DEF_MATERIAL));//ResourceManager::getTexture("AbstractCubes.jpg")
		clBase->mesh->SetColor(RGBA_RED, true);  // si on met a true, les effets du material reste....

		param.geom = eGeometry::CONE;
		param.name = "chain1";
		param.pos = { Pos.x, Pos.y - 0.25f, Pos.z };
		param.scale = { fScale / 1.75f, fScale, fScale };
		param.texName = texName;
		param.masse = masse;
		cl1 = addDynamicMesh(param);

		param.name = "chain2";
		param.pos = { Pos.x, Pos.y - 0.5f, Pos.z };
		param.scale = { fScale / 1.5f, fScale, fScale };
		cl2 = addDynamicMesh(param);

		Create6DOFJoint(clBase->getBody(), cl1->getBody(), dVector(-angle, -angle, -angle, 0.0f), dVector(angle, angle, angle, 0.0f));
		Create6DOFJoint(cl1->getBody(), cl2->getBody(), dVector(-angle, -angle, -angle, 0.0f), dVector(angle, angle, angle, 0.0f));
	}



	
	
	/////////////////////////////////////////

	//   WATER

	///////////////////////////////////////
	CLWater::CLWater(void)
	{
		CTVScene pTVScene;
		CTVTextureFactory pTVTexFactory;
		CTVGraphicEffect pTVGraphEffect;
		

		// Creates the reflection/refraction render surfaces, which will be used for water objects.
		pReflectRS = pTVScene.CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
		pReflectRS->SetBackgroundColor(RGBA(0, 0, 0.1906f, 1));

		pRefractRS = pTVScene.CreateRenderSurfaceEx(-1, -1, cTV_TEXTUREFORMAT_DEFAULT, true, true, 1);
		pRefractRS->SetBackgroundColor(RGBA(0, 0, 0.1906f, 1));

		int iDUDV = pTVTexFactory.LoadDUDVTexture("Media\\distortiontexture.dds", "DUDV", -1, -1, 150); //distortiontexture.dds

		pWaterMesh = pTVScene.CreateMeshBuilder("WaterMesh");
		WaterHeight = 85;
		pWaterMesh->AddFloor(iDUDV, -2048, -2048, 2048, 2048, WaterHeight, 16, 16, true);
		pWaterMesh->SetBlendingMode(cTV_BLEND_ALPHA, -1);

		pWaterPlane = new cTV_PLANE(cTV_3DVECTOR(0, 1, 0), -WaterHeight - 0.75f);// between .75 and like 1.3 are the best values
		pTVGraphEffect.SetWaterReflection(pWaterMesh, pReflectRS, pRefractRS, 0, pWaterPlane);
		pTVGraphEffect.SetWaterReflectionBumpAnimation(pWaterMesh, true, 0.5, 0.5);  //  les 1 sont la vitesse en x et y

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