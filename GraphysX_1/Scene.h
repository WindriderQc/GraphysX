#pragma once
#include "DynamicMesh.h"
#include "PushPlate.h"
#include "KinematicPrimitive.h"
#include "MeshPrimitive.h"
#include "Bullet.h"
#include "Joint.h"
#include "Human.h"
#include "MeshBillboard.h"
#include "Actor.h"
#include "Sky.h"
#include "Camera.h"
#include "TV3D65\tvAll.h"
#include "STSceneParam.h"


namespace GraphysX
{	
	class CLSpline3D;
	class PushPlate;

	class CLScene
	{
	public:
		CLScene();
		~CLScene(void);

		CLCamera clCamera;
		CLSky clSky;
	
		CTVLandscape* pLand;
		CTVCollisionResult* pClickCollision;
		CTVMesh* pMeshClicked;
		CTVMesh* pOldMeshClicked;
		
		GRAPHYSX_API void Render();
		GRAPHYSX_API void Update();

		GRAPHYSX_API void createDefaultLighting();
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
		std::vector<CLActor*> vActors;
		std::vector<CLDynamicMesh*> vDynPrim;  //   TODO: MEME NOM que le vector dans l'objet....  bad!!  enlever de l'objet ou avoir ici un pointeur sur le vector de l'objet.	
		std::vector<CLKinematicPrimitive*> vKinPrim;///  TODO:   Faudra vider ces vector au destruct scene!!!!!!!  ou est-ce qu'il se vide au delete de CLScene auto?
		std::map<CTVMesh*, CLMeshPrimitive*> mapMeshPrim;
	
	protected:
		int defaultLight = NULL;
		STSceneParam stSceneParam;
		NewtonWorld*	World;	
		void loadPhysmat();	
		void UpdatePhysics(); // float timestep);
		void Draw3DLines(void);
		
		void UpdateMouse3DPos();

		int loadFireCycleTex();
		
		
		GRAPHYSX_API static cTV_3DVECTOR Mouse3DPos;  // TODO:  check pkoi GRAPHYSX_API.... ?
	public:		
		GRAPHYSX_API Human* addSphereHuman(STEntityParam& stGeom, int cycleTex = NULL);
		GRAPHYSX_API CLKinematicPrimitive* addKinematicPrimitive(STEntityParam& stGeom);
		GRAPHYSX_API CLMeshPrimitive* addMeshPrimitive(STEntityParam& stGeom);
		GRAPHYSX_API CLMeshBillboard* addMeshBillboard(STEntityParam& stGeom);
		
		GRAPHYSX_API PushPlate* addPushplate(STEntityParam& stGeom, cTV_3DVECTOR forcepush = { 0, 0, 100.0f }, cTV_3DVECTOR forcepull = { 0, 0, 1.0f });
		GRAPHYSX_API CLDynamicMesh* addDynamicMesh( STEntityParam& stGeom);

		GRAPHYSX_API CLActor* CLScene::createCubX(STEntityParam& stGeom);
/*	//	GRAPHYSX_API CLMeshX* addMeshX(std::string filepath, STEntityParam& stGeom);*/

		GRAPHYSX_API CLMesh3DText* addMesh3DText(char* text, char* fontname, float fontsize, STEntityParam& stGeom, float extrude = 0.2f);
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
	
// BUILDER
		void BuildScene(STSceneParam Scene2Build);
		cTV_2DVECTOR BuildASCIIScene(std::string asciiFilePath);  // return the loaded map size
		void addFloor(int mapSize, std::string floorTextureName);
		void createSmoke();
		//void CreatePiston(float x, float z, float AngleY);
		void CreateVoieLactee(void);
		void CreateChain(cTV_3DVECTOR Pos, float fScale, const float angle, float masse, std::string texName);		
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
	
		CTVRenderSurface* pReflectRS;
		CTVRenderSurface* pRefractRS;
		CTVMesh* pWaterMesh;
		cTV_PLANE* pWaterPlane;

		float WaterHeight;
	};
}