#include "ODEMoteur.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float                
#pragma warning(disable: 4244) // conversion from 'double' to 'dReal', possible loss of data   

#define SQR(x)     x * x     /* square a number */

/*variables/fonctions globales*/
float   GlobalFriction;
float   GlobalSuspension;
dWorldID IDWorld;
dJointGroupID contactgroup;
dGeomID WheelGeom4Slip[4];

void nearCallback(void *data,dGeomID o1, dGeomID o2);
dReal CalculateSlip(dGeomID o1);



/*Constructeur de la classe*/
ODEMoteur::ODEMoteur(void)
{
	int i;

	// Création d'un monde ODE 3D -> container pour stocker les différents corps rigides
	// La fonction dWordCreate() retourne un numéro d'identifiant unique sur notre container
	IDWorld = dWorldCreate();
	// On applique sur notre monde de la force gravitationnelle
	// Sur la planete Terre, elle est de l'ordre de -9,81 m/s²
	// Les objets en suspension dans l'air tomberont alors vers le bas (loi de Newton)
    dWorldSetGravity(IDWorld, 0.0f, -9.81f, 0.0f);
	// Réduction des erreurs de précisions et des jointures par correction
	// Doit être compris entre 0.1 et 0.8 ( avec 0.8, meilleur correction )
	// Valeur par défaut : 0.2
	dWorldSetERP(IDWorld, 0.8f);
	// Correction logicielle des contraintes de force sur les jointures
	// Rappel:  une jointure permet de "souder" deux corps rigides entre eux
	// Les contraintes hard sont coorigées par l'ERP, et le soft par le CFM.
	// Valeurs comprises entre 10e-9 et 1
	dWorldSetCFM(IDWorld, 0.00001f);
	// Création d'un nouveau espace vide pour acceuillir des formes géométriques (container)
	// Cet espace sert uniquement aux moteur de collisions de ODE.
	// La fonction dSimpleSpaceCreate(0) retourne un numéro d'identifiant unique sur notre container
	identifiantSpace = dHashSpaceCreate(0);


    for(i=0;i<MAX_PHYSIC_MESH;i++)
	  {
	  GeomTable[i] = NULL;
	  }

	GlobalFriction = 0.4;      //  A tester avec différentes valeurs - setter sur une TrackBar 
	GlobalSuspension = 0.5;	   //  A tester avec différentes valeurs - setter sur une TrackBar
	TractionMode = 3;

	speed = 0;
	Start = 0;  // Variable pour UpdatePhysics
    RemainingTime = 0;
}



/*Destructeur de la classe*/
ODEMoteur::~ODEMoteur(void)
{
	/*Destruction des jointures existantes*/
	dJointGroupEmpty(contactgroup);

	/*Destruction du container des corps rigides ainsi que destruction de son contenu*/
	dWorldDestroy(IDWorld);

	/*Destruction de l'espace géométrique*/
	/*L'espace et son contenu (tous les objets géométriques) sont effacés*/
	dSpaceDestroy(identifiantSpace);

	/*Libération de la mémoire utilisée par ODE*/
	dCloseODE();
}



/*Méthode d'interaction sur la balle*/
void ODEMoteur::interagir()
{
	dBodySetForce(BilleBody,0.0,0.0,0.0);
	dBodySetPosition(BilleBody, 0.0f, 9.0f, -0.9f); //affecte une position
}



