// GfxNet.h
#pragma once
//#include "..\GraphysX\IGraphysX.h"
#include "EntityNET.h"   // TODO  :   ark....  sont dépendant des inclues avant pour les enum par exemple..

#include "..\GraphysX\Itv.h"
#include "ToolsNET.h"

#include "SceneNET.h"

/*namespace GfxNet {

	public ref class GfxTools
	{
	public:
		GraphysX::IGraphysX* m_nativeClass = nullptr;
		GfxTools();
		GfxTools(System::String^ AppPath, int width, int height);
		~GfxTools();

	protected:
		!GfxTools();

	public:
		System::String^ test();
		//void window();
		float time();
		void init(int width, int height);
		void renderScene();
		void Update();
		void MouseHighlight();
		bool AppStillIdle();
		float getSceneTime();
		//IntPtr getW();

		//void createEntity(EntityNET obj);

	private:
		GraphysX::CLTV3D* m_TV3D;
		GraphysX::CLScene* m_ActiveScene;
	};

}*/

namespace GraphysX
{
	public ref class Tv 
	{
		Itv* n_Itv = nullptr;
	public:
		Tv(IntPtr hwnd);
		~Tv();
	protected:
		!Tv();
	public:
		void render();
		void update();
		bool AppStillIdle();

		//void Serialize(String^ Path, Object^ Scene3D);
		//Scene3D^ Deserialized(std::string Path);
		//void ClearScene3D(GfxNet::SceneNET^ scene3D);
		void ClearActiveScene();
		void BuildScene(GfxNet::SceneNET^ Scene2Build);
		Vec2D^ BuildASCIIScene(System::String^ asciiFilePath);  // return the loaded map size
		void addFloor(int mapSize, System::String^ floorTextureName);
		void createSmoke();
		//void CreatePiston(float x, float z, float AngleY);
		void CreateVoieLactee(void);
		void CreateChain(Vec3D Pos, float fScale, float angle, float masse, System::String^ texName);
		void addPhysMesh(GfxNet::EntityNET^ param);

	
		static STSceneParam sceneNETtoNative(GfxNet::SceneNET^ scene);
		static STEntityParam entityNETtoNative(GfxNet::EntityNET^ entity);
		static GfxNet::SceneNET^ nativeToSceneNET(STSceneParam scene);
		static GfxNet::EntityNET^ nativeToEntityNET(STEntityParam entity);
	private:

	};
}



/*

public ref class GraphysXWrapper {
	GraphysX::IGraphysX* m_nativeClass;

public:
	GraphysXWrapper(System::String^ AppPath, int width, int height) {
		std::string path;
		ToolsNET::MarshalString(AppPath, path);
		m_nativeClass = new GraphysX::IGraphysX(path, width, height);
		m_TV3D = &m_nativeClass->clTV3D;
		m_ActiveScene = m_nativeClass->pActiveScene;
		//m_ActorFactory = m_nativeClass->clActorFactory;
	}
	~GraphysXWrapper()		{ delete m_nativeClass; }
	void renderScene()		{ m_nativeClass->renderActiveScene(); }
	void Update()			{ m_nativeClass->Update(); }
	void MouseHighlight()	{ m_nativeClass->MouseHighlight(); }
	bool AppStillIdle()		{ return(m_nativeClass->AppStillIdle()); }
	float getSceneTime()	{ return(m_nativeClass->getSceneTime()); }
	/*CTVParticleSystem* CreateParticleSystem(const std::string& name) {
	return(m_nativeClass->CreateParticleSystem(name.c_str()));
	}

	GraphysX::CLTV3D* m_TV3D;
	GraphysX::CLScene* m_ActiveScene;
	//CLActorFactory* m_ActorFactory;

protected:
	!GraphysXWrapper() { delete m_nativeClass; }
};

*/


/*

public ref class GraphAppWrapper {
App* m_nativeApp;
public:
GraphAppWrapper()		{ m_nativeApp = new App(); }
~GraphAppWrapper()		{ delete m_nativeApp; }
void onInit()		{ m_nativeApp->onInit(); }
void addScreens()	{ m_nativeApp->addScreens(); }
//void Cleanup()		{ m_nativeApp->Cleanup(); }
void onExit()		{ m_nativeApp->onExit(); }
void update()		{ m_nativeApp->update(); }
void render()		{ m_nativeApp->render(); }
void run()			{ m_nativeApp->run(); }
bool isRunning()		{ return(m_nativeApp->isRunning()); }
IGraphysX* getGraphysX() { return(m_nativeApp->getGraphysX()); }
protected:
!GraphAppWrapper() { delete m_nativeApp; }
};
*/