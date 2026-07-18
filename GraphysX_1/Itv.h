#pragma once
#include <string>
#include "STEntityParam.h"
#include "STSceneParam.h"

namespace GraphysX
{
	extern "C"
	{
		GRAPHYSX_API HWND createSDLwindow(int width = 1280, int height = 720);

		class CLScene;
		class GRAPHYSX_API Itv
		{
		public:
			Itv(void* handle);
			~Itv();
			void render();
			void update();
			bool AppStillIdle();
			void CreateEntity(STEntityParam obj);
		
			//void Serialize(String^ Path, Object^ Scene3D);
			//Scene3D^ Deserialized(std::string Path);
			void ClearActiveScene();
			void BuildScene(STSceneParam Scene2Build);
			cTV_2DVECTOR BuildASCIIScene(std::string asciiFilePath);  // return the loaded map size
			void addFloor(int mapSize, std::string floorTextureName);
			void createSmoke();
			//void CreatePiston(float x, float z, float AngleY);
			void CreateVoieLactee(void);
			void CreateChain(cTV_3DVECTOR Pos, float fScale, const float angle, float masse, std::string texName);
			void addPhysMesh(STEntityParam param); 


		private:
			static CLScene* activeScene;
		};
	}
}