/*Méthode d'initialisation du support ODE*/
void ODEMoteur::initialiser(void)
{
// int        i;
// dVector3    a;
// dMatrix3    R;
 dMass masse;

	contactgroup = dJointGroupCreate(10000);
	dJointGroupEmpty(contactgroup);

	//définition du ciel
	CielGeom = dCreateBox(identifiantSpace, 10.0f,1.0f,10.0f);
	dGeomSetPosition(CielGeom, 0.0f,2500.0f, 0.0f);                // Hauteur du ciel a re-setter
	dGeomSetBody (CielGeom,0);

	//Création du sol
	// Pour obtenir un objet statique (immobile), il ne faut pas définir de corps(body)
	// La seule définition d'une géométrie associée à un bodyId nul suffit.
	GroundGeom = dCreateBox(identifiantSpace, 3000.0f, 2.0f, 3000.0f); //on crée la géométrie associée
	dGeomSetPosition(GroundGeom, 0.0f, -1.0f, 0.0f);
	dGeomSetBody (GroundGeom,0); 
	
	GeomTable[0] = GroundGeom;      //   Geom #0


	//Création de la bille
	BilleBody = dBodyCreate(IDWorld); //on crée le corps rigide
	dBodySetPosition(BilleBody, 0.0f, 500.0f, 0.0f); //affecte une position
	dBodyEnable(BilleBody); //active le corps
	dBodySetGravityMode(BilleBody,1); //1= influencé par la gravité
	dMassSetSphere(&masse, 2.0f, 0.3f);
	dBodySetMass(BilleBody, &masse);
	dBodySetForce(BilleBody,0.0,0.0,+10.0);
	BilleGeom = dCreateSphere(identifiantSpace,100.0f); //on crée la géométrie associée
	dGeomSetBody (BilleGeom,BilleBody); //et on associe finalement le corps rigide à la géométrie
	
	GeomTable[1] = BilleGeom;      //   Geom #1

	


	/*
	// Hummer Body
	HummerBody =  dBodyCreate(IDWorld);
    dMassSetBox(&masse, 1, LENGTH, HEIGHT, WIDTH);
    dMassAdjust(&masse, CMASS);
    dBodySetMass(HummerBody, &masse);
    dRFromAxisAndAngle(R, 1, 0, 0, -M_PI*0.5);
    dBodySetRotation(HummerBody, R);

    dBodySetPosition(HummerBody, 0, STARTY, 0);   
 //   dMassRotate(&masse,R);
 //   dMassTranslate(&masse, 0, STARTY, 0);
    HummerGeom =  dCreateBox(identifiantSpace, LENGTH, HEIGHT, WIDTH);
    dGeomSetBody(HummerGeom, HummerBody);
	GeomTable[0] = HummerGeom;                     //  Geom #0


	// wheel bodies
   for( i= 0; i<4; i++)
    { 
      WheelBody[i] =  dBodyCreate(IDWorld);

      dMassSetSphere(&masse, 1, RADIUS);
      dMassAdjust(&masse, WMASS);
      dBodySetMass(WheelBody[i], &masse);
      dRFromAxisAndAngle(R, 1, 0, 0, -M_PI*0.5);
      dBodySetRotation(WheelBody[i], R);

      WheelGeom[i] =  dCreateCylinder(identifiantSpace,RADIUS, 0.3);
      dGeomSetBody(WheelGeom[i], WheelBody[i]);
	  GeomTable[i+1] = WheelGeom[i];                     //  Geom #1 à #4
	  WheelGeom4Slip[i] = WheelGeom[i];
	} 
   dBodySetPosition(WheelBody[0],  0.38*LENGTH, STARTY-HEIGHT*0.5-WHEEL_OFFSET,  WIDTH*0.45);
   dBodySetPosition(WheelBody[1],  0.38*LENGTH, STARTY-HEIGHT*0.5-WHEEL_OFFSET, -WIDTH*0.45);
   dBodySetPosition(WheelBody[2], -0.33*LENGTH, STARTY-HEIGHT*0.5-WHEEL_OFFSET,  WIDTH*0.45);
   dBodySetPosition(WheelBody[3], -0.33*LENGTH, STARTY-HEIGHT*0.5-WHEEL_OFFSET, -WIDTH*0.45);


 // front and back wheel hinges
   for( i= 0; i<=3; i++)
    { 
      CarWheelJoint[i] =  dJointCreateHinge2(IDWorld, 0);
      dJointAttach(CarWheelJoint[i], HummerBody, WheelBody[i]);
	  a[0] = GetBodyPositionX(WheelBody[i]);
	  a[1] = GetBodyPositionY(WheelBody[i]);
	  a[2] = GetBodyPositionZ(WheelBody[i]);
      dJointSetHinge2Anchor(CarWheelJoint[i], a[0], a[1], a[2]);
      dJointSetHinge2Axis1(CarWheelJoint[i], 0, -1, 0);
      dJointSetHinge2Axis2(CarWheelJoint[i], 0, 0, -1);
    } 

   // set joint suspension
   for( i= 0; i<=3; i++)
    { 
      dJointSetHinge2Param(CarWheelJoint[i], dParamSuspensionERP, SUSPENSION_ERP);
      dJointSetHinge2Param(CarWheelJoint[i], dParamSuspensionCFM, GlobalSuspension);
    } 

   // lock back wheels along the steering axis
   for( i= 2; i<=3; i++)
    { 
      // set stops to make sure wheels always stay in alignment
      dJointSetHinge2Param(CarWheelJoint[i], dParamLoStop, -WHEEL_WOBBLE);
      dJointSetHinge2Param(CarWheelJoint[i], dParamHiStop, +WHEEL_WOBBLE);
    } 

*/

	srand( (unsigned)time( 0 ) );
}



