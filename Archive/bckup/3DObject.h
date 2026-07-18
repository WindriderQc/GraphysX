#ifndef __3DObject_H__
#define __3DObject_H__
#pragma once
#include "TV3DMoteur.h"

#define MILLISEC_PER_DAY 86400000   //  defini 2 fois, dans atmos aussi, a arranger
#define SUN_ORBIT_RADIUS 10000
#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846



enum eTYPEOBJECT {SPHERE, CUBE, BILLBOARD, FLOOR, CUSTOM};
enum ePHYSICTYPE {PHYSICSTATIC = 1, PHYSICALL};

//  AJOUTER LE iMaterial AU CONSTRUCTEUR DE L'OBJECT COMME LA TEXTURE....

class CL3DObject
{
public:
	CL3DObject(int iTYPE, char* name, char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float Masse, int iPHYSIC, int iNewtonMat, NewtonWorld* world);
	~CL3DObject(void);
	
	void Render(void);
//	void DrawWireframe(void);
	void NewtonDebug(void);
	void SetPosition(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot);
	
	CTVMesh* Mesh;
	char* Name;
	NewtonBody* pbody = NULL;
	NewtonCollision* Collision = NULL;
	float fMasse;
	// Info pour échange avec physics engine
	cTV_3DVECTOR vCtrlTorque;
	dVector Torque1;
	bool bCTRLBALL;
	bool bJumping;
	bool bAllowJumps; // me semble pas clean comme architecture...   ptete creer des classe qui hérite de 3Dobject..?
	bool bIsFinishline; // me semble pas clean comme architecture...   ptete creer des classe qui hérite de 3Dobject..?
	bool bIsOnElevator;  // me semble pas clean comme architecture...   ptete creer des classe qui hérite de 3Dobject..?

	
private:
	CLTV3DMoteur* clTV3D;
	void BuildPhysicsMesh(CTVMesh* mesh);
	NewtonWorld* World = NULL;		

	// Animation 
public:
	int iAnimNbrFrame; // Durée de l'animation
	bool bEtat;  // Animation Start/Stop
	void Translate(cTV_3DVECTOR Force, float SpeedFactor);
	void Rotate( cTV_3DVECTOR Force, float SpeedFactor);
	void RotateAround( float SpeedFactor, cTV_3DVECTOR *Center, float fRadius);
	void ChangeColor( int Intensity, float SpeedFactor);
	void Stop(void) { bAnimState = false; }
	void Start(void){ bAnimState = true; }
	void Reset(void){ iActualFrame = 0; }
	void ChangeAnimDuration(int iFrameNbr){ iAnimNbrFrame = iFrameNbr; }
private:
	int iActualFrame;  // Compteur
	bool bAnimState;	   // active ou pas 
	float fTime;
};
#endif