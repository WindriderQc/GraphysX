#pragma once
#include "Entity3D.h"

class CTVMesh;
class CTVPath;

namespace GraphysX
{ 	
	class CLMeshPrimitive : public CLEntity3D
	{
	public:
		CLMeshPrimitive(STEntityParam geomParam);
		virtual ~CLMeshPrimitive();

		STEntityParam stParam;
		CTVMesh* mesh = nullptr;
		
		virtual void update();
		virtual void render();
		
		static int getNbrGeometry();
	//	static std::map<CTVMesh*, CLMeshPrimitive*> mapMeshPrim;
		virtual CTVMesh* loadMesh();
		void setFollowSpline(Spline spline, bool looping);   //  TODO  : check pour enlever le Graph API....   ici pour juste une fonction??   met sdans interface

		
		CTVPath* getPath(void)  { return(&path); }

	protected:
		void setMeshUponParam();
	private:
		static int nbrMesh;
		CTVPath path;

	};



	class CLMesh3DText : public CLMeshPrimitive
	{
	public:
		CLMesh3DText(char* text, char* fontname, float fontsize, STEntityParam stParam, float extrude = 0.2f);	
		~CLMesh3DText(){};

		virtual CTVMesh* loadMesh() override;

	private:
		char* mText;
		char* mFontname;
		float mFontsize;
		float mExtrude;
	};


	class CLMeshX : public CLMeshPrimitive
	{
	public:
		CLMeshX(std::string filepath, STEntityParam stParam);
		~CLMeshX(){};

		virtual CTVMesh* loadMesh() override;
	protected:	
		std::string filePath;
	};

}
	