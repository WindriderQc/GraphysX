#pragma once
#ifndef __TV3DMoteur_H__
#define __TV3DMoteur_H__

#include "Global.h"  //   doit etre avant les autre header pour avoir les include du stdafx....   bug....


// TrueVision3D Header Files (For Reference) 
#include "CTVActor.h" 
//#include "CTVAI.h" 
#include "CTVAtmosphere.h" 
//#include "CTVBitmapParts.h" 
#include "CTVCamera.h" 
#include "CTVCameraFactory.h" 
#include "CTVCollisionResult.h" 
#include "CTVDeviceInfo.h" 
#include "CTVEngine.h" 
#include "CTVGameController.h" 
#include "CTVGameControllers.h" 
#include "CTVGlobals.h" 
#include "CTVGraphicEffect.h" 
#include "CTVInputEngine.h" 
//#include "CTVInternalObjects.h"     //  DX headers needs to be include in order to use this.
#include "CTVLandscape.h" 
#include "CTVLightEngine.h" 
#include "CTVMaterialFactory.h" 
#include "CTVMathLibrary.h" 
#include "CTVMesh.h" 
#include "CTVMiniMesh.h" 
#include "CTVNode.h" 
//#include "CTVOctree.h" 
#include "CTVParticleSystem.h" 
#include "CTVPath.h" 
//#include "CTVPhysics.h"
#include "CTVRenderSurface.h" 
#include "CTVScene.h" 
#include "CTVScreen2DImmediate.h" 
#include "CTVScreen2DText.h" 
#include "CTVShader.h" 
#include "CTVTextureFactory.h" 
//#include "CTVTileMap.h" 
//#include "CTVViewport.h" 
#include "HelperFunctions.h" 
#include "tv_types.h" 

#include "newton.h"
#include "dMatrix.h" 
//#include "CustomJoint.h"

typedef std::vector<cTV_3DVECTOR> vecPosition3D;

#define MAX_PHYSICS_LOOPS			1
#define MAX_PHYSICS_FPS				120.0f



struct sVERTEX
{
	float x; float y; float z;
	sVERTEX() : x(0), y(0), z(0){};
};

struct sLine3D{
	cTV_3DVECTOR Start, End;
	sLine3D() : Start(cTV_3DVECTOR(0, 0, 0)), End(cTV_3DVECTOR(0, 0, 0)){};
};


class CLTV3DMoteur
{
private:
	CLTV3DMoteur();
	~CLTV3DMoteur();
	static CLTV3DMoteur *_singleton;

	CLGlobalVar* clGlobalVar;
	
	
public:
	// Fonctions de création et destruction du singleton
    static CLTV3DMoteur *getInstance ()
	{
		if (NULL == _singleton)
		  {
		    _singleton =  new CLTV3DMoteur;
		  }
	 return _singleton;
	}
    static void kill ()
	{
	  if (NULL != _singleton)
	   {
	     delete _singleton;
	     _singleton = NULL;
	   }
	}

	void DrawTV3DLine(cTV_3DVECTOR VecA, cTV_3DVECTOR VecB, int Color);
	void DrawMeshAxis(CTVMesh* mesh, int iAxisLength);
	void DrawTV3DAxis(void);
	void DrawWireframe(CTVMesh* Mesh);
	void DrawSpline(vecPosition3D Spline, int RGBAColor);
	void SetMeshOpacity(CTVMesh* Mesh, float fOpacity);
	vecPosition3D CreateTrianglePile(int NbrEtage, cTV_3DVECTOR pos, float BoxSize);
	

	CTVEngine* pTV;
	CTVScene* pTVScene;
	CTVMathLibrary* pTVMaths;
	CTVTextureFactory* pTVTexturefactory;
	CTVMaterialFactory* pTVMaterialfactory;
	CTVScreen2DImmediate* p2DImmediate;

	int iStdMaterial;// STD lighting material
	int iTranslucentMaterial;
	int iGlassMat;  // BallZShell material
};


///////////////////////////////////////////////
//  Animator
//  Banque de méthode qui interagisse avec la scene
///////////////////////////////////////////////////
/*
class CLAnimator
{
public:
	CLAnimator(void)
	{
	pTVScene = new CTVScene();
	pMath = new CTVMathLibrary();
	};
	~CLAnimator(void)
	{
	delete(pTVScene); pTVScene = NULL;
	delete(pMath); pMath = NULL;
	};
	void UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land, float fTimeElapsed)
	{
		// We need a position for the Mesh
		cTV_3DVECTOR MeshPosition = Mesh->GetPosition();
		// We need a destination for the Mesh
		cTV_3DVECTOR MeshDestination = dest;
		// We need a direction for the Mesh to make it point in the right direction.
		cTV_3DVECTOR MeshDirection;
		// We need angles for the Mesh
		float MeshAngleY;

		cTV_3DVECTOR dVector;
		pMath->TVVec3Subtract(&dVector, &MeshDestination, &MeshPosition);
		pMath->TVVec3Normalize(&dVector, &dVector);
		MeshDirection = dVector;
		// Update the Mesh's angle
		if (MeshDirection.z > 0)
			MeshAngleY = pMath->Rad2Deg(atan(MeshDirection.x / MeshDirection.z));
		else
			MeshAngleY = pMath->Rad2Deg(atan(MeshDirection.x / MeshDirection.z)) + 180;

		// Set the  mesh rotation
		Mesh->SetRotation(0.0f, MeshAngleY, 0.0f);


		// Check if Mesh has reached destination, if not, update the Mesh
		// position by adding a scale of the vector destination.
		if ((pMath->GetDistance3D(MeshPosition.x, 0, MeshPosition.z, MeshDestination.x, 0, MeshDestination.z)) > 2)
		{
			// Update all the Mesh's position
			cTV_3DVECTOR dV2;
			pMath->TVVec3Scale(&dV2, &MeshDirection, (fTimeElapsed * 0.1f));
			pMath->TVVec3Add(&MeshPosition, &MeshPosition, &dV2);
			MeshPosition.y = Land->GetHeight(MeshPosition.x, MeshPosition.z) + 10;
			// Update the  mesh position
			Mesh->SetPosition(MeshPosition.x, MeshPosition.y, MeshPosition.z);
		}
	};

private:
	CTVScene* pTVScene;
	CTVMathLibrary* pMath;
};
*/
#endif