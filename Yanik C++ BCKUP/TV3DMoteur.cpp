#include "TV3DMoteur.h"
#include <stdio.h>


// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float      

//LPDIRECT3DDEVICE9* d3Device;
//LPDIRECT3DDEVICE9 Test;

/*Constructeur de la classe*/
CLTV3DMoteur::CLTV3DMoteur(void)
{
 bFULLSCREEN = 0;    //   a faire: un .H commun avec tout les bool de config.
 bDEBUG_MODE = 0;   // Show la pos de la cam et le speed factor dans le status bar en bas de la fenetre...
 bSHOW_FPS = 1; 
 //bMOUSE_CAM_CTRL = 0;
 //bCAM_CTRL = 1;
 bTVSHADOWS = 1;
 bCEGUI_ACTIVE = 0;
 bDRAW_AXIS = 0;
 bENABLE_ATMOSPHERE = 0;
 bGAME_MODE = 0;

	CamPos.x = 1000;
	CamPos.y = 250;
	CamPos.z = -3250;		
	CamLookAt.x = 0;
	CamLookAt.y = 200;
	CamLookAt.z = 0;



	clAtmos = NULL;
	clCEGUI = NULL;
	clMenuManager = NULL;
	Snowfall = NULL;
	SmokeFest = NULL;
	clExplosion = NULL;
	clBallZ = NULL;

	for(int i=0;i<MAX_PHYSIC_MESH;i++)
	  {
	  MeshTable[i] = NULL;
	  }

}

/*Destructeur de la classe*/
CLTV3DMoteur::~CLTV3DMoteur(void)
{
 // Clean up the scene 
    if( pScene ) 
    { 
      pScene->DestroyAllMeshes(); 
      delete(pScene); pScene = NULL; 
    } 
 
	if(clInput){ delete(clInput); clInput = NULL; }
	if(clAtmos){ delete(clAtmos); clAtmos = NULL; }
	if(clCEGUI){ delete(clCEGUI); clCEGUI = NULL; }
	//if(Snowfall){ delete(Snowfall); Snowfall = NULL; }
	//if(SmokeFest){ delete(SmokeFest); SmokeFest = NULL; }
	if(clMenuManager){ delete(clMenuManager); clMenuManager = NULL; }
	if(LightTimeOn) { delete(LightTimeOn); LightTimeOn = NULL; }
	if(LightTimeOff) { delete(LightTimeOff); LightTimeOff = NULL; }
	if(FanTimeOn) { delete(FanTimeOn); FanTimeOn = NULL; }
	if(FanTimeOff) { delete(FanTimeOff); FanTimeOff = NULL; }
	if(ClockDisplay) { delete(ClockDisplay); ClockDisplay = NULL; }

	if(clExplosion) { delete (clExplosion); clExplosion = NULL; }
	if(clBallZ) { delete (clBallZ); clBallZ = NULL; }


    // Destroy the engine last 
    if( pTV ) { delete(pTV); pTV = NULL; } 		
}


void CLTV3DMoteur::Init3D(HWND WindowHandle, char *srchpath)
{
// Create the pTV Interface first:
	pTV = new CTVEngine();

	pTV->SetSearchDirectory(srchpath);
	// Set the debug file/options.
	// Do this before the 3D init so it can log any errors found during init.
	pTV->SetDebugMode(true, true);
	pTV->SetDebugFile(strcat(srchpath, "\\debugfile.txt"));

	// Its time to init the engine:
	pTV->Init3DWindowed(WindowHandle, true);

	// Something good to do is to enable the auto-resize feature:
	// Get the default viewport and set autoresize to true for it:
	pTV->GetViewport()->SetAutoResize(true);

	// Lets display the FPS:
	if(bSHOW_FPS)
		pTV->DisplayFPS(true);

	// Set the prefered angle system:
	pTV->SetAngleSystem(cTV_ANGLE_DEGREE);

	GREEN = RGBA256(0,255,0,255);  // A vérifier et ajouter autre couleur 
	RED = RGBA256(255,0,0,255);
	BLUE = RGBA256(0,0,255,255);  
	WHITE = RGBA256(255,255,255,255);
	BLACK = RGBA256(0,0,0,255);

	// Now after we are done initializing the TVEngine component lets continue:
	// Create any other components after pTV init->
	pScene = new CTVScene();
	pLights = new CTVLightEngine();
	pTexFactory = new CTVTextureFactory();
	p2DImmediate = new CTVScreen2DImmediate();
	p2DText = new CTVScreen2DText();
	//pGlobals = new CTVGlobals();
	
	//pIntObj = new CTVInternalObjects();
	pTVMath = new CTVMathLibrary();

	clInput = new CInput();
	clInput->bCAM_CTRL_ENABLED = 1;

	pScene->SetViewFrustum(60, 7000);
	SetCamera();
	
	InitLight();
	LoadTextures();

	if(bENABLE_ATMOSPHERE)
		clAtmos = new CLAtmosphere();
	
	Create3DObjects();
	//  Load the 3D Font
	LoadLettersAndNumbers();
	
	// Locate the clock display
	DisplayPos.x = 600; DisplayPos.y = 975; DisplayPos.z = 0;
	ClockDisplay = new CLClockDisplay(DisplayPos,3,0,RGBA256(0,0,255,255), st3DFont);

	//  Doit etre apres la création de ClockDisplay sinon affiche pas...!?!?!?!?   Pkoi?
	CreateMenuDomotique();  // Load les 4 Clock Displays



// Initialize particle engines:
    // Snow: 50 particles, size=1000x1000 units at y=100, ground at y=0
/*	cTV_3DVECTOR SnowfallOrg;
	SnowfallOrg.x = 0;
	SnowfallOrg.y = 1000;
	SnowfallOrg.z = 0;
    Snowfall = new TSnowfall(100, SnowfallOrg, 5000, 5000, 0, SnowTexture); // rajouter la size des particles( billboard)
    Snowfall->ResetSystem();

	cTV_3DVECTOR SmokeFestOrg;
	SmokeFestOrg.x = -300;
	SmokeFestOrg.y = 0;
	SmokeFestOrg.z = 0;
    SmokeFest = new CLSmoke(100, SmokeFestOrg, 250, SnowTexture); // rajouter la size des particles( billboard)
    SmokeFest->ResetSystem();

*/


clMenuManager = new CLMenuManager();
SetMenuButtons();

clExplosion = new CLExplosion();

	if(bCEGUI_ACTIVE)
	{
		clCEGUI = new CL_TV3DCEGUI();
		clCEGUI->InitCEGUI();  // Initialisation du GUI
	}

}

