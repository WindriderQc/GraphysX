#pragma once
#include "TV3DMoteur.h"
#include "dHighResolutionTimer.h"
#include "Camera.h"
#include "DynamicMesh.h"
#include "PushPlate.h"
#include "KinematicPrimitive.h"
#include "MeshPrimitive.h"
#include "Bullet.h"
#include "Joint.h"
#include "Human.h"
#include "ZombieKiller.h"
#include "PushPlate.h"
#include "MeshBillboard.h"


#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846

#define SUN_ORBIT_RADIUS 1000


namespace GraphysX
{
	class CLSpline3D;

	class CLScene
	{
	public:
		CLScene();
		~CLScene(void);

		CLCamera clCamera;
	
		CTVLandscape* pLand;
		CTVCollisionResult* pClickCollision;
		CTVMesh* pMeshClicked;
		CTVMesh* pOldMeshClicked;
		
		GRAPHYSX_API void Render();
		GRAPHYSX_API void Update(float fTimeElapsed);

		GRAPHYSX_API void mouseClick(int x, int y);
		GRAPHYSX_API void ClearScene();
		GRAPHYSX_API NewtonWorld* GetWorld() { return(World); }
		GRAPHYSX_API static cTV_3DVECTOR getMouse3DPos();
		GRAPHYSX_API void MouseHighlight();  //  TODO :  devrait etre une function de TV3DMOteur non?
		GRAPHYSX_API cTV_3DVECTOR getDefaultLightPos();
		GRAPHYSX_API int addLight(cTV_LIGHT light, char* name = "light", bool meshmark = true, bool enable = true, bool castShadows = true);
		GRAPHYSX_API void createFire(cTV_3DVECTOR pos);
		GRAPHYSX_API CLSpline3D* add3DSpline(Spline spline, bool isVisible);
		std::vector<sLine3D> vecLine3D; // stock tous les lignes a etre dessiné dans le refresh 2D
		int cycleTexFire;

		//std::vector<std::unique_ptr<CLEntity3D>> vEntity;
		std::vector<CLEntity3D*> vEntity;
		std::vector<CLDynamicMesh*> vDynPrim;  //   MEME NOM que le vector dans l'objet....  bad!!  enlever de l'objet ou avoir ici un pointeur sur le vector de l'objet.	
		std::vector<CLKinematicPrimitive*> vKinPrim;///    Faudra vider ces vector au destruct scene!!!!!!!  ou est-ce qu'il se vide au delete de CLScene auto?
		std::map<CTVMesh*, CLMeshPrimitive*> mapMeshPrim;
	
	protected:
		int defaultLight = NULL;
		// Atmosphere
		int iIndiceSunlight, iIndiceMoonlight;
		int iSunOrbitYOffset;
		//These indices are store to avoid the lookup costs
		int texDayUp, texDayDown, texDayLeft, texDayRight, texDayFront, texDayBack;
		int texNightUp, texNightDown, texNightLeft, texNightRight, texNightFront, texNightBack;
		int texSun;

		int GetSunOrbitYOffset(void)        { return(iSunOrbitYOffset);     }
		void SetSunOrbitYOffset(int Offset) {  iSunOrbitYOffset = Offset;	}
		void UpdateRenderAtmos();

		long lTimeOfDay;

		// physics 
		NewtonWorld*	World;
		dFloat m_physicsTime = 0.0f;
		unsigned64 m_microsecunds = 0;
		void UpdatePhysics(); // float timestep);
		void Draw3DLines(void);
		
		
		void UpdateMouse3DPos();

		int loadFireCycleTex();
		
		
		GRAPHYSX_API static cTV_3DVECTOR Mouse3DPos;  // TODO:  check pkoi GRAPHYSX_API.... ?
	public:		
		GRAPHYSX_API Human* addSphereHuman(STMeshParam& stGeom, float masse = 1.0f, int tex = NULL);
		GRAPHYSX_API CLKinematicPrimitive* addKinematicPrimitive(std::string texName, STMeshParam& stGeom, ePHYSMAT physMat);
		GRAPHYSX_API CLMeshPrimitive* addMeshPrimitive(std::string texName, STMeshParam& stGeom);
		GRAPHYSX_API CLMeshBillboard* addMeshBillboard(STMeshParam& stGeom, std::string texName);
		
		GRAPHYSX_API PushPlate* addPushplate(std::string texName, STMeshParam& stGeom, ePHYSMAT physMat, cTV_3DVECTOR forcepush = { 0, 0, 100.0f }, cTV_3DVECTOR forcepull = { 0, 0, 1.0f }, float masse = 1.0f);

		GRAPHYSX_API CLDynamicMesh* addDynamicMesh( STMeshParam& stGeom, ePHYSMAT physMat, std::string texName = "", float masse = 0.0f);

	//	GRAPHYSX_API CLMeshX* addMeshX(std::string filepath, STMeshParam& stGeom);

		GRAPHYSX_API CLMesh3DText* addMesh3DText(char* text, char* fontname, float fontsize, STMeshParam& stGeom, float extrude = 0.2f);
		GRAPHYSX_API Bullet* shotBullet(std::string texName, cTV_3DVECTOR pos, cTV_3DVECTOR dir, float offset, float diameter, ePHYSMAT physMat, float speed, float masse, int lifetime);

		GRAPHYSX_API void setPhysmatElasticity(ePHYSMAT mat1, ePHYSMAT mat2, float elast);
		GRAPHYSX_API void setPhysmatFriction(ePHYSMAT mat1, ePHYSMAT mat2, float staticFric, float kineticFric);
		GRAPHYSX_API void setPhysmatCollisionCallback(ePHYSMAT mat1, ePHYSMAT mat2, void* collisionCallback);

		GRAPHYSX_API void setMaterialToPhysEntity(NewtonBody* body, ePHYSMAT material);
		
		GRAPHYSX_API CLMeshPrimitive* addPrimSphere(std::string name, cTV_3DVECTOR pos, float radius, std::string textureName = "");
		GRAPHYSX_API CLMeshPrimitive* addPrimCube(std::string name, cTV_3DVECTOR pos, cTV_3DVECTOR scale, std::string textureName = "");
		GRAPHYSX_API CLMeshPrimitive* addPrimCylinder(std::string name, cTV_3DVECTOR pos, float radius, float height, std::string textureName = "");

		GRAPHYSX_API CL6DOFJoint* CLScene::Create6DOFJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, dVector LimitsMinAngle, dVector LimitsMaxAngle);
		GRAPHYSX_API CLSlidingJoint* CreateSlidingJoint(NewtonBody* ParentBody, NewtonBody* ChildBody, float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit);
	};




	class CLWater   // TODO : a revoir...  peut définitivement etre improved ou assimilé par Scene
	{
	public:
		CLWater(void);
		~CLWater(void);

		void StartReflectRender(void);
		void StopReflectRender(void);
		void StartRefractRender(void);
		void StopRefractRender(void);
		void Render(void);

	protected:
		CTVScene* pTVScene;
		CTVTextureFactory* pTVTexFactory;

		CTVGraphicEffect* pTVGraphEffect;
		CTVRenderSurface* pReflectRS;
		CTVRenderSurface* pRefractRS;
		CTVMesh* pWaterMesh;
		cTV_PLANE* pWaterPlane;

		float WaterHeight;
	};
}