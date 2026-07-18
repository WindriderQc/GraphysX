#pragma once
#include "GraphysXdef.h"
#include "TV3DMoteur.h"
#include "Scene.h"
#include "Clock.h"
#include "ActorFactory.h"
#include "Displayer.h"
#include "Sound.h"
#include "Tools.h"

#include <SDL\SDL.h>
#include <SDL\SDL_syswm.h>
//#include "..\GraphysX\CLCEGUI.h"

using namespace GraphysX;

extern "C"
{

		class IGraphysX
		{
		public:
			GRAPHYSX_API IGraphysX(std::string PathString, int iWinWidth, int iWinHeight);
			GRAPHYSX_API ~IGraphysX();

			GRAPHYSX_API void Update();
			GRAPHYSX_API void renderActiveScene();
			GRAPHYSX_API void renderDisplayer();
			GRAPHYSX_API float getSceneTime();
			GRAPHYSX_API void MouseHighlight();
			GRAPHYSX_API bool AppStillIdle();
			//GRAPHYSX_API CTVParticleSystem* CreateParticleSystem(const char* name);
			GRAPHYSX_API GraphysX::CLMeshPrimitive* isClickOnMesh(float mouseX, float mouseY);
			



			GRAPHYSX_API float getTimeElapsed();
			GRAPHYSX_API std::string getAppPath();
			GRAPHYSX_API HWND createSDLwindow(int width = 1280, int height = 720);


			
			CLActorFactory* clActorFactory = nullptr;
			GraphysX::CLDisplayer clDisplayer;
			GraphysX::CLSound clSound;
			GraphysX::CLTV3D clTV3D;
			
			
			GraphysX::CLScene* pActiveScene = nullptr;
			
		//	GRAPHYSX_API static CTVMesh*  getNewtonCollisionMesh(const NewtonJoint* joint);  //  devrait avoir plus de method static dans IGraphysX TODO

		
		
			
		private:
			static std::string m_AppPath;     // TODO  PKOI STATIC???
			static float fTimeElapsed;        //  PKOI STATIC???
			GraphysX::CLScene* defaultScene;
	};			
}