//Render sub
void CLTV3DMoteur::Render(float TimeElapsed)
{
	int i = 0;
	char Buffer[256];

		Refresh3DClock();
		

		pTV->Clear(false);

		if(bENABLE_ATMOSPHERE)
		{
			clAtmos->UpdateAndRender(TimeElapsed);
			//char* Time = clAtmos->lTimeOfDay;
			sprintf(Buffer, "%d", clAtmos->lTimeOfDay);
		}
		
		//pSoleil->Enable(false);
		//pSoleil->SetAlphaTest(true,128,true,-1);
		
		pSoleil->Render();

		clMenuManager->ShowActiveMenu(pScene); 

		ClockDisplay->Render();
		LightTimeOn->Render();
		LightTimeOff->Render();
		FanTimeOn->Render();
		FanTimeOff->Render();

		if(bGAME_MODE == 1)
			clBallZ->Render();
		
		if(clExplosion->IsFinished() == false)
			clExplosion->Render(TimeElapsed);


		float SpeedFactor = TimeElapsed / 1000; // pour avoir un speedfactor en seconde
		float Time = SpeedFactor / 1000;
	/*	Snowfall->UpdateSystem(Time);  // si ca va mal, changer speedfactor pour enlever le /1000
		Snowfall->RenderSystem();
		SmokeFest->UpdateSystem(Time);  // si ca va mal, changer speedfactor pour enlever le /1000
		SmokeFest->RenderSystem();*/
	
		//pScene->RenderAll(true,false);

	if(bDRAW_AXIS)
	{
		DrawMeshAxis(pCube[0]);	
		DrawTV3DAxis();
	}
	
	p2DText->Action_BeginText(false);
	p2DText->NormalFont_DrawText("Atmel Card Control Interface with TV3D V0.1a - C++ by YB",5,20,RGBA(1,1,1,1),0);
	p2DText->NormalFont_DrawText("X = White, Y = Green, Z = Red",5,32,RGBA(1,1,1,1),0);
	if(bENABLE_ATMOSPHERE)
	{
		p2DText->NormalFont_DrawText("Time of Day: ",5,44,RGBA(1,1,1,1),0);
		p2DText->NormalFont_DrawText(Buffer,85,44,RGBA(1,1,1,1),0);	
	}
	p2DText->Action_EndText();

/*
if(bCEGUI_ACTIVE)
	clCEGUI->Render();   // vérifier si dois etre apres le call de RenderToScreen
	//pD3Ddevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);     A checker pour fixer bug d'affichage
*/
	pTV->RenderToScreen();
}

void CLTV3DMoteur::SetCamera(void)
{
    pScene->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);
}

void CLTV3DMoteur::LoadTextures(void)
{
	SoleilTex = pTexFactory->LoadTexture("Media\\gold_01.jpg", "SoleilTex", -1,-1,cTV_COLORKEY_MAGENTA,true);
	//TempSensTexture = pTexFactory->LoadTexture("Media\\SensTempGREEN.bmp", "SensTemp", -1,-1,cTV_COLORKEY_MAGENTA,true);
	//CursorTexture = pTexFactory->LoadTexture("Media\\AimCursor.bmp", "CursorTex", -1,-1,cTV_COLORKEY_MAGENTA,true);
	ClockTexture = pTexFactory->LoadTexture("Media\\gold_01.jpg","ClockTex",-1,-1,cTV_COLORKEY_NO,true);
	MarbleTexture = pTexFactory->LoadTexture("Media\\marble10.jpg","MarbleTex",-1,-1,cTV_COLORKEY_NO,true);
    EarthTexture = pTexFactory->LoadTexture("Media\\Earth.jpg","EarthTex",-1,-1,cTV_COLORKEY_NO,true);
	//PlasticTexture = pTexFactory->LoadTexture("Media\\Soft_Flat.jpg","PlasticTex",-1,-1,cTV_COLORKEY_NO,true);
	CloudsTexture = pTexFactory->LoadTexture("Media\\Clouds.jpg","CloudsTex",-1,-1,cTV_COLORKEY_BLACK,true);
	//TexturePanneauMenu = pTexFactory->LoadTexture("Media\\gold_01.jpg","CloudsTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBAlarms = pTexFactory->LoadTexture("Media\\AlarmClockIcon.jpg","AlarmsTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBSystem = pTexFactory->LoadTexture("Media\\SystemIcon.jpg","SystemTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBTemp = pTexFactory->LoadTexture("Media\\TemperatureMonitorIcon.jpg","TempTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBAtmel = pTexFactory->LoadTexture("Media\\memoryChip.jpg","AtmelTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBTools = pTexFactory->LoadTexture("Media\\ToolsIcon.png","ToolsTex",-1,-1,cTV_COLORKEY_NO,true);
	//ClosedIconTex = pTexFactory->LoadTexture("Media\\ClosedIcon.bmp","ClosedIconTex",-1,-1,cTV_COLORKEY_WHITE,true);
	//OpenedIconTex = pTexFactory->LoadTexture("Media\\OpenedIcon.bmp","OpenedIconTex",-1,-1,cTV_COLORKEY_WHITE,true);
//	RedLightTrailTex = pTexFactory->LoadTexture("Media\\RedLight.jpg","RedLightTrailTex",-1,-1,cTV_COLORKEY_BLACK,true);
	EarthGridTex = pTexFactory->LoadTexture("Media\\EarthGridXL.bmp","EarthGridTex",-1,-1,cTV_COLORKEY_BLACK,true);
	SnowTexture = pTexFactory->LoadTexture("Media\\Snow.dds","SnowTex",-1,-1,cTV_COLORKEY_BLACK,true);

}

