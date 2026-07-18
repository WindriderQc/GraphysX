#pragma once
#include "Entity3D.h"
//#include <TV3D65\tv_types.h>
#include <TV3D65\CTVActor.h>


namespace GraphysX
{
	class CLActor : public CLEntity3D
	{
	public:
		CLActor(STEntityParam geomParam, CTVShader* m_Shader);
		CLActor(STEntityParam geomParam, char* Path, CTVShader* m_Shader); 
		virtual ~CLActor();

		void update();
		void MoveBodyToBone();

		STEntityParam stParam;

		CTVActor* tvActor;
		cTV_3DVECTOR vBoxMin, vBoxMax;
		cTV_3DMATRIX mxOriginBone;
		cTV_3DMATRIX mxMesh;
	};
}