/* on met à jour le moteur de simulation en avançant d'un pas*/
void ODEMoteur::update()
{
	dSpaceCollide (identifiantSpace, 0, nearCallback);
	
	const int N = 10;
	dContact contact[N];
	int n = dCollide (BilleGeom, GroundGeom, N, &contact[0].geom, sizeof(dContact));
	if (n>0)
	{
		//printf("Collision de la bille sur le sol !!\n");
	//	dBodyAddForce(BilleBody,rand()%400-200,rand()%200+200,-rand()%100-100);
	}
	

	dWorldStep(IDWorld,0.01f);
	dJointGroupEmpty(contactgroup);
}

void ODEMoteur::DoPhysics(int iKeyPressed )
 { 
   if(!((iKeyPressed == K_LEFT) || (iKeyPressed == K_RIGHT)))
     {
      steer =  steer * 0.85;
     }
   if(!((iKeyPressed == K_UP) || (iKeyPressed == K_DOWN)))
    { 
      speed =  speed * 0.99;
      MotorRunning =  false;
    } 

  if(steer > 0.95)  
      steer =  0.95;

   if(steer < -0.95) 
      steer =  -0.95;

   if(speed > 15)   
      speed =  15;

   if(speed < -15)   
      speed =  -15;


 if(MotorRunning)
  { 
      // motor
      switch(TractionMode)
	  {
	  //FWD	 
	  case 1: 
//         dJointSetHinge2Param(CarWheelJoint[0], dParamVel2, -speed);
//         dJointSetHinge2Param(CarWheelJoint[0], dParamFMax2, 2);

//         dJointSetHinge2Param(CarWheelJoint[1], dParamVel2, -speed);
//         dJointSetHinge2Param(CarWheelJoint[1], dParamFMax2, 2);
         break;

      //RWD
	  case 2:
        // dJointSetHinge2Param(CarWheelJoint[2], dParamVel2, -speed);
         //dJointSetHinge2Param(CarWheelJoint[2], dParamFMax2, 0.1);
         
		 //dJointSetHinge2Param(CarWheelJoint[3], dParamVel2, -speed);
        // dJointSetHinge2Param(CarWheelJoint[3], dParamFMax2, 0.1);
		 break;
	  
	  //4x4
	  case 3: 
         /* 
		 dJointSetHinge2Param(CarWheelJoint[0], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[0], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[1], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[1], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[2], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[2], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[3], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[3], dParamFMax2, 0.77);
		 */
         break;
	  }
	}  
   else
    { 
      // the clutch is in! (not accel or breaking)
      switch(TractionMode)
	  {
	  //FWD	 
	  case 1: 
         /*
		 dJointSetHinge2Param(CarWheelJoint[0], dParamVel2, speed);
         dJointSetHinge2Param(CarWheelJoint[0], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[1], dParamVel2, speed);
         dJointSetHinge2Param(CarWheelJoint[1], dParamFMax2, 0.77);
		 */
         break;

      //RWD
	  case 2:
		  /*
         dJointSetHinge2Param(CarWheelJoint[2], dParamVel2, 0);
         dJointSetHinge2Param(CarWheelJoint[2], dParamFMax2, 0.005);

         dJointSetHinge2Param(CarWheelJoint[3], dParamVel2, 0);
         dJointSetHinge2Param(CarWheelJoint[3], dParamFMax2, 0.005);
		 */
      	 break;
	  
	  //4x4
	  case 3: 
		  /*
         dJointSetHinge2Param(CarWheelJoint[0], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[0], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[1], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[1], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[2], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[2], dParamFMax2, 0.77);

         dJointSetHinge2Param(CarWheelJoint[3], dParamVel2, -speed);
         dJointSetHinge2Param(CarWheelJoint[3], dParamFMax2, 0.77);
		 */
		 break;
	  } 
	}
 
	  
 } 

