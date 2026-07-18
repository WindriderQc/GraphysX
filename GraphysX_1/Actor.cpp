#include "stdafx.h"
#include "Actor.h"
#include "TV3DMoteur.h"
#include <TV3D65\CTVActor.h>

/*void ActorTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	//char* Name = ((CTVMesh*)NewtonBodyGetUserData(body))->GetMeshName();
	//if (!strcmp(Name, "Suzanne1.RotatorCube"))
	//{
	//	int i = 0;
	//}
	//dMatrix mx;
	//NewtonBodyGetMatrix(body, &mx[0].m_w);
	CLActor* clactor = ((CLActor*)NewtonBodyGetUserData(body));

	cTV_3DMATRIX mxBone = clactor->tvActor->GetBoneMatrix(1);
	//cTV_3DVECTOR postv = clactor->tvActor->GetBonePosition(1);
	cTV_3DMATRIX mxMesh = clactor->mxMesh;						// Le format de matrix de Bone est pas compatible avec Newton...  ??!!  
	mxMesh.m41 = mxBone.m41 - clactor->mxOriginBone.m41;  //        donc on prends la matrix du mesh comme base et applique la position du bone.
	mxMesh.m42 = mxBone.m42 - clactor->mxOriginBone.m42;
	mxMesh.m43 = mxBone.m43 - clactor->mxOriginBone.m43;


	//CLTV3D::getInstance()->DrawTV3DLine(((CLActor*)NewtonBodyGetUserData(body))->vBoxMin, ((CLActor*)NewtonBodyGetUserData(body))->vBoxMax, RGBA_YELLOW);
	NewtonBodySetMatrix(body, mxMesh.m[0]); // A Corriger pour choisir le Bone en question (1)
}*/

namespace GraphysX
{
	CLActor::CLActor(STEntityParam geomParam, CTVShader* m_Shader) : stParam(geomParam)  //  TODO  : disssocier le shader de la classe actor
	{
		tvActor = CLTV3D::pTVScene.CreateActor("CubX");
		if (!tvActor->LoadTVA("..\\StockRoom\\CubXScreensaver.tva")){
			MSGBOX("Actor", "Loading Error");
		}
		else
		{
			tvActor->SetScale(0.03f, 0.03f, 0.03f);
			tvActor->SetPosition(20, 5, 20);
			tvActor->SetShadowCast(true, true);
			tvActor->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE);
			if (m_Shader)
				tvActor->SetShader(m_Shader);							 //Apply the shader to the mesh.
			int animcount = tvActor->GetAnimationCount();
			char* test = tvActor->GetAnimationName(0);
			tvActor->PlayAnimation(1);
		}
	}
	CLActor::CLActor(STEntityParam geomParam, char* Path, CTVShader* m_Shader) : stParam(geomParam)// , NewtonWorld* World)
	{

		tvActor = CLTV3D::pTVScene.CreateActor(stParam.name.c_str());
		if (!tvActor->LoadXFile(Path, true, true)){
			MSGBOX("Actor", "Loading Error");
		}
		else
		{
			tvActor->SetPosition(stParam.pos.x, stParam.pos.y, stParam.pos.z);
			tvActor->Enable(true);
			tvActor->SetShadowCast(true, true);
			tvActor->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE);

			if (m_Shader)
				tvActor->SetShader(m_Shader);							 //Apply the shader to the mesh.

			int animcount = tvActor->GetAnimationCount();
			char* test = tvActor->GetAnimationName(0);
			char* test1 = tvActor->GetAnimationName(1);

			tvActor->PlayAnimation(1);
			tvActor->SetAnimationLoop(true);
			tvActor->GetBoundingBox(&vBoxMin, &vBoxMax);

			/*if (World){
				CTVMesh* tvMesh = tvActor->GetDeformedMesh();
				mxMesh = tvMesh->GetMatrix();
				tvMesh->Enable(false);
				mxOriginBone = tvActor->GetBoneMatrix(1, false);

			}*/

			std::cout << std::endl << stParam.name << " Loaded!" << std::endl;
			std::cout << "Bone Count : " << tvActor->GetBoneCount() << " & Triangle Count : " << tvActor->GetTriangleCount() << std::endl << std::endl;
		}
	}
	CLActor::~CLActor()
	{
		tvActor->Destroy();
	}
	void CLActor::update()// float fTimeElapsed)
	{
		tvActor->GetBoundingBox(&vBoxMin, &vBoxMax);
	}
	/*void CLActor::MoveBodyToBone()   // TODO : devrait etre agencé avec un Kinematic body...
	{
	CTVMesh* mesh = tvActor->GetDeformedMesh();
	cTV_3DMATRIX mxMesh = mesh->GetMatrix();
	cTV_3DMATRIX mxBone = tvActor->GetBoneMatrix(1);
	//cTV_3DVECTOR postv = clactor->tvActor->GetBonePosition(1);

	mxBone.m41 = mxBone.m41 - mxOriginBone.m41;
	mxBone.m42 = mxBone.m42 - mxOriginBone.m42;
	mxBone.m43 = mxBone.m43 - mxOriginBone.m43;

	//mxMesh.m11 = mxBone.m[0][0];// postv.x;
	//mxMesh.m22 = mxBone.m[1][1];
	//mxMesh.m33 = mxBone.m[2][2];
	mxMesh.m41 = mxBone.m[3][0];
	mxMesh.m42 = mxBone.m[3][1];
	mxMesh.m43 = mxBone.m[3][2];

	//CLTV3D::getInstance()->DrawTV3DLine(((CLActor*)NewtonBodyGetUserData(body))->vBoxMin, ((CLActor*)NewtonBodyGetUserData(body))->vBoxMax, RGBA_YELLOW);
	NewtonBodySetMatrix(clPhysicEntity->pbody, mxMesh.m[0]); // A Corriger pour choisir le Bone en question (1)

	mesh->Destroy();
	}*/

}