void CLTV3DMoteur::Create3DObjects(void)
{
    int i;
	
	// variable pour permettre de detecter les bounding box   --  Pas encore utilisé
	//cTV_3DVECTOR BoxMin;
	//cTV_3DVECTOR BoxMax;

	// A faire pour tout les mesh pour aider la vitesse en plus de desactiver les alcul de collision si pas nécessaire.
	//->ComputeBoundingVolumes();   // test pour faire marcher la calisse de collision testing
		
//   testing purpose
	pSoleil = pScene->CreateMeshBuilder("SoleilMesh");
	pSoleil->CreateBox(20,20,20, false);
	pSoleil->SetPosition(0,100,0);
	pSoleil->SetLightingMode(cTV_LIGHTING_MANAGED);
	pSoleil->SetColor(RGBA(1,0,0,1),true);

	//pSoleil->SetPosition(0,0,0);
	//pSoleil->Enable(false);
	//pSoleil->SetColor(RGBA(0,1,0,1),true);
	//pSoleil->SetCullMode(cTV_BACK_CULL);
	//pSoleil->ComputeNormals();
	//pSoleil->ComputeOctree();
	//pSoleil->Enable(true);


	/*	pClouds = pScene->CreateMeshBuilder("CloudsMesh");
	pClouds->CreateSphere(NULL,225,64,64,pGlobals->RGBA256(255,255,255,255));	
	pClouds->SetPosition(800,-100,250);
	pClouds->SetTexture(CloudsTexture,-1);
*/

	//  Comment avoir un dGeom de ODE a la forme du pLand fait par clAtmos????
	pGround = pScene->CreateMeshBuilder("Ground");
	pGround->CreateBox(3000,2,3000,false);
	pGround->SetPosition(0,-1.5,0);
	pGround->SetTexture(EarthTexture,-1);
	MeshTable[0] = pGround;					// Ground = Objet #0


	//pBille = pScene->CreateMeshBuilder("Bille");	
	//pBille->CreateSphere(SoleilTex,100,8,8,RGBA(1.0 ,1.0 ,1.0 ,1.0));	  
	//pBille->SetPosition(1000,500,1000);
	//MeshTable[1] = pBille;					// Bille = Objet #1


/*
	 pBoite = pScene->CreateMeshBuilder("Boite");
	// pBoite->LoadSkinMesh("boite.x");//
	 //pBoite->LoadXFile("Boite.x",true,true);
	 pBoite->LoadXFileHierarchy("Media\\Boite.x",true,false);
	 //pBoite->Load3DSMesh("boite.3ds", false, true, false,true,true);
	 pBoite->SetPosition(800,-100,250);
	 pBoite->ScaleMesh(5,5,5);
	 pBoite->Enable(true);
/*
	pGear = pScene->CreateMeshBuilder("Gear");
	pGear->Load3DSMesh("Media\\Gears.3ds", false, true, false,true,true);
	pGear->SetPosition(300,600,300);
	pGear->ScaleMesh(8,8,8);
	pGear->RotateY(90,true);
	pGear->SetTexture(GearTexture,-1);

	pSwitch = pScene->CreateMeshBuilder("Switch");
	pSwitch->Load3DSMesh("Media\\LightSwitch.3ds", false, true, false,true,true);
	pSwitch->SetPosition(1000,600,300);
	pSwitch->ScaleMesh(65,65,65);
	pSwitch->RotateZ(90,true);
	pSwitch->RotateX(90,true);
	pSwitch->RotateY(90,true);
	pSwitch->SetTexture(PlasticTexture,-1);
	pSwitch->SetMaterial(pGlobals->GetMat("matname"), -1);
	clMenuManager.CreateBouton(pSwitch, 1);
*/

/*	pConfigBtn = pScene->CreateMeshBuilder("ConfigBtn");
	pConfigBtn->LoadXFileHierarchy("ConfigBtn.x",false,true);
	pConfigBtn->SetPosition(1000,600,0);
	//pConfigBtn->RotateX(90,true);
	//pConfigBtn->RotateY(90,true);
	//pConfigBtn->RotateZ(90,true);
	pConfigBtn->ScaleMesh(10,10,10);
	//clMenuManager.CreateBouton(pConfigBtn,1);

*/
	

///// Section Menu niveau 0 - Main

	pCube[0] = pScene->CreateMeshBuilder ("Cube0");
	pCube[0]->LoadTVM("media\\cubx.tvm",true,true);
	pCube[0]->SetScale(12,12,12);
	
	pCube[1] = pScene->CreateMeshBuilder ("Cube1");
	pCube[1]->CreateBox(200,200,200,false);
	//pCube[1]->SetMaterial(pGlobals->GetMat("matname"), -1);
	pCube[1]->SetTexture(TexBSystem, -1);

	pCube[2] = pScene->CreateMeshBuilder ("Cube2");
	pCube[2]->CreateBox(200,200,200,false);
	//pCube[2]->SetMaterial(pGlobals->GetMat("matname"), -1);
	pCube[2]->SetTexture(TexBTools, -1);

	for(i = 0;i<3;i++)
	{
	//pCube[i]->SetShadowCast(true,true);  
	//pCube[i]->SetCullMode(TV_FRONT_CULL);
	pCube[i]->SetPosition(-500,500-i*250,0);
	pCube[i]->SetLightingMode(cTV_LIGHTING_MANAGED);
	}

	pEarth = pScene->CreateMeshBuilder("EarthMesh");
	pEarth->CreateSphere(290,32,32);
	//pEarth->ShowBoundingBox(true,-256);
	//pEarth->SetMeshCenter(0,0,0);
	pEarth->SetPosition(800,-100,250);
	pEarth->SetLightingMode(cTV_LIGHTING_MANAGED);
	pEarth->SetTexture(EarthTexture,-1);
	//pEarth->SetMaterial(pGlobals->GetMat("matname"), -1);


    pEarthGrid = pScene->CreateMeshBuilder("EarthMesh1");
	pEarthGrid->CreateSphere(310,64,64);	
	pEarthGrid->SetPosition(800,-100,250);
	pEarthGrid->SetLightingMode(cTV_LIGHTING_MANAGED);
	//pEarthGrid->Enable(true);
	pEarthGrid->SetTexture(EarthGridTex,-1);
	
	pFleche = pScene->CreateMeshBuilder("Fleche");
	pFleche->LoadTVM("media\\fleche.tvm",true,true);//CreateBox(10,10,10,false);//Load3DSMesh("Fleche.3ds", true, true, false,false,false);
	pFleche->SetPosition(1000,600,300);
	pFleche->SetScale(1,1,1);
	//pFleche->RotateZ(90,true);
	//pFleche->RotateX(90,true);
	pFleche->RotateY(180,true);
	pFleche->SetLightingMode(cTV_LIGHTING_MANAGED);
	pFleche->SetTexture(MarbleTexture,-1);
	//pFleche->SetMaterial(pGlobals->GetMat("matname"), -1);


///// Section Menu niveau 1 - Atmel ctrl
/*	for(i = 0; i<8;i++)
	{
		pIOButton[i] = pScene->CreateBillboard(ClosedIconTex, (i*150)-500,750,0,206,205,"IOIcon", true);
		pIOButton[i]->Enable(false);  	
	}
*/
	
	pMenuDomotique = pScene->CreateMeshBuilder("MenuDomo");
	pMenuDomotique->LoadTVM("Media\\MenuDomo.TVM",true,true);
	pMenuDomotique->SetPosition(-950, 400, 0);
	pMenuDomotique->SetScale(8,8,8);
	pMenuDomotique->SetLightingMode(cTV_LIGHTING_MANAGED);
	pMenuDomotique->SetTexture(ClockTexture,-1);
//	pMenuDomotique->SetMaterial(pGlobals->GetMat("matname"), -1);
	pMenuDomotique->Enable(false);

	LightEtat = pScene->CreateMeshBuilder("LightMesh");
	LightEtat->CreateSphere(64,64,64);	
	LightEtat->SetPosition(-1250, 800, 0);
	LightEtat->SetLightingMode(cTV_LIGHTING_MANAGED);
	//LightEtatOn->SetTexture(EarthTexture,-1);
	LightEtat->SetColor(RGBA(0,1,1,1), true);
	LightEtat->Enable(false);

	FanEtat = pScene->CreateMeshBuilder("FanMesh");
	FanEtat->CreateSphere(64,64,64);	
	FanEtat->SetPosition(-1250, 300, 0);
	FanEtat->SetLightingMode(cTV_LIGHTING_MANAGED);
	FanEtat->SetColor(RGBA(0,1,1,1), true);
	FanEtat->Enable(false); 



/////  Section Menu niveau 2 - SysInfo
	pInetConnected = pScene->CreateMeshBuilder("InetConnected"); 
	pInetConnected->CreateSphere(60,64,64); //Rouge
	pInetConnected->SetPosition(-500,-250,0);
	pInetConnected->SetLightingMode(cTV_LIGHTING_MANAGED);
	pInetConnected->Enable(false);

}


