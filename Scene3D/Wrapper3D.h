#pragma once
#include <GraphysX\GraphysXdef.h>
#include <GraphysX\IGraphysX.h>
#include "ToolsNET.h"
#include "App.h"

namespace GraphysXWrap
{
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
		GraphysX::IGraphysX* getGraphysX() { return(m_nativeApp->getGraphysX()); }
	protected:
		!GraphAppWrapper() { delete m_nativeApp; }
	};


	public ref class GraphysXWrapper {
		GraphysX::IGraphysX* m_nativeClass;

	public:
		GraphysXWrapper(System::String^ AppPath, int width, int height) {
			std::string path;
			ToolsNET::MarshalString(AppPath, path);
			m_nativeClass = new GraphysX::IGraphysX(path, width, height);
			//m_TV3D = &m_nativeClass->clTV3D;
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
		}*/

		//GraphysX::CLTV3D* m_TV3D;
		GraphysX::CLScene* m_ActiveScene;
		//CLActorFactory* m_ActorFactory;

	protected:
		!GraphysXWrapper() { delete m_nativeClass; }
	};
}