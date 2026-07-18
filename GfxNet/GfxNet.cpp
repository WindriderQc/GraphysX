// This is the main DLL file.
#include "stdafx.h"
#include "GfxNet.h"

#pragma comment(lib, "..\\Deps\\Lib\\GraphysX")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2")
#pragma comment(lib, "..\\Deps\\Lib\\SDL2Main")
/*
#pragma comment(lib, "..\\Deps\\Lib\\TV3D65")

#pragma comment(lib, "..\\Deps\\Lib\\CEGUIBase-0_d")
#pragma comment(lib, "..\\Deps\\Lib\\CEGUIDirect3D9Renderer-0_d")
//Newton Lib
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\newton\\Debug\\newton_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\core\\Debug\\Core_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\dJointLibrary\\Debug\\dJointLibrary_d")
#pragma comment(lib,"..\\Deps\\Lib\\dMath_d")
#pragma comment(lib,"..\\Newton\\VS_2013\\Win32\\physics\\Debug\\physics_d")
#pragma comment(lib,"..\\Deps\\Lib\\pthread_d")
#pragma comment(lib,"..\\Deps\\Lib\\dContainers_d")

*/


/*
namespace GfxNet 
{
	GfxTools::GfxTools()  
	{	
		m_nativeClass = new GraphysX::IGraphysX();
	}
	GfxTools::GfxTools(System::String^ AppPath, int width, int height)
	{
		std::string path;
		ToolsNET::MarshalString(AppPath, path);
		m_nativeClass = new GraphysX::IGraphysX(path, width, height);
		//m_TV3D = &m_nativeClass->clTV3D;
		m_ActiveScene = m_nativeClass->pActiveScene;
	}
	GfxTools::~GfxTools() { this->!GfxTools();		}
	GfxTools::!GfxTools() { delete m_nativeClass;   }
	void GfxTools::init( int width, int height)
	{
		m_nativeClass->init( width, height);
		//m_TV3D = &m_nativeClass->clTV3D;
		m_ActiveScene = m_nativeClass->pActiveScene;
	}
	
	//void GfxTools::window() { m_nativeClass->createSDLwindow(); }
	System::String^ GfxTools::test() { 	return ("GfxNet ok");   }
	float GfxTools::time() {
		std::cout << "test time" << std::endl;
		return(13); // m_nativeClass->clTV3D.getTimeElapsed()); 
	}
	void GfxTools::renderScene()		
	{ 
		m_nativeClass->renderActiveScene();
	}
	void GfxTools::Update()			{ m_nativeClass->Update(); }
	void GfxTools::MouseHighlight()	{ m_nativeClass->MouseHighlight(); }
	bool GfxTools::AppStillIdle()		{ return(m_nativeClass->AppStillIdle()); }
	float GfxTools::getSceneTime()	{ return(m_nativeClass->getSceneTime()); }
	//IntPtr GfxTools::getW() { return(IntPtr(m_nativeClass->w)); }
}
*/
namespace GraphysX
{
	Tv::Tv(IntPtr hwnd)
	{ 
		n_Itv = new Itv((void*)hwnd); 
	}
	Tv::~Tv() { this->!Tv(); }
	Tv::!Tv() { delete n_Itv; }
	void Tv::render() { n_Itv->render(); }
	void Tv::update() { n_Itv->update(); }
	bool Tv::AppStillIdle()		{ return(n_Itv->AppStillIdle()); }


	//void Tv::ClearScene3D(GfxNet::SceneNET^ scene3D)   {  n_Itv->ClearScene3D(sceneNETtoNative(scene3D)); 	}
	void Tv::ClearActiveScene()   { n_Itv->ClearActiveScene(); }
	void Tv::BuildScene(GfxNet::SceneNET^ Scene2Build) {  n_Itv->BuildScene(sceneNETtoNative(Scene2Build)); }
	Vec2D^ Tv::BuildASCIIScene(System::String^ asciiFilePath)
	{
		std::string path;
		ToolsNET::MarshalString(asciiFilePath, path);
		cTV_2DVECTOR v = n_Itv->BuildASCIIScene(path);
		Vec2D vec(v.x, v.y);
		return(%vec);

	}
	void Tv::addFloor(int mapSize, System::String^ floorTextureName)
	{
		std::string tex;
		ToolsNET::MarshalString(floorTextureName, tex);
		n_Itv->addFloor(mapSize, tex);
	}
	void Tv::createSmoke()
	{
		n_Itv->createSmoke();
	}
	//void CreatePiston(float x, float z, float AngleY);
	void Tv::CreateVoieLactee(void)
	{
		n_Itv->CreateVoieLactee();
	}
	void Tv::CreateChain(Vec3D Pos, float fScale, float angle, float masse, System::String^ texName)
	{
		std::string tex;
		ToolsNET::MarshalString(texName, tex);
		n_Itv->CreateChain(cTV_3DVECTOR(Pos.x, Pos.y, Pos.z), fScale, angle, masse, tex);
	}

	void Tv::addPhysMesh(GfxNet::EntityNET^ param)
	{
		n_Itv->addPhysMesh(entityNETtoNative(param));
	}