void CLTV3DMoteur::CreateMenuDomotique(void)
{
int iSizeDisplay = 1; 

DisplayPos.x = -400; 
DisplayPos.z = 0;

DisplayPos.y = 850; 
LightTimeOn = new CLClockDisplay(DisplayPos,iSizeDisplay,0, BLUE, st3DFont);  
LightTimeOn->iTime[0]=1; LightTimeOn->iTime[1]=1; LightTimeOn->iTime[2]=1; LightTimeOn->iTime[3]=1;
LightTimeOn->WriteTime(ClockTexture, NULL); 
LightTimeOn->HideDisplay();
	
DisplayPos.y = 700; 
LightTimeOff = new CLClockDisplay(DisplayPos,iSizeDisplay,0, BLUE, st3DFont);
LightTimeOff->iTime[0]=1; LightTimeOff->iTime[1]=1; LightTimeOff->iTime[2]=1; LightTimeOff->iTime[3]=1;
LightTimeOff->WriteTime(ClockTexture, NULL); 
LightTimeOff->HideDisplay();

DisplayPos.y = 350; 
FanTimeOn = new CLClockDisplay(DisplayPos, iSizeDisplay, 0, BLUE, st3DFont);
FanTimeOn->iTime[0]=1; FanTimeOn->iTime[1]=1; FanTimeOn->iTime[2]=1; FanTimeOn->iTime[3]=1;
FanTimeOn->WriteTime(ClockTexture, NULL); 
FanTimeOn->HideDisplay();
	
DisplayPos.y = 200;
FanTimeOff = new CLClockDisplay(DisplayPos, iSizeDisplay ,0, BLUE, st3DFont);
FanTimeOff->iTime[0]=1; FanTimeOff->iTime[1]=1; FanTimeOff->iTime[2]=1; FanTimeOff->iTime[3]=1;
FanTimeOff->WriteTime(ClockTexture, NULL); 
FanTimeOff->HideDisplay();

//////////////////////////////




}
void CLTV3DMoteur::LoadLettersAndNumbers(void)
{
int i;

char String[22]="Media\\alphabet\\ .tvm";


//  Load l'alphabet A-Z
for(i = 0; i < 26; i++)
  {
    String[15]=char(i) + 0x41;  // ajustement selon la table ascii
	st3DFont.Alphabet[i]= pScene->CreateMeshBuilder("Alphabet");
	st3DFont.Alphabet[i]->LoadTVM(String,true,true);	
	st3DFont.Alphabet[i]->RotateX(-90,true);
	//Alphabet[i]->SetMaterial(pTVGlobals->GetMat("matname"), -1);
	st3DFont.Alphabet[i]->Enable(false);
  }

// Load les chiffres  0-9
for(i=0;i<10;i++)
  {
	String[15] = char(i) + 0x30;    // ajustement selon la table ascii
	st3DFont.Numbers[i]= pScene->CreateMeshBuilder("Numbers");
	st3DFont.Numbers[i]->LoadTVM(String,true,true);
	st3DFont.Numbers[i]->RotateX(-90,true);
	//Numbers[i]->SetMaterial(pTVGlobals->GetMat("matname"), -1);
	st3DFont.Numbers[i]->Enable(false);
  }

st3DFont.SpecialChar[0] = pScene->CreateMeshBuilder("SpecialChar");   // A rajouter : Un vrai ':' et modifier inittime pour l'utiliser.
st3DFont.SpecialChar[0]->CreateSphere(10,8,8);
//SpecialChar[0]->SetMaterial(pTVGlobals->GetMat("matname"), -1);
st3DFont.SpecialChar[0]->Enable(false);


}

