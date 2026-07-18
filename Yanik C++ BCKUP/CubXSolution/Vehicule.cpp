#include "StdAfx.h"
#include "Vehicule.h"

       
        CLVehicule::CLVehicule(void)
        {
        }
		
		CLVehicule::CLVehicule(int CarID,  int LandPhysicMaterial)
        {
			pTVMaths = new CTVMathLibrary();
            pTVMaterialfactory = new CTVMaterialFactory();
            pTVTexturefactory = new CTVTextureFactory();
            pTVPhysics = new CTVPhysics();       
            pTVScene = new CTVScene();
			
            LoadTextures("Media\\Cars\\Textures\\Windows.bmp", "Media\\Cars\\Textures\\UnderCarriage.bmp",
									"Media\\Cars\\Textures\\ChassisSTI.bmp", "Media\\Cars\\Textures\\Wheel.bmp");
			
			InitMaterials(); 
			InitObjects("Media\\Cars\\chassis.tvm","Media\\Cars\\wheel_l.tvm","Media\\Cars\\wheel_r.tvm");     
			InitPhysicsMaterials(LandPhysicMaterial);

			iCarID = CarID;
			
		}
		CLVehicule::~CLVehicule(void)
		{ 
		
		}

        void CLVehicule::InitSound()
        {
            //Add code here
        }
		
        void CLVehicule::InitShaders()
        {
            //Add code here
        }
					
		void CLVehicule::LoadTextures(char* ChassisTex, char* WindowTex, char* UnderTex, char* WheelTex)
        {
            //Create Very Small Colored Texture For Windows
            texTintedWindows = pTVTexturefactory->CreateTexture(1, 1, true, "TintedWindows");
            pTVTexturefactory->SetPixel(texTintedWindows, 0, 0, RGBA256(190,190,190,255)); // Gray

            //Vehicle
            texChassisSTI = pTVTexturefactory->LoadTexture(ChassisTex, "ChassisSTI");
			texWindows = pTVTexturefactory->LoadTexture(WindowTex, "Windows");
			texUnderCarriage = pTVTexturefactory->LoadTexture(UnderTex, "UnderCarriage");
            texWheel = pTVTexturefactory->LoadTexture(WheelTex, "Wheel");
        }
        void CLVehicule::InitObjects(char* Chassis, char* WheelL, char* WheelR)
        {
            //Chassis
            m_chassis = pTVScene->CreateMeshBuilder("mChassis");
            m_chassis->LoadTVM(Chassis, false, false);
            m_chassis->SetShadowCast(true, true);
            m_chassis->SetTexture(texChassisSTI, 0);
            m_chassis->SetTextureEx(0, texWindows, 1);
            m_chassis->SetTextureEx(1, texTintedWindows, 1);
            m_chassis->SetTexture(texUnderCarriage, 2);
            m_chassis->SetMaterial(matWindow, 1);
            m_chassis->SetAlphaTest(true, 0, true, 1);
            m_chassis->SetBlendingMode(cTV_BLEND_ADD, 1);
            m_chassis->SetCullMode(cTV_DOUBLESIDED);
            m_chassis->SetShadowCast(true, true);

            //Front Left Wheel
            float scale = 1.0f;
            m_fl = pTVScene->CreateMeshBuilder("mfl");
            m_fl->LoadTVM(WheelL, true, true);
            m_fl->SetScale(scale, scale, scale);
            m_fl->SetLightingMode(cTV_LIGHTING_MANAGED);
            m_fl->SetMaterial(matWheels);
            m_fl->SetTexture(texWheel);
            m_fl->SetCullMode(cTV_DOUBLESIDED);
            m_fl->SetShadowCast(true, true);

            //Front Right Wheel
            m_rl = pTVScene->CreateMeshBuilder("mrl");
            m_rl->LoadTVM(WheelL, true, true);
            m_rl->SetScale(scale, scale, scale);
            m_rl->SetMaterial(matWheels);
            m_rl->SetLightingMode(cTV_LIGHTING_MANAGED);
            m_rl->SetTexture(texWheel);
            m_rl->SetCullMode(cTV_DOUBLESIDED);
            m_rl->SetShadowCast(true, false);
            m_rl->SetShadowCast(true, true);

            //Rear Left Wheel
            m_fr = pTVScene->CreateMeshBuilder("mfr");
            m_fr->LoadTVM(WheelR, true, true);
            m_fr->SetScale(scale, scale, scale);
            m_fr->SetMaterial(matWheels);
            m_fr->SetLightingMode(cTV_LIGHTING_MANAGED);
            m_fr->SetTexture(texWheel);
            m_fr->SetCullMode(cTV_DOUBLESIDED);
            m_fr->SetShadowCast(true, false);

            //Rear Right Wheel
            m_rr = pTVScene->CreateMeshBuilder("mrr");
            m_rr->LoadTVM(WheelR, true, true);
            m_rr->SetScale(scale, scale, scale);
            m_rr->SetMaterial(matWheels);
            m_rr->SetLightingMode(cTV_LIGHTING_MANAGED);
            m_rr->SetTexture(texWheel);
            m_rr->SetCullMode(cTV_DOUBLESIDED);
            m_rr->SetShadowCast(true, false);
            m_rr->SetShadowCast(true, true);

            m_chassis->SetLightingMode(cTV_LIGHTING_MANAGED);
            m_chassis->SetMaterial(matVehicleBody);
            m_chassis->ComputeNormals();
            m_chassis->ComputeBoundings();
            m_chassis->SetScale(scale, scale, scale);         

            //Add The Physics to the chassis
            pbi_chassis = pTVPhysics->CreateMeshBody(1500, m_chassis, cTV_BODY_CONVEXHULL); //1500
            pTVPhysics->SetAutoFreeze(pbi_chassis, false);
            pTVPhysics->SetBodyPosition(pbi_chassis, 0.0f, 8, 0.0f);
            pTVPhysics->SetBodyRotation(pbi_chassis, 0.0f, 0.0f, 0.0f);

            //Create The Vehicle
            car_ID = pTVPhysics->CreateVehicle(pbi_chassis);

            //Do Suspention Settings
            float susheight = 0.5f; //distance from chassis to wheel 0.5f
            float susplen = 1.0f; // 10
            float susshock = 40.0f; //Springiness of suspension 10
            float susspring = 300.0f; //Stiffness of suspension 400
            flw = pTVPhysics->AddVehicleWheelEx(car_ID, 25.0f, 0.5f * scale, 0.372f * scale, new cTV_3DVECTOR(1, 0, 0), -0.8f * scale, -susheight * scale, 1.25f * scale, 1, 0, 0, susplen, susshock, susspring, m_fl); //fl
            frw = pTVPhysics->AddVehicleWheelEx(car_ID, 25.0f, 0.5f * scale, 0.372f * scale, new cTV_3DVECTOR(1, 0, 0), 0.8f * scale, -susheight * scale, 1.25f * scale, 1, 0, 0, susplen, susshock, susspring, m_fr); //fr
            rlw = pTVPhysics->AddVehicleWheelEx(car_ID, 25.0f, 0.5f * scale, 0.372f * scale, new cTV_3DVECTOR(1, 0, 0), -0.8f * scale, -susheight * scale, -1.425f * scale, 1, 0, 0, susplen, susshock, susspring, m_rl); //rl
            rrw = pTVPhysics->AddVehicleWheelEx(car_ID, 25.0f, 0.5f * scale, 0.372f * scale, new cTV_3DVECTOR(1, 0, 0), 0.8f * scale, -susheight * scale, -1.425f * scale, 1, 0, 0, susplen, susshock, susspring, m_rr); //rr

            //Change the car's center of mass / make it drive better
            pTVPhysics->SetBodyCenterOfMass(car_ID, new cTV_3DVECTOR(0, -1.0f, 10.0f));

            //Add wheel frictions
            //Note that this code will also stop sliding on slopes
            float sideslip = 0.1f;
            float sideslipcoef = 0.0f;
            float maxlongslide = 10000.0f;
            float maxlongslidecoef = 0.0f;
            pTVPhysics->SetVehicleWheelParameters(car_ID, flw, sideslip, sideslipcoef, maxlongslide, maxlongslidecoef);
            pTVPhysics->SetVehicleWheelParameters(car_ID, frw, sideslip, sideslipcoef, maxlongslide, maxlongslidecoef);
            pTVPhysics->SetVehicleWheelParameters(car_ID, rlw, sideslip, sideslipcoef, maxlongslide, maxlongslidecoef);
            pTVPhysics->SetVehicleWheelParameters(car_ID, rrw, sideslip, sideslipcoef, maxlongslide, maxlongslidecoef);
		}
		
		void CLVehicule::InitPhysicsMaterials(int LandPhysicMaterial)
        {
            //Chassis   
            pmatChassis = pTVPhysics->CreateMaterialGroup("Chassis");
            pTVPhysics->SetMaterialInteractionFriction(pmatChassis, LandPhysicMaterial, 0.3f, 0.17f);
            pTVPhysics->SetMaterialInteractionBounciness(pmatChassis, LandPhysicMaterial, 0.1f);
            pTVPhysics->SetMaterialInteractionSoftness(pmatChassis, LandPhysicMaterial, 1000.0f);
            pTVPhysics->SetBodyMaterialGroup(pbi_chassis, pmatChassis);
        }
 
		void CLVehicule::InitMaterials()
        {
            //Create Materials
            matWindow = pTVMaterialfactory->CreateMaterial("CarWindows");
            matVehicleBody = pTVMaterialfactory->CreateMaterial("matVehicleBody");
            matWheels = pTVMaterialfactory->CreateMaterial("matWheels");
            
            //Car Windows
            pTVMaterialfactory->SetAmbient(matWindow, 1, 1, 1, 1);
            pTVMaterialfactory->SetDiffuse(matWindow, 1, 1, 1, 1);
            pTVMaterialfactory->SetSpecular(matWindow, 0.8f, 0.8f, 0.8f, 1);
            pTVMaterialfactory->SetPower(matWindow, 10);
            pTVMaterialfactory->SetOpacity(matWindow, 1.0f);

            //Vehicle Body
            pTVMaterialfactory->SetAmbient(matVehicleBody, 0.2f, 0.2f, 0.2f, 1);
            pTVMaterialfactory->SetDiffuse(matVehicleBody, 0.9f, 0.9f, 0.9f, 1.0f);
            pTVMaterialfactory->SetSpecular(matVehicleBody, 1.0f, 1.0f, 1.0f, 1);
            pTVMaterialfactory->SetPower(matVehicleBody, 100);
            pTVMaterialfactory->SetEmissive(matVehicleBody, 0, 0, 0.1f, 1);

            //Wheels
            pTVMaterialfactory->SetAmbient(matWheels, 0.8f, 0.8f, 0.8f, 1);
            pTVMaterialfactory->SetDiffuse(matWheels, 0.2f, 0.2f, 0.2f, 1.0f);
            pTVMaterialfactory->SetSpecular(matWheels, 0.2f, 0.2f, 0.2f, 1);
            pTVMaterialfactory->SetPower(matWheels, 200);
            pTVMaterialfactory->SetEmissive(matWheels, 0.0f, 0.0f, 0, 1);
        }


        void CLVehicule::Render(void)
        {    
		
             //Render Objets
             m_chassis->Render();
             m_fl->Render();
             m_fr->Render();
             m_rl->Render();
             m_rr->Render();
             //Render Transparent Objects
        }

	
        void CLVehicule::ManageInput(void)
        {
  
            //Accelerate and Brake
            //float CarPower = 3000; //3000
			stEngineCtrl.Power = 3000;

            if (pTVInput->IsKeyPressed(cTV_KEY_UPARROW)) //Accellerate
            {
				stEngineCtrl.Accel = true;
            }
            else
            {
               stEngineCtrl.Accel = false; 
               
			   if (pTVInput->IsKeyPressed(cTV_KEY_DOWNARROW))
                {
					stEngineCtrl.Brake = true;
                }
			   else
			   {
					stEngineCtrl.Brake = false;
			   }
            }

            //Steering
            if (pTVInput->IsKeyPressed(cTV_KEY_LEFTARROW))
            {
				stEngineCtrl.fSteerAngle -= 10;
                if (stEngineCtrl.fSteerAngle < -45)
                    stEngineCtrl.fSteerAngle = -45;
            }
            else
            {
                if (pTVInput->IsKeyPressed(cTV_KEY_RIGHTARROW))
                {
                    stEngineCtrl.fSteerAngle += 10;
                    if (stEngineCtrl.fSteerAngle > 45)
                        stEngineCtrl.fSteerAngle = 45;
                }
                else
                {
                    stEngineCtrl.fSteerAngle = 0;
                }
            }

            //Car Handbrake
            if (pTVInput->IsKeyPressed(cTV_KEY_RIGHTCONTROL))
            {
				stEngineCtrl.Handbrake = true;
            }
			else
			{
				stEngineCtrl.Handbrake = false;
			}


			//  Possiblement a mettre quelquye part d'autre.......
            if (pTVInput->IsKeyPressed(cTV_KEY_P))
            {
                pTVPhysics->VehicleReset(car_ID);
                pTVPhysics->SetBodyRotation(pbi_chassis, 0, 0, 0);
                pTVPhysics->SetBodyPosition(pbi_chassis, pTVPhysics->GetBodyPosition(pbi_chassis).x, pTVPhysics->GetBodyPosition(pbi_chassis).y + 0.2f, pTVPhysics->GetBodyPosition(pbi_chassis).z);
            }

            if (pTVInput->IsKeyPressed(cTV_KEY_O))
            {
                pTVPhysics->VehicleReset(car_ID);
                pTVPhysics->SetBodyPosition(pbi_chassis, 0.0f, 5, 0.0f);
                pTVPhysics->SetBodyRotation(pbi_chassis, 0.0f, 0.0f, 0.0f);
            }


SetEngineValues(stEngineCtrl);

	}