void ODEMoteur::UpdatePhysics(void)
{
 
 double UpdatesPerSecond = 120;
 double        UpdateTime;
 double    ODEStepSize;
  
   UpdateTime =  1000  /  UpdatesPerSecond;
   if((Start == 0)   ) 
	   Start =  GetTickCount();

   RemainingTime =  RemainingTime + (GetTickCount() - Start);
   Start =  GetTickCount();

   if((RemainingTime > 250)   )
      RemainingTime =  0;

   while( (RemainingTime > UpdateTime))
    { 
      dSpaceCollide(identifiantSpace, NULL, nearCallback);
   /*   if((jointDoor > 0)    )
         if ((Vector3Length(jf->f1) > 10) || (Vector3Length(jf->t1) > 10) || (Vector3Length(jf->f2) > 10) || (Vector3Length(jf->t2) > 10)) then
          { 
            dJointSetFeedback(jointDoor, NULL);
            dJointDestroy(jointDoor);
            jointDoor =  0;
            dSpaceRemove(space2, Door);
            dSpaceAdd(space, Door);
          } */

      ODEStepSize =  5 / UpdatesPerSecond;  // defaut 5

   
      dWorldStep(IDWorld, ODEStepSize);

	//Weird tweak to keep wheels aligned              // PKOI en DOUBLE????  A vérifier si pertinent...
/*
	  dJointSetHinge2Param(CarWheelJoint[0], dParamLoStop, steer);
      dJointSetHinge2Param(CarWheelJoint[0], dParamHiStop, steer);
      dJointSetHinge2Param(CarWheelJoint[1], dParamLoStop, steer);
      dJointSetHinge2Param(CarWheelJoint[1], dParamHiStop, steer);
      dJointSetHinge2Param(CarWheelJoint[2], dParamLoStop, 0);
      dJointSetHinge2Param(CarWheelJoint[2], dParamHiStop, 0);
      dJointSetHinge2Param(CarWheelJoint[3], dParamLoStop, 0);
      dJointSetHinge2Param(CarWheelJoint[3], dParamHiStop, 0);
*/
      // Remove all contact joints
      dJointGroupEmpty(contactgroup);

      RemainingTime = RemainingTime - UpdateTime;
 } 

}
 // Methode qui fait reagir le Buggy selon les touches entrées par le user
 void ODEMoteur::ReactToKeyPressed(int KeyPressed)
 {
  switch(KeyPressed)
  {
      case K_RIGHT:
         steer =  steer + TURN_SPEED;
		 break;
      case K_LEFT:
         steer =  steer - TURN_SPEED;
		 break;
      case K_UP:   
         MotorRunning =  true;
		 if(speed >= 0)
		    {
			 speed =  speed + ACCELERATION;
		    }
		 else{
			 speed = 0;
		     }
		 break;
      case K_DOWN:  
         MotorRunning =  true;
         if(speed <= 0)
		     {
            speed =  speed - ACCELERATION;
		     }
		 else{
			 speed = 0;
		     }
         break;
      case K_SPACE:    //HandBrake
       /*  dJointSetHinge2Param(joint[2], dParamVel2, 0);
         dJointSetHinge2Param(joint[2], dParamFMax2, 0.8);

         dJointSetHinge2Param(joint[3], dParamVel2, 0);
         dJointSetHinge2Param(joint[3], dParamFMax2, 0.8);*/ /// a integrer lors de l'integration de la detection du KeyRelease
		break;
      case K_X:    //Flip Buggy
       //  dBodyAddForce(WheelBody[0], 0, 3.5, 0);    //3.5
       // dBodyAddForce(WheelBody[1], 0, 3.5, 0);
       break;
  } 
 
 
 }
 // Méthose qui retourne la position X-Y-Z d'un Geom ou d'un Body
float ODEMoteur::GetGeomPositionX(dGeomID GeomID)
{
const dReal* positionGeom = dGeomGetPosition(GeomID);
return(positionGeom[0]);
}
float ODEMoteur::GetGeomPositionY(dGeomID GeomID)
{
const dReal* positionGeom = dGeomGetPosition(GeomID);
return(positionGeom[1]);
}
float ODEMoteur::GetGeomPositionZ(dGeomID GeomID)
{
const dReal* positionGeom = dGeomGetPosition(GeomID);
return(positionGeom[2]);
}


float ODEMoteur::GetBodyPositionX(dBodyID BodyID)
{
const dReal* positionBody = dBodyGetPosition(BodyID);
return(positionBody[0]);
}
float ODEMoteur::GetBodyPositionY(dBodyID BodyID)
{
const dReal* positionBody = dBodyGetPosition(BodyID);
return(positionBody[1]);
}
float ODEMoteur::GetBodyPositionZ(dBodyID BodyID)
{
const dReal* positionBody = dBodyGetPosition(BodyID);
return(positionBody[2]);
}

