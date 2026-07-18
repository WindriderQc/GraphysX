#pragma once
#include "StdAfx.h"
#include <ode/ode.h>	// Entête de la librairie ODE

#define LENGTH  4.59216064452	// chassis length
#define WIDTH  2.2092375183	// chassis width
#define HEIGHT  1.62787181856	// chassis height
#define RADIUS  0.36	// wheel radius
#define STARTY  3	// starting height of chassis   // 3 par defaut
#define CMASS  1.4		// chassis mass
#define WMASS  0.17	// wheel mass
#define WHEEL_OFFSET  0
#define ACCELERATION  0.25
#define TURN_SPEED  0.07
#define SUSPENSION_ERP 0.4
  // try it, it's fun!
#define WHEEL_WOBBLE 0.0



#define K_UP	200
#define K_DOWN	208
#define K_LEFT	203
#define K_RIGHT	205
#define K_X		45
#define K_SPACE	57




class ODEMoteur
{
public:
	/*Constructeur de la classe*/
	ODEMoteur(void);
	/*Destructeur de la classe*/
	~ODEMoteur(void);
	/*Méthode d'initialisation du support ODE*/
	void initialiser(void);
	/* on met à jour le moteur de simulation en avançant d'un pas*/
	void update();
	/*Méthode d'interaction sur la balle*/
	void interagir();
	// Méthode qui associe un Mesh a un GeomID
	void SetGeomsMeshIndex(dGeomID GeomID, short MeshIndex);
	
	void DoPhysics(int iKeyPressed );
	void UpdatePhysics(void);
	void ReactToKeyPressed(int KeyPressed);

	float GetGeomPositionX(dGeomID GeomID);
	float GetGeomPositionY(dGeomID GeomID);
	float GetGeomPositionZ(dGeomID GeomID);
	float GetBodyPositionX(dBodyID BodyID);
	float GetBodyPositionY(dBodyID BodyID);
	float GetBodyPositionZ(dBodyID BodyID);

	float GetGeomRotMatrix11(dGeomID GeomID);
	float GetGeomRotMatrix12(dGeomID GeomID);
	float GetGeomRotMatrix13(dGeomID GeomID);
	float GetGeomRotMatrix14(dGeomID GeomID);
	float GetGeomRotMatrix21(dGeomID GeomID);
	float GetGeomRotMatrix22(dGeomID GeomID);
	float GetGeomRotMatrix23(dGeomID GeomID);
	float GetGeomRotMatrix24(dGeomID GeomID);
	float GetGeomRotMatrix31(dGeomID GeomID);
	float GetGeomRotMatrix32(dGeomID GeomID);
	float GetGeomRotMatrix33(dGeomID GeomID);
	float GetGeomRotMatrix34(dGeomID GeomID);
	float GetGeomRotMatrix41(dGeomID GeomID);
	float GetGeomRotMatrix42(dGeomID GeomID);
	float GetGeomRotMatrix43(dGeomID GeomID);
	float GetGeomRotMatrix44(dGeomID GeomID);

	dGeomID GeomTable[MAX_PHYSIC_MESH];



	float     speed, steer;
	bool     MotorRunning;
	int TractionMode;  //  1 = FWD   2 = RWD   3 = 4WD

protected:
	dSpaceID identifiantSpace;
	
	
	/*
	dBodyID HummerBody;
	dBodyID WheelBody[4];
	dGeomID HummerGeom;
	dGeomID WheelGeom[4];
	dJointID CarWheelJoint[4];	
	*/

	dBodyID BilleBody;
	dGeomID GroundGeom;
	dGeomID CielGeom;
	dGeomID BilleGeom;
	
	DWORD    Start;
    dReal    RemainingTime;

};