CTVMesh* CLVehicule::GetChassis(void)
{
	return(m_chassis);
}

void CLVehicule::SetPosition(cTV_3DVECTOR CarPos)
{
    pTVPhysics->VehicleReset(car_ID);
	pTVPhysics->SetBodyRotation(pbi_chassis, 0, 0, 0);
    pTVPhysics->SetBodyPosition(pbi_chassis, CarPos.x, CarPos.y + 0.2f, CarPos.z);
}








void CLVehicule::SetEngineValues(STEngineControl stEngCtrl)
{
  
            //Accelerate and Brake
           
            if (stEngCtrl.Accel)
            {
                pTVPhysics->SetVehicleWheelTorque(car_ID, rlw, stEngCtrl.Power, -1000);
                pTVPhysics->SetVehicleWheelTorque(car_ID, rrw, stEngCtrl.Power, -1000);
            }
            else
            {
                if (stEngCtrl.Brake)
                {
                    pTVPhysics->SetVehicleWheelTorque(car_ID, rlw, -stEngCtrl.Power, -1000);
                    pTVPhysics->SetVehicleWheelTorque(car_ID, rrw, -stEngCtrl.Power, -1000);
                }
                else
                {
                    pTVPhysics->SetVehicleWheelTorque(car_ID, flw, 0, -10000);
                    pTVPhysics->SetVehicleWheelTorque(car_ID, frw, 0, -10000);
                    pTVPhysics->SetVehicleWheelTorque(car_ID, rlw, 0, -10000);
                    pTVPhysics->SetVehicleWheelTorque(car_ID, rrw, 0, -10000);
                }
            }

            //Steering
            pTVPhysics->SetVehicleWheelSteering(car_ID, flw, stEngCtrl.fSteerAngle);
            pTVPhysics->SetVehicleWheelSteering(car_ID, frw, stEngCtrl.fSteerAngle);
            

            //Car Handbrake
			if (stEngCtrl.Handbrake)
            {
                pTVPhysics->VehicleWheelHandBrake(car_ID, rlw, 1, 2000);
                pTVPhysics->VehicleWheelHandBrake(car_ID, rrw, 1, 2000);
            }
}
