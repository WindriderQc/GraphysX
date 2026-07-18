#pragma once
#include "GraphysXdef.h"
#include "STEntityParam.h"

//class NewtonWorld;

namespace GraphysX
{

	struct GRAPHYSX_API STSceneParam      //   Doit etre exporter car STSceneParam est utilisé comme return type dans GfxNet...  TODO  : check solution
	{
	public:
		STSceneParam();
		void updateHeader();
	//	void setPhysicWorld(NewtonWorld* physWorld);
		void setPlayerPos(float fx, float fy, float fz);

		std::string ActionsHeader;
		std::string filepathASCII = "";
		cTV_2DVECTOR mapSize = { 0, 0 };
		std::vector<STEntityParam> Entity3DList;
		cTV_3DVECTOR fS;
		cTV_3DVECTOR fF;
		cTV_3DVECTOR hS;
		cTV_3DVECTOR hF;
		cTV_3DVECTOR playerPos;
		std::vector<cTV_3DVECTOR> ringPosList;
	private:
//		NewtonWorld* m_world = nullptr;
	};

}