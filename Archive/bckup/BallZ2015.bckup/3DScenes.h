#ifndef _CL3DScenes_h__
#define _CL3DScenes_h__
#pragma once
#include "Scene.h"
#include "3DObject.h"
#include "CLBallZ.h"
#include "VoieLactee.h"
#include "Atmosphere.h"
#include "Anneaux.h"
#include "SplineFollow.h"
#include "BigWay.h"
#include "MeshFactory.h"
#include "Actor.h"
#include "Animator.h"

//#include "dCustomJoints\CustomKinematicController.h"



class CLBallZScene : public CLScene
{
private:
	CLBallZ* clBallZ = NULL;
	CLVoieLactee *clVoieLactee;
	CL3DObject *clObjectNowhere, *clElevatorfloor, *clTestBox;
	int iUpDown, iElevatorGate;
	CLSplineFollow *clPlanePATH;
	CLAnneaux *clAnneaux;
	CLAtmosphere *clAtmosphere;
	CLWater *clWater;
	//CLLand *clLand;
	cTV_3DVECTOR VecPosAnneaux[100];
	CTVMesh *floor;

	CTVLandscape* pLand;
	CTVCollisionResult* pClickCollision;
	CTVMesh* pMeshClicked;
	CTVMesh* pOldMeshClicked;

	cTV_3DVECTOR Mouse3DPos;

	CLActorFactory* clActorFactory;
	CLAnimator* clAnimator;
	



	//Texture ID
	int iYellowtwowaysignTex, iGridTex, iDamierTex, i3D_Spheres, iAbstractCubes, iMagicienTex, IDParticleTex;
	STSkybox stSkyDay, stSkyNight, stSkyStars;

	//elevator Gates
	enum eGATES {GATE0, GATE1, GATE2};

	vector<sLine3D> vec3DVecToDraw;

	// shader
	float fBumpAmount;
	int IDDiffuse, IDNormal;
	CTVShader* m_Shader;


	void SetPhysicMaterials(void);
	void LoadTextures(void);
	void SetupScene(void);
public:
	CLBallZScene();
	virtual ~CLBallZScene(void);
	
	int iPhysicMaterials[5];

	void AddInteractions();
	void Render();
	void UpdateScene(void);
	void UpdateWater(long lTimeOfDay);
	void ManageInput(void);
	void ManageMeshClicked(void);
	void MouseGrab(void);
	void MouseHighlight(void);
	void MouseDrop(void);
	void LoadFormation(CLXMLData* data);
	void CreateFormation(int iDefaultNbrFlyer);
	void CreateBoxTrianglePile(float fBoxSize, cTV_3DVECTOR pos, int iTexture);
	
	CTVMesh* GetMeshClicked(){ return pMeshClicked; };
	CLFormation* clFormation;

	
};
#endif