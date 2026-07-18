#pragma once
#include "Entity3D.h"
#include <map>
#include "TV3D65\tvAll.h"


namespace GraphysX
{ 	
	class CLMeshPrimitive : public CLEntity3D
	{
	public:
		CLMeshPrimitive(STMeshParam geomParam);
		virtual ~CLMeshPrimitive();

		STMeshParam stParam;
		CTVMesh* mesh = nullptr;
		
		virtual void update();
		virtual void render();
		
		static int getNbrGeometry();
	//	static std::map<CTVMesh*, CLMeshPrimitive*> mapMeshPrim;
		virtual CTVMesh* loadMesh();
		GRAPHYSX_API void setFollowSpline(Spline spline, bool looping);
		
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
		CLMesh3DText(char* text, char* fontname, float fontsize, STMeshParam stParam, float extrude = 0.2f);	
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
		CLMeshX(std::string filepath, STMeshParam stParam);
		~CLMeshX(){};

		virtual CTVMesh* loadMesh() override;
	protected:	
		std::string filePath;
	};

}
	