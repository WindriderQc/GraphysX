#pragma once
#include <string>
//#include <vector>
#include <TV3D65\tv_types.h>
#include "GfxEnum.h"

class CTVMesh;
class CTVActor;

namespace GraphysX
{
	struct STEntityParam   // TODO : creer des class qui hérite de STENtityParam pour avoir des config d'object pre fait
	{
	public:
		eObjet type = NONE;
		eGeometry geom = NON_PRIMITIVE;
		ePHYSMAT physMat = DEF_PHYSMAT;
		eMATERIAL lightMat = DEF_MATERIAL;

		std::string name;
		cTV_3DVECTOR pos;
		cTV_3DVECTOR rot;
		cTV_3DVECTOR scale;
		bool enable;
		unsigned int color;
		float masse;
		float speed;
		bool MeshControlled;
		std::string pathToMesh;    //  TODO : Path to mesh devient inutile si on utilise param.name comme nom de fichier
		std::string texName;

		STEntityParam(eObjet eType = NONE, eGeometry eGeom = CUBE, std::string sName = "Entity3D"
			, cTV_3DVECTOR vPos = { 0, 0, 0 },  cTV_3DVECTOR vScale = { 1, 1, 1 }, cTV_3DVECTOR vRot = { 0, 0, 0 }
			, bool bEnable = true, float fMasse = 0, float fSpeed = 0, bool meshcontrolled = false, ePHYSMAT physmat = DEF_PHYSMAT
			, std::string pathtomesh = "", std::string textureName = "", int icolor = -1, CTVMesh* pMesh = nullptr, CTVActor* pActor = nullptr)
		{
			type = eType;
			geom = eGeom;
			name = sName;
			pathToMesh = pathtomesh;
			texName = textureName;
			pos = vPos;
			rot = vRot;
			scale = vScale;
			enable = bEnable;
			masse = fMasse;

			physMat = physmat;
			MeshControlled = meshcontrolled;
			tvMesh = pMesh;
			tvActor = pActor;
			color = icolor;
		}

		CTVMesh* getTVMesh() { return tvMesh; }
		void setTVMesh(CTVMesh* mesh) { tvMesh = mesh; }
		CTVActor* getTVActor() { return tvActor; }
		void setTVActor(CTVActor* actor) { tvActor = actor; }
	private:
		CTVMesh* tvMesh = nullptr;
		CTVActor* tvActor = nullptr;
	};
}