	STSceneParam Tv::sceneNETtoNative(GfxNet::SceneNET^ scene3D)
	{
		STSceneParam st;
		//ToolsNET::MarshalString(scene3D->ActionsHeader, st.ActionsHeader);
		
		st.updateHeader();  // TODO  :  check si on doit prendre celui de NET ou si ok comme ca.
		ToolsNET::MarshalString(scene3D->filepathASCII, st.filepathASCII);
		st.mapSize.x = scene3D->mapSize->x; 
		st.mapSize.y = scene3D->mapSize->y;

		for each(GfxNet::EntityNET^ e in scene3D->EntityNETList)
		{	
			st.Entity3DList.push_back(entityNETtoNative(e));
		}
		st.fS = cTV_3DVECTOR(scene3D->fS->x, scene3D->fS->y, scene3D->fS->z);
		st.fF = cTV_3DVECTOR(scene3D->fF->x, scene3D->fF->y, scene3D->fF->z);
		st.hS = cTV_3DVECTOR(scene3D->hS->x, scene3D->hS->y, scene3D->hS->z);
		st.hF = cTV_3DVECTOR(scene3D->hF->x, scene3D->hF->y, scene3D->hF->z);
	

		st.playerPos = cTV_3DVECTOR(scene3D->playerPos->x, scene3D->playerPos->y, scene3D->playerPos->z);
		

		for each(Vec3D^ v in scene3D->ringPosList)
		{
			st.ringPosList.push_back(cTV_3DVECTOR(v->x, v->y, v->z));
		}
		//void setPhysicWorld(NewtonWorld* physWorld) { m_world = physWorld; }
		//void setPlayerPos(float fx, float fy, float fz);
		return(st);
	}
	STEntityParam Tv::entityNETtoNative(GfxNet::EntityNET^ e)
	{
		STEntityParam p;
		p.type = (eObjet)e->Type;
		p.geom = (eGeometry)e->Geom;
		ToolsNET::MarshalString(e->Name, p.name);
		p.pos = cTV_3DVECTOR(e->Pos->x, e->Pos->y, e->Pos->z);
		p.rot = cTV_3DVECTOR(e->Rot->x, e->Rot->y, e->Rot->z);
		p.scale = cTV_3DVECTOR(e->Scale->x, e->Scale->y, e->Scale->z);
		p.enable = e->bEnable;
		p.masse = e->Masse;
		p.MeshControlled = e->MeshControlled;
		p.physMat = (ePHYSMAT)e->physMat;
		ToolsNET::MarshalString(e->PathToMesh, p.pathToMesh);
		ToolsNET::MarshalString(e->TextureName, p.texName);
		return p;
	}

	GfxNet::SceneNET^ Tv::nativeToSceneNET(STSceneParam scene)
	{
		GfxNet::SceneNET^ s = gcnew GfxNet::SceneNET();  //  TODO  : use smart pointer??   memory leak ici je crois
		s->updateHeader();
		s->filepathASCII = ToolsNET::stdStringToSystemString(scene.filepathASCII);
		s->mapSize->x = scene.mapSize.x;
		s->mapSize->y = scene.mapSize.y; 

		for each(STEntityParam st in scene.Entity3DList)
		{
			s->EntityNETList.Add(nativeToEntityNET(st));
		}
		
		
		s->fS = %Vec3D(scene.fS.x, scene.fS.y, scene.fS.z);;
		s->fF = %Vec3D(scene.fF.x, scene.fF.y, scene.fF.z);
		s->hS = %Vec3D(scene.hS.x, scene.hS.y, scene.hS.z);
		s->hF = %Vec3D(scene.hF.x, scene.hF.y, scene.hF.z);
		s->playerPos = %Vec3D(scene.playerPos.x, scene.playerPos.y, scene.playerPos.z);
		for each(cTV_3DVECTOR v in scene.ringPosList)
		{
			s->ringPosList.Add(%Vec3D(v.x, v.y, v.z));
		}
		//void setPhysicWorld(NewtonWorld* physWorld) { m_world = physWorld; }
		//void setPlayerPos(float fx, float fy, float fz);
		return(s);

	}
	GfxNet::EntityNET^ Tv::nativeToEntityNET(STEntityParam entity)
	{
		GfxNet::EntityNET^ e = gcnew GfxNet::EntityNET();//  TODO  : use smart pointer??   memory leak ici je crois
		
		e->Type = (GfxNet::EntityNET::enumType)entity.type;
		e->Geom = (GfxNet::EntityNET::enumGeom)entity.geom;
		e->Name = ToolsNET::stdStringToSystemString(entity.name);
		e->Pos = %Vec3D(entity.pos.x, entity.pos.y, entity.pos.z) ;
		e->Rot = %Vec3D(entity.rot.x, entity.rot.y, entity.rot.z);
		e->Scale = %Vec3D(entity.scale.x, entity.scale.y, entity.scale.z);
		e->bEnable = entity.enable;
		e->Masse = entity.masse;
		e->MeshControlled = entity.MeshControlled;
		e->physMat = (GfxNet::EntityNET::enumPHYSMAT)entity.physMat;
		e->PathToMesh = ToolsNET::stdStringToSystemString(entity.pathToMesh);
		e->TextureName = ToolsNET::stdStringToSystemString(entity.texName);

		return (e);
		
	}

	//void Serialize(String^ Path, Object^ Scene3D);
	//Scene3D^ Deserialized(std::string Path);
}
	

/*

#include <vcclr.h>

namespace GFX_NET
{
	GraphysX_NET::GraphysX_NET(System::String^ AppPath, int width, int height)
	{
		std::string path;
		MarshalString(AppPath, path);
		m_nativeClass = new GraphysX::IGraphysX(path, width, height);
		m_TV3D = &m_nativeClass->clTV3D;
		m_ActiveScene = m_nativeClass->pActiveScene;
		//m_ActorFactory = m_nativeClass->clActorFactory;
	}


	void GraphysX_NET::MarshalString(System::String ^ s, std::string& os) {
		using namespace System::Runtime::InteropServices;
		const char* chars =
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	}

}

*/