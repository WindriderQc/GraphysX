#include "stdafx.h"
#include "MeshPrimitive.h"
#include <TV3D65\CTVMesh.h>
#include <TV3D65\CTVPath.h>
#include <TV3D65\CTVScene.h>


namespace GraphysX
{
	int CLMeshPrimitive::nbrMesh = 0;
	//std::map<CTVMesh*, CLMeshPrimitive*> CLMeshPrimitive::mapMeshPrim; 

	CLMeshPrimitive::CLMeshPrimitive(STEntityParam geomParam) : stParam(geomParam)
	{
		loadMesh();
		nbrMesh++;
	}
	CLMeshPrimitive::~CLMeshPrimitive()
	{
		//mapMeshPrim.erase(mesh); 
		mesh->Destroy();
		mesh = nullptr;
		nbrMesh--;
	}
	int CLMeshPrimitive::getNbrGeometry()
	{
		return nbrMesh;
	}
	void CLMeshPrimitive::setMeshUponParam()
	{
		mesh->Enable(stParam.enable);
		mesh->SetPosition(stParam.pos.x, stParam.pos.y, stParam.pos.z);
		mesh->SetRotation(stParam.rot.x, stParam.rot.y, stParam.rot.z);   /// TODO: PKOI rotation change la value a 0.999  au lieu de 1 ??  crash physics	
		//mesh->SetScale(stParam.scale.x, stParam.scale.y, stParam.scale.z);  // TODO :  Newton n'accepte pas les scale matrix...  la matrix doit etre prise avant que le mesh soit scaler.!!!
		if (stParam.color != -1)
			mesh->SetColor(stParam.color);

		/*
		cTV_3DMATRIX matrix;
		mesh->Enable(stParam.enable);
		mesh->SetPosition(stParam.pos.x, stParam.pos.y, stParam.pos.z);
		matrix = mesh->GetMatrix();
		mesh->SetRotation(stParam.rot.x, stParam.rot.y, stParam.rot.z);   /// TODO: PKOI rotation change la value a 0.999  au lieu de 1 ??  crash physics
		matrix = mesh->GetMatrix();
		//mesh->SetScale(stParam.scale.x, stParam.scale.y, stParam.scale.z);
		
		dMatrix mat = &matrix.m11;

		//mesh->SetMatrix(&param.Matrix);
		*/
	}
	CTVMesh* CLMeshPrimitive::loadMesh()
	{
		int res = -1;
		CTVScene pTVScene;
		mesh = pTVScene.CreateMeshBuilder(stParam.name.c_str());
		
		switch (stParam.geom)
		{
			case eGeometry::CUBE:
				res = mesh->CreateBox(stParam.scale.x, stParam.scale.y, stParam.scale.z);
				break;
			case eGeometry::SPHERE:
				res = mesh->CreateSphere(stParam.scale.x, 36, 36);
				break;
			case eGeometry::CYLINDER:
				res = mesh->CreateCylinder(stParam.scale.x, stParam.scale.y, 24, true);
				break;
			case eGeometry::CONE:
				res = mesh->CreateCone(stParam.scale.x, stParam.scale.y, 24, true);
				break;
		// SPECIAL CASE //
			case eGeometry::TEAPOT:
				mesh->CreateTeapot();
				mesh->SetScale(stParam.scale.x, stParam.scale.y, stParam.scale.z);
				res = 0;
				break;
			case eGeometry::PLANE:
				res = mesh->AddFloor(NULL, -(stParam.scale.x) + stParam.pos.x, -(stParam.scale.z) + stParam.pos.z, (stParam.scale.x) + stParam.pos.x, (stParam.scale.z) + stParam.pos.z, stParam.pos.y, 16, 16);
				if (res==0) return(mesh);	else 	res = -1;
				break;

			case eGeometry::CUSTOM_MESH:
				res = mesh->LoadXFile(stParam.name.c_str(), true, true);
				if (!res)
					res = mesh->LoadTVM(stParam.name.c_str(), true, true);
				break;
			case eGeometry::NON_PRIMITIVE:
				mesh->SetMeshName(stParam.name.c_str());  // TODO:   ca sert a koi??
				return nullptr;
				break;
		}
		if (res == -1)	{
			MSGBOX("loadmesh error", "MeshPrimitive class"); 
			return nullptr;
		}
		else {
			stParam.setTVMesh(mesh);
			setMeshUponParam();
			mesh->SetLightingMode(cCONST_TV_LIGHTINGMODE::cTV_LIGHTING_MANAGED);
			return(mesh);
		}
	}

	void CLMeshPrimitive::render() {
		mesh->Render();
	}
	void CLMeshPrimitive::update() {

		//mesh->Enable(stParam.enable);
		//mesh->SetPosition(stParam.pos.x, stParam.pos.y, stParam.pos.z);
		//mesh->SetRotation(stParam.rot.x, stParam.rot.y, stParam.rot.z);
		//mesh->SetScale(stParam.scale.x, stParam.scale.y, stParam.scale.z);
		//mesh->SetMatrix(&param.Matrix);
	}

	void CLMeshPrimitive::setFollowSpline(Spline spline, bool looping) {

		path.SetPathType(cTV_PATH_SPLINE);
		path.EnableLooping(looping);

		for each(cTV_3DVECTOR vec in spline)
			path.AddPathNode(&vec);

		mesh->SetPath(&path);
		mesh->SetPathSpeed(stParam.speed, true); // 0  // SetPathSpeed(20 * tv.AccurateTimeElapsed(), true);
	}




	CLMesh3DText::CLMesh3DText(char* text, char* fontname, float fontsize, STEntityParam stParam, float extrude) 
		: CLMeshPrimitive(stParam),	mText(text), mFontname(fontname), mFontsize(fontsize), mExtrude(extrude)
	{}
	CTVMesh* CLMesh3DText::loadMesh()
	{
		//mesh->Destroy();  //kills the mesh created in the base class
		//mesh = GraphysX::CLTV3D::pTVScene.CreateMeshBuilder(name.c_str());
		mesh->Create3DText(mText, mFontname, mFontsize, mExtrude);
		setMeshUponParam();
		//if (iTexture)	Mesh->SetTexture(iTexture, -1)
		mesh->SetLightingMode(cCONST_TV_LIGHTINGMODE::cTV_LIGHTING_MANAGED);
	//	mapMeshPrim.insert(std::make_pair(mesh, this));
		stParam.setTVMesh(mesh);
		return(mesh);
	}

	CLMeshX::CLMeshX(std::string filepath, STEntityParam stParam) : CLMeshPrimitive(stParam), filePath(filepath)
	{}
	CTVMesh* CLMeshX::loadMesh()
	{
		//mesh->Destroy();
		//mesh = GraphysX::CLTV3D::pTVScene.CreateMeshBuilder(name.c_str());
		mesh->LoadXFile(filePath.c_str(), true, true);
		mesh->SetMeshName(filePath.c_str());
		setMeshUponParam();
		//if (iTexture)	Mesh->SetTexture(iTexture, -1)
		mesh->SetLightingMode(cCONST_TV_LIGHTINGMODE::cTV_LIGHTING_MANAGED);
		stParam.setTVMesh(mesh);
		return(mesh);
	}
}