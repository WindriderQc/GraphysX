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

//#include "dCustomJoints\CustomKinematicController.h"

class CLBallZScene : public CLScene
{
	//friend void ApplyForceAndTorqueCallbackElevator(const NewtonBody* body, dFloat timestep, int threadIndex);
	//friend void ApplyTransformationCallbackElevator(const NewtonBody *_body, const dFloat *_matrix, int threadIndex);
private:
	CLBallZ* clBallZ;
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

	CTVMesh* XMesh;
	CTVLandscape* pLand;
	CTVCollisionResult* pClickCollision;
	CTVMesh* pMeshClicked;
	CTVMesh* pOldMeshClicked;
	
	// Physic Material
	int defaultID, woodID, FinishID, floorID, BallPhysID, ElevatorPhysID;

	//Texture ID
	int iYellowtwowaysignTex, iGridTex, iDamierTex, i3D_Spheres, iAbstractCubes, iMagicienTex, IDParticleTex;
	STSkybox stSkyDay, stSkyNight, stSkyStars;

	//elevator Gates
	enum eGATES {GATE0, GATE1, GATE2};

public:
	CLBallZScene();
	virtual ~CLBallZScene(void);
	void Render(long lTimeOfDay);
	void UpdateScene(void);
	void UpdateWater(long lTimeOfDay);
	void ManageInput(void);
	void MouseGrab(void);
	void MouseDrop(void);
	CTVMesh* GetMeshClicked(){ return pMeshClicked; };
	
	void CreateBoxTrianglePile(float fBoxSize, cTV_3DVECTOR pos, int iTexture);

	CLFormation* clFormation;

private:
	void SetPhysicMaterials(void);
	void LoadTextures(void);
	void SetupScene(void);
};
#endif