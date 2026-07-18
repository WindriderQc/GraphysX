#pragma once
#include "EntityNET.h"
#include "Vec3D.h"
#include "ToolsNET.h"

namespace GfxNet
{
	public ref class SceneNET
	{
	public:
		SceneNET();
		
		System::String^ ActionsHeader = "";
		System::String^  filepathASCII = "";
		Vec2D^ mapSize;
		System::Collections::Generic::List<EntityNET^> EntityNETList;
		Vec3D^ fS;
		Vec3D^ fF;
		Vec3D^ hS;
		Vec3D^ hF;

		Vec3D^ playerPos;  
		System::Collections::Generic::List<Vec3D^> ringPosList;
		//void setPhysicWorld(NewtonWorld* physWorld) { m_world = physWorld; }
		void setPlayerPos(float fx, float fy, float fz) { playerPos->x = fx, playerPos->y = fy, playerPos->z = fz; };
              
		static void Serialize(String^ Path, Object^ Scene);
		static SceneNET^ Deserialized(std::string Path);
	
		void updateHeader();
		private:
	};
}