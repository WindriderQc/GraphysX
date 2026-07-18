#pragma once
#include "TV3DMoteur.h"
#include "newton.h"


class CLActor
{
public:
	CLActor(CTVShader* m_Shader);
	CLActor(char* Name, char* Path, cTV_3DVECTOR Pos, CTVShader* m_Shader, NewtonWorld* World);
	~CLActor();

	void Update(float fTimeElapsed);
	void MoveBodyToBone();

	CTVActor* tvActor;
	cTV_3DVECTOR vBoxMin, vBoxMax;
	cTV_3DMATRIX mxOriginBone;
	cTV_3DMATRIX mxMesh;
	
private:
	GraphysX::CLTV3D clTV3D;
};