// Méthodes qui retourne les valeurs d'une matrices de rotation ODE
float ODEMoteur::GetGeomRotMatrix11(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[0]);
}
float ODEMoteur::GetGeomRotMatrix12(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[1]);
}
float ODEMoteur::GetGeomRotMatrix13(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[2]);
}
float ODEMoteur::GetGeomRotMatrix14(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(0);
}
float ODEMoteur::GetGeomRotMatrix21(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[4]);
}
float ODEMoteur::GetGeomRotMatrix22(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[5]);
}
float ODEMoteur::GetGeomRotMatrix23(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[6]);
}
float ODEMoteur::GetGeomRotMatrix24(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(0);
}
float ODEMoteur::GetGeomRotMatrix31(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[8]);
}
float ODEMoteur::GetGeomRotMatrix32(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[9]);
}
float ODEMoteur::GetGeomRotMatrix33(dGeomID GeomID)
{
const dReal* RotMatrixGeom = dGeomGetRotation(GeomID);
return(RotMatrixGeom[10]);
}
float ODEMoteur::GetGeomRotMatrix34(dGeomID GeomID)
{
return(0);
}
float ODEMoteur::GetGeomRotMatrix41(dGeomID GeomID)
{
return(0);
}
float ODEMoteur::GetGeomRotMatrix42(dGeomID GeomID)
{
return(0);
}
float ODEMoteur::GetGeomRotMatrix43(dGeomID GeomID)
{
return(0);
}
float ODEMoteur::GetGeomRotMatrix44(dGeomID GeomID)
{
return(1);
}
dReal CalculateSlip(dGeomID o1)
{
 int        i;
 dVector3    lvel;
 dMatrix3    rmat;
 dVector3    front;
 const dReal *RecVec;
 const dReal *RecMat;
 dReal    speed, dotprod, Result;
  
   Result =  0.1;

   for( i= 0; i<=3; i++)
    { 
      if((WheelGeom4Slip[i] == o1)   )
       { 
         RecVec = dBodyGetLinearVel(dGeomGetBody(o1));
		 lvel[0] = RecVec[0];
		 lvel[1] = RecVec[1];
		 lvel[2] = RecVec[2];
		 lvel[3] = RecVec[3];
         
		 RecMat = dGeomGetRotation(o1);
		 rmat[0] = RecMat[0];
		 rmat[1] = RecMat[1];
		 rmat[2] = RecMat[2];
		 rmat[3] = RecMat[3];
		 rmat[4] = RecMat[4];
		 rmat[5] = RecMat[5];
		 rmat[6] = RecMat[6];
		 rmat[7] = RecMat[7];
		 rmat[8] = RecMat[8];
		 rmat[9] = RecMat[9];
		 rmat[10] = RecMat[10];
		 rmat[11] = RecMat[11];

		 // get vector3 length
         speed = sqrt(SQR(lvel[0])+SQR(lvel[1])+SQR(lvel[2]));

         dNormalize3(lvel);

         front[0] =  rmat[2];
         front[1] =  rmat[6];
         front[2] =  rmat[10];
         dotprod =  dDOT(front, lvel);

         if(dotprod < 0) 
			 dotprod =  -dotprod;
         Result =  (1.0 - dotprod);
         Result =  Result * speed;
         Result =  Result * 0.035;//Scale
         if(Result < 0.01)    
			 Result =  0.01;
         if(Result > 0.8)   
			 Result =  0.8;
       } 
    } 

	return(Result);
 } 



/*Méthode de gestion de collision entre deux formes géométriques*/
void nearCallback(void *data,dGeomID o1, dGeomID o2)
{
	int i, n;
	dReal sl;

	const int N = 10;
	dContact contact[N];
	n = dCollide (o1, o2, N, &contact[0].geom, sizeof(dContact));
	if (n > 0)
	{
		for (i=0; i<n; i++)
		{/*
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu = 5000; //5000;
			contact[i].surface.bounce = 0.5;//0.3
			contact[i].surface.bounce_vel = 1.5;//0.2
  			*/
			contact[i].surface.mode =  dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM;// | dContactApprox1;  //  

         contact[i].surface.mu = GlobalFriction; //dInfinity;

         sl = CalculateSlip(o1);
         contact[i].surface.slip1 =  sl;
         contact[i].surface.slip2 =  sl;
         contact[i].surface.soft_erp =  0.5;//0.5
         contact[i].surface.soft_cfm =  0.3;//0.3
			
			
			dJointID c = dJointCreateContact (IDWorld,contactgroup,&contact[i]);
			dJointAttach(c,dGeomGetBody(o1), dGeomGetBody(o2));
		}
	}
}