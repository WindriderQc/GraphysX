 #pragma once
 #include "StdAfx.h"
 

struct STEngineControl
	{
		float Power;
		bool Accel;
		bool Brake;
		float fSteerAngle;
		bool Handbrake;
	};

 
 class CLVehicule 
 {
    public:   
			CLVehicule(void);
			CLVehicule(int CarID, int LandPhysicMaterial);  // Pour creer d'autre Car dans la scene
			~CLVehicule(void);
	
			void InitSound(void);
            void InitShaders(void);
            void InitObjects(char* Chassis, char* WheelL, char* WheelR);
            void Render(void);
			void ManageInput(void);
			void LoadTextures(char* ChassisTex, char* WindowTex, char* UnderTex, char* WheelTex);
			void InitPhysicsMaterials(int LandPhysicMaterial);
			void InitMaterials();

			CTVMesh* GetChassis(void);
			void SetPosition(cTV_3DVECTOR CarPos);
			void SetEngineValues(STEngineControl stEngCtrl);
			int iCarID;

			


	protected:		

			STEngineControl stEngineCtrl;
			
			//Materials
			int matVehicleBody;
			int matWindow;
			int matWheels;

			// Car
			CTVMesh* m_chassis;
			CTVMesh* m_fl;
			CTVMesh* m_fr;
			CTVMesh* m_rl;
			CTVMesh* m_rr;
			int pbi_chassis;
			int flw;
			int frw;
			int rlw;
			int rrw;
			int car_ID;
			float steerAngle;
			int texWindows, texUnderCarriage, texChassisSTI, texWheel, texTintedWindows;

			int pmatChassis;
			
			CTVScene* pTVScene;   
			CTVCamera* pTVCamera;
			CTVInputEngine* pTVInput;			
			CTVMathLibrary* pTVMaths;                 
			CTVTextureFactory* pTVTexturefactory;     
			CTVMaterialFactory* pTVMaterialfactory;                      
	        CTVPhysics* pTVPhysics;
};