void CLTV3DMoteur::ShowTimeDisplays(void)
{
LightTimeOn->ShowDisplay();
LightTimeOff->ShowDisplay();
FanTimeOn->ShowDisplay();
FanTimeOff->ShowDisplay();
}

void CLTV3DMoteur::HideTimeDisplays(void)
{
LightTimeOn->HideDisplay();
LightTimeOff->HideDisplay();
FanTimeOn->HideDisplay();
FanTimeOff->HideDisplay();
}

void CLTV3DMoteur::Refresh3DClock(void)
{
	SYSTEMTIME st;
	
	GetLocalTime(&st);
	ClockDisplay->iTime[0] = (st.wHour) /10;
	ClockDisplay->iTime[1] = (st.wHour) % 10; 
	ClockDisplay->iTime[2] = st.wMinute/10;
	ClockDisplay->iTime[3] = st.wMinute % 10;	
	
	// Desing pas propre car acces a toute les fonction de CLFontNClock avec l'objet.  Et les texture non pas d'effet, en commentaire.	
	ClockDisplay->WriteTime(ClockTexture, NULL);  
}


void CLTV3DMoteur::InitLight(void)
{ 
	cTV_3DVECTOR VecDir;
	pLights->SetGlobalAmbient(0,0,0);

	VecDir.x = -1; VecDir.y = -1; VecDir.z = 1;
	pLights->CreateDirectionalLight(&VecDir,1,1,1,"GlobalLight",0.000);
	
/*	
	//SoleilLight

	DxVBLibA::D3DVECTOR D3DSoleilPosVec;
  DxVBLibA::D3DCOLORVALUE D3DSoleilColor;

  D3DSoleilPosVec.x = 500;  D3DSoleilPosVec.y = 1000;  D3DSoleilPosVec.z = -1500;
  D3DSoleilColor.a = 1; D3DSoleilColor.b = 1; D3DSoleilColor.g = 1; D3DSoleilColor.r = 1;
  
  Soleil = new DxVBLibA::D3DLIGHT8;

  Soleil->Type =  DxVBLibA::D3DLIGHT_DIRECTIONAL;
  Soleil->Position =  D3DSoleilPosVec;
  Soleil->ambient =  D3DSoleilColor;   
  Soleil->diffuse =  D3DSoleilColor;
  Soleil->Attenuation0 =  1;
  Soleil->Range =  4000;
  //Soleil->

  pLight->CreateLight(Soleil, "Soleil", true);   
  pLight->UpdateLight(1, Soleil);

  
  // Set the material
  pMatFactory->CreateLightMaterial(1.0f, 1.0f, 1.0f, 1.0f, 0.3f, "matname");
*/
	

}
float CLTV3DMoteur::GetTimeElapsed(void)
{
return(pTV->AccurateTimeElapsed());
}
void CLTV3DMoteur::DrawTV3DAxis(void)
{
	p2DImmediate->Draw_Line3D(0,0,0,10000,0,0,RGBA(1, 1, 1, 1), -2);  // X Axis   Blanc
	p2DImmediate->Draw_Line3D(0,0,0,0,10000,0,RGBA(0, 1, 0, 1), -2);  // Y Axis   Vert
	p2DImmediate->Draw_Line3D(0,0,0,0,0,10000,RGBA(1, 0, 0, 1), -2);  // Z Axis   Rouge
}

void CLTV3DMoteur::DrawMeshAxis(CTVMesh* mesh)
{
	cTV_3DVECTOR VecPos;
	VecPos = mesh->GetPosition();

	p2DImmediate->Draw_Line3D(VecPos.x,VecPos.y,VecPos.z,VecPos.x + 100,VecPos.y,VecPos.z,RGBA(1, 1, 1, 1), -2);  // X Axis   Blanc
	p2DImmediate->Draw_Line3D(VecPos.x,VecPos.y,VecPos.z,VecPos.x,VecPos.y + 100,VecPos.z,RGBA(0, 1, 0, 1), -2);  // Y Axis   Vert
	p2DImmediate->Draw_Line3D(VecPos.x,VecPos.y,VecPos.z,VecPos.x,VecPos.y,VecPos.z + 100,RGBA(1, 0, 0, 1), -2);  // Z Axis   Rouge
}
void CLTV3DMoteur::SetMenuButtons(void)
{
int i;

///// Section Menu niveau 0 - Main
for(i = 0;i<3;i++)
	{
	clMenuManager->CreateBouton(pCube[i], 0);
	}
	
clMenuManager->stObjPassifs[0].ListeMeshesPassifs[0] = pEarth;
clMenuManager->CreateBouton(pEarthGrid, 0);
clMenuManager->CreateBouton(pFleche, ALLMENU);


///// Section Menu niveau 1 - Atmel ctrl
/*	for(i = 0; i<8;i++)
	{
	clMenuManager->CreateBouton(pIOButton[i], 1);
	}
*/
clMenuManager->stObjPassifs[1].ListeMeshesPassifs[0] = pMenuDomotique;
clMenuManager->stObjPassifs[1].ListeMeshesPassifs[1] = LightEtat;
clMenuManager->stObjPassifs[1].ListeMeshesPassifs[2] = FanEtat;   


/////  Section Menu niveau 2 - SysInfo
clMenuManager->stObjPassifs[2].ListeMeshesPassifs[0] = pInetConnected;

}

void CLTV3DMoteur::GetMeshClicked(void)
{
	clInput->pMeshClicked = NULL;
	pCollision = pScene->MousePick(clInput->stMouse.PosX, clInput->stMouse.PosY, 
										cTV_OBJECT_MESH, cTV_TESTTYPE_ACCURATETESTING );
	if(pCollision->IsCollision())
		clInput->pMeshClicked = pCollision->GetCollisionMesh();	
	clInput->pOldMeshClicked = clInput->pMeshClicked;
	
	if(clInput->pMeshClicked != NULL)
		MeshClickedAction();
}

void CLTV3DMoteur::MeshClickedAction(void)
{
	int i;
	int iIndex = clInput->pMeshClicked->iMeshIndex;

	if(iIndex == pSoleil->iMeshIndex)
	{
		clExplosion->Start();     // Démarre l'explosion
		clBallZ = new CLBallZ();
		MeshTable[1] = clBallZ->GetBallMesh();
		bGAME_MODE = 1;
	}
			if(iIndex == pCube[0]->iMeshIndex)  //  Menu 1
				{
					clMenuManager->iNiveauActif = 1; // ctrl atmel
					ShowTimeDisplays();
					//return(NULL);
				}	
			else if(iIndex == pCube[1]->iMeshIndex)  //  Menu 2
				{
					clMenuManager->iNiveauActif = 2; // info sys
					HideTimeDisplays();
				//	if(clSystemTools.IsInternetConnected() == NULL)
				//		pInetConnected->SetColor(pGlobals->RGBA256(0, 255, 0, 255),true);// Vert
				//	else
				//		pInetConnected->SetColor(pGlobals->RGBA256(255, 0, 0, 255),true);// Rouge
		
				//clSystemTools.GetIPs();	
				//clFontAndClock->WriteString(clSystemTools.IPLocal,2,-1000,-400,0,0,pGlobals->RGBA256(0,0,255,255));
				//clFontAndClock->WriteString(clSystemTools.IPInet,2,-1000,-600,0,0,pGlobals->RGBA256(0,0,255,255));
				//return(NULL); //return(ID_STUFF_SETATMELTIME);
				}
			else if(iIndex == pCube[2]->iMeshIndex)
			{//return(ID_STUFF_SELECTCOMPORT);
			}
			else if(iIndex == pFleche->iMeshIndex)    //  Main Menu
				{
					clMenuManager->iNiveauActif = 0; // Main
					HideTimeDisplays();
					//return(NULL);
				}	


			// Section Config des TimeDisplay
			else if(iIndex == LightTimeOn->pDisplaySelect->iMeshIndex) 
				{
				LightTimeOn->ShowControls();
				pScene->SetCamera(LightTimeOn->DisplayPos.x - 50, LightTimeOn->DisplayPos.y + 50, LightTimeOn->DisplayPos.z -750,
											LightTimeOn->DisplayPos.x, LightTimeOn->DisplayPos.y, LightTimeOn->DisplayPos.z);
				//return(NULL);
				}
			else if(iIndex == LightTimeOff->pDisplaySelect->iMeshIndex)
				{
				LightTimeOff->ShowControls();
				pScene->SetCamera(LightTimeOff->DisplayPos.x - 50, LightTimeOff->DisplayPos.y + 50, LightTimeOff->DisplayPos.z -750,
											LightTimeOff->DisplayPos.x, LightTimeOff->DisplayPos.y, LightTimeOff->DisplayPos.z);
				//return(NULL);
				}
			else if(iIndex == FanTimeOn->pDisplaySelect->iMeshIndex)
				{
				FanTimeOn->ShowControls();
				pScene->SetCamera(FanTimeOn->DisplayPos.x - 50, FanTimeOn->DisplayPos.y + 50, FanTimeOn->DisplayPos.z -750,
											FanTimeOn->DisplayPos.x, FanTimeOn->DisplayPos.y, FanTimeOn->DisplayPos.z);
				//return(NULL);
				}
			else if(iIndex == FanTimeOff->pDisplaySelect->iMeshIndex)
				{
				FanTimeOff->ShowControls();
				pScene->SetCamera(FanTimeOff->DisplayPos.x - 50, FanTimeOff->DisplayPos.y + 50, FanTimeOff->DisplayPos.z -750,
											FanTimeOff->DisplayPos.x, FanTimeOff->DisplayPos.y, FanTimeOff->DisplayPos.z);
				//return(NULL);
				}
	
		//  Send the configured alarm to the atmel card
				//LightTimeOn
			else if(iIndex == LightTimeOn->pAccept->iMeshIndex)
				{
				LightTimeOn->HideControls();
				for(i=0; i<4;i++)
					{
						//clAtmel.stAtmelIO[0].stAlarm.iStartTime[i] = LightTimeOn->iTime[i]; 
					}
					LightTimeOn->WriteTime(ClockTexture, NULL);
				SetCamera();
				//pScene->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);
				//return(ID_SET_LIGHT_TIMER);
				}
			else if(iIndex == LightTimeOn->pLeftArrow->iMeshIndex)
				{
				LightTimeOn->iSelectorIndex--;
				if(LightTimeOn->iSelectorIndex == -1)  // reste a trouver ou dans le code rafraichir le pNumSelect
					LightTimeOn->iSelectorIndex = 0;
				LightTimeOn->SetNumSelectPos();
				//return(NULL);
				}
			else if(iIndex == LightTimeOn->pRightArrow->iMeshIndex)
				{
					LightTimeOn->iSelectorIndex++;
				if(LightTimeOn->iSelectorIndex == 4)
					LightTimeOn->iSelectorIndex = 3;
				LightTimeOn->SetNumSelectPos();
				//return(NULL);
				}
			else if(iIndex == LightTimeOn->pPlus->iMeshIndex)
				{
				LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = LightTimeOn->iTime[LightTimeOn->iSelectorIndex] +1;
				if(LightTimeOn->iTime[LightTimeOn->iSelectorIndex] == 10)
					LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = 0;
				LightTimeOn->WriteTime(ClockTexture, NULL);   
				//return(NULL);
				}
			else if(iIndex == LightTimeOn->pMinus->iMeshIndex)
				{
				LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = LightTimeOn->iTime[LightTimeOn->iSelectorIndex] -1;
				if(LightTimeOn->iTime[LightTimeOn->iSelectorIndex] == -1)
					LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = 9;
				LightTimeOn->WriteTime(ClockTexture, NULL);   
				//return(NULL);
				}


			//LightTime Off
			else if(iIndex == LightTimeOff->pAccept->iMeshIndex)
				{
				LightTimeOff->HideControls();
				for(i=0; i<4;i++)
					{
//						clAtmel.stAtmelIO[0].stAlarm.iStopTime[i] = LightTimeOff->iTime[i]; 
					}
				SetCamera();
				//return(NULL);
				}
			else if(iIndex == FanTimeOn->pAccept->iMeshIndex)
				{
				FanTimeOn->HideControls();
				for(i=0; i<4;i++)
					{
//						clAtmel.stAtmelIO[1].stAlarm.iStartTime[i] = FanTimeOn->iTime[i]; 
					}
				SetCamera();
				//return(NULL);
				}
			else if(iIndex == FanTimeOff->pAccept->iMeshIndex)
				{
				FanTimeOff->HideControls();
				for(i=0; i<4;i++)
					{
//						clAtmel.stAtmelIO[1].stAlarm.iStopTime[i] = FanTimeOff->iTime[i]; 
					}
				SetCamera();
				//return(NULL);
				}




			/*	
			else if(pMeshClicked == pIOButton[0])
				return(IOClickedAction(0));
			else if(pMeshClicked == pIOButton[1])
				return(IOClickedAction(1));	
			else if(pMeshClicked == pIOButton[2])
				return(IOClickedAction(2));
			else if(pMeshClicked == pIOButton[3])
				return(IOClickedAction(3));
			else if(pMeshClicked == pIOButton[4])
				return(IOClickedAction(4));
			else if(pMeshClicked == pIOButton[5])
				return(IOClickedAction(5));
			else if(pMeshClicked == pIOButton[6])
				return(IOClickedAction(6));
			else if(pMeshClicked == pIOButton[7])
				return(IOClickedAction(7));

*/
}

/*
unsigned int CLTV3DMoteur::InputAction(void)
{
	if(clInput.stKeyboard.KeyPressed1 == TV_KEY_ESCAPE)//Check if ESCAPE has been pressed.
	{		
		PostQuitMessage(0);
	}
	if(clInputstKeyboard.KeyPressed1 == TV_KEY_0)//
	{		
	//	clMenuManager.iAncienNiveau = clMenuManager.iNiveauActif;
	//	clMenuManager.iNiveauActif = 0;
	}
	if(clInput.stKeyboard.KeyPressed1 == TV_KEY_1)//
	{		
	//	clMenuManager.iAncienNiveau = clMenuManager.iNiveauActif;
	//	clMenuManager.iNiveauActif = 1;
	}
	if(clInput.stKeyboard.KeyPressed1 == TV_KEY_2)//
	{		
	//	clMenuManager.iAncienNiveau = clMenuManager.iNiveauActif;
	//	clMenuManager.iNiveauActif = 2;
	}
	if(clInput.stKeyboard.KeyPressed1 == TV_KEY_Z)//
	{		
	pIOButton[1]->EnableGroup(0,true);	
	}
	if(clInput.stKeyboard.KeyPressed1 == TV_KEY_X)//
	{		
	pIOButton[1]->EnableGroup(0,false);
	}



	if(bCAM_CTRL)
	{
	//Check if we pressed the UP arrow key, if so, then we are
    //walking forward.
    if((clInput.stKeyboard.KeyPressed1 == TV_KEY_UP) || (clInput.stInputDevice.stKeyboard.KeyPressed2 == TV_KEY_UP))
        sngWalk = 1;
    //If we are not walking forward, maybe we are walking backward
    //by using the DOWN arrow? If so, set walk speed to negative.
    if((clInput.stKeyboard.KeyPressed1 == TV_KEY_DOWN) || (clInput.stInputDevice.stKeyboard.KeyPressed2 == TV_KEY_DOWN))
        sngWalk = -1;
    //Check if we pressed the LEFT arrow key, if so, then strafe
    //on the left.
    if((clInput.stKeyboard.KeyPressed1 == TV_KEY_LEFT) || (clInput.stInputDevice.stKeyboard.KeyPressed2 == TV_KEY_LEFT))
        sngStrafe = 1;
    //If we are not strafing left, maybe we want to strafe to the
    //right, using the RIGHT arrow? If so, set strafe to negative.
    if((clInput.stKeyboard.KeyPressed1 == TV_KEY_RIGHT) || (clInput.stInputDevice.stKeyboard.KeyPressed2 == TV_KEY_RIGHT))
        sngStrafe = -1;
	
	
    //Now, for the mouse input...
	long tmpMouseX = clInput.stMouse.DeltaPosX;
	long tmpMouseY = clInput.stMouse.DeltaPosY;
	long tmpMouseB1 = clInput.stMouse.B1; 
	long tmpMouseB2 = clInput.stMouse.B2; 
	long tmpMouseB3 = clInput.stMouse.B3;
    long tmpMouseScrollOld = 0;  // a integrer!!
	long tmpMouseScrollNew = 0; // controle de la molette

    //We pass the actual value of the mouse scroller to the variable
    //that holds the old mouse scroller value.
    // tmpMouseScrollOld = tmpMouseScrollNew;

    //From the mouse return values, we update the camera angles
    //by adding or substracting the mouse return value.
    sngAngleX = sngAngleX - ((float)tmpMouseY / 100.0f);
    sngAngleY = sngAngleY - ((float)tmpMouseX / 100.0f);

    //We will add a simple check, so we can't look up at more
    //than 80 degrees nor down than -80 degrees.
    if(sngAngleX > 1.3f) sngAngleX = 1.3f;
    if(sngAngleX < -1.3f) sngAngleX = -1.3f;
	
    //Okay, now for the smothing of the movement... We checked
    //above if we were pressing a key. If so, then we updated the
    //movement variable to 1 (positive or negative). Here, we
    //lower this value until it get to 0. This method give us a
    //smoother camera movement. We start by updating the forward
    //and backward (walk) movement
    if(sngWalk > 0)
	{
        sngWalk = sngWalk - 0.05f;
        if(sngWalk < 0) sngWalk = 0;
	}
    if(sngWalk < 0)
	{
        sngWalk = sngWalk + 0.05f;
        if(sngWalk > 0) sngWalk = 0;
	}
    //Now, we update the left and right (strafe) movement.
    if(sngStrafe > 0)
	{
        sngStrafe = sngStrafe - 0.05f;
        if(sngStrafe < 0) sngStrafe = 0;
	}
    if(sngStrafe < 0)
	{
        sngStrafe = sngStrafe + 0.05f;
        if(sngStrafe > 0) sngStrafe = 0;
	}
	fTimeElapsed = GetElapsedTime();
    //Update the vectors using the angles and positions.
    D3DCamPos.x = D3DCamPos.x + (cosf(sngAngleY) * sngWalk * fTimeElapsed) + (cosf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * fTimeElapsed);
    D3DCamPos.z = D3DCamPos.z + (sinf(sngAngleY) * sngWalk * fTimeElapsed) + (sinf(sngAngleY + 3.141596f / 2.0f) * sngStrafe * fTimeElapsed);
    
	if(bMOUSE_CAM_CTRL)
	{
	CamLookAt.x = CamPos.x + cosf(sngAngleY);
	CamLookAt.y = CamPos.y + tanf(sngAngleX);
	CamLookAt.z = CamPos.z + sinf(sngAngleY);
	}
	} // fin du if bCTRL_CAM



	
}*/


void CLTV3DMoteur::WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color)
{
unsigned int i;
CTVMesh* Write[50];
CTVMesh* Written[50];


for(i=0; i< strlen(cMot); i++)  //strlen
{
switch(cMot[i])
     {
     case 'A':
     case 'a':
            Write[i]= st3DFont.Alphabet[0];     //  A
            break;
     case 'B':
     case 'b':
            Write[i]= st3DFont.Alphabet[1];       // B
            break;
     case 'C':
     case 'c':
            Write[i]= st3DFont.Alphabet[2];       // C
            break;
     case 'D':
     case 'd':
            Write[i]= st3DFont.Alphabet[3];      //  ...
            break;
     case 'E':
     case 'e':
            Write[i]= st3DFont.Alphabet[4];
            break;
     case 'F':
     case 'f':
            Write[i]= st3DFont.Alphabet[5];
            break;
     case 'G':
     case 'g':
            Write[i]= st3DFont.Alphabet[6];
            break;
     case 'H':
     case 'h':
            Write[i]= st3DFont.Alphabet[7];
            break;
     case 'I':
     case 'i':
            Write[i]= st3DFont.Alphabet[8];
            break;
     case 'J':
     case 'j':
            Write[i]= st3DFont.Alphabet[9];
            break;
     case 'K':
     case 'k':
            Write[i]= st3DFont.Alphabet[10];
            break;
     case 'L':
     case 'l':
            Write[i]= st3DFont.Alphabet[11];
            break;
     case 'M':
     case 'm':
            Write[i]= st3DFont.Alphabet[12];
            break;
     case 'N':
     case 'n':
            Write[i]= st3DFont.Alphabet[13];
            break;
     case 'O':
     case 'o':
            Write[i]= st3DFont.Alphabet[14];
            break;
     case 'P':
     case 'p':
            Write[i]= st3DFont.Alphabet[15];
            break;
     case 'Q':
     case 'q':
            Write[i]= st3DFont.Alphabet[16];
            break;
     case 'R':
     case 'r':
            Write[i]= st3DFont.Alphabet[17];
            break;
     case 'S':
     case 's':
            Write[i]= st3DFont.Alphabet[18];
            break;
     case 'T':
     case 't':
            Write[i]= st3DFont.Alphabet[19];
            break;
     case 'U':
     case 'u':
            Write[i]= st3DFont.Alphabet[20];
            break;
     case 'V':
     case 'v':
            Write[i]= st3DFont.Alphabet[21];
            break;
     case 'W':
     case 'w':
            Write[i]= st3DFont.Alphabet[22];     // ...
            break;
     case 'X':
     case 'x':
            Write[i]= st3DFont.Alphabet[23];     // Y
            break;
     case 'Y':
     case 'y':
            Write[i]= st3DFont.Alphabet[24];     //  X
            break;
     case 'Z':
     case 'z':
            Write[i]= st3DFont.Alphabet[25];     // Z
            break;
     case '1':
            Write[i]= st3DFont.Numbers[1];
            break;
     case '2':
            Write[i]= st3DFont.Numbers[2];
            break;
     case '3':
            Write[i]= st3DFont.Numbers[3];
            break;
     case '4':
            Write[i]= st3DFont.Numbers[4];
            break;
     case '5':
            Write[i]= st3DFont.Numbers[5];
            break;
     case '6':
            Write[i]= st3DFont.Numbers[6];
            break;
     case '7':
            Write[i]= st3DFont.Numbers[7];
            break;
     case '8':
            Write[i]= st3DFont.Numbers[8];
            break;
     case '9':
            Write[i]= st3DFont.Numbers[9];
            break;
     case '0':
            Write[i]= st3DFont.Numbers[0];
            break;
	 case '.':
			Write[i]= st3DFont.SpecialChar[0];
            break;

     default: 
		     Write[i]=NULL;    //  Pour l'espace ou les caractere inconnu
			 break;
     }
if(Write[i] != NULL)
  {
	  Written[i] =  Write[i]->Duplicate("Written", true); //
	   if(cMot[i] == '.')
	    Written[i]->SetPosition(x,y - size*20,z);	 
	  else
		Written[i]->SetPosition(x,y,z);					//     
	  Written[i]->RotateY(angle_y,true);  // Rotate around global Y axis.
	  Written[i]->SetScale(size,size,size);
	  //   appliquer les textures avant d'afficher  ( a faire)
	  Written[i]->SetColor(Color,true);
	  Written[i]->Enable(true);
  }
else
  {Written[i] = NULL;}


x = x+(55*size);   // 55 = decallage entre les lettres * Size pour fitter avec la grosseur des lettres 
}

//  Written[] peux etre la sortie si besoin...

}



