#include "Area.h"

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 
#pragma warning(disable: 4244) // conversion double -> float 
#pragma warning(disable: 4996) // Bug avec les string...  ptete a vérifier


CLArea *CLArea::_singleton = NULL;

CLArea::CLArea(void)
{
	clAtmos = NULL;
	clMenuManager = NULL;
	Snowfall = NULL;
	SmokeFest = NULL;
	clExplosion = NULL;
	clBallZ = NULL;

	clGlobalVar = CLGlobalVar::getInstance();
	clTV3D = CLTV3DMoteur::getInstance();
	Init3DEnvironnement();
}

CLArea::~CLArea(void)
{

	clNotesManager->kill();

	if(clAtmos){ delete(clAtmos); clAtmos = NULL; }
	if(Snowfall){ delete(Snowfall); Snowfall = NULL; }
	if(SmokeFest){ delete(SmokeFest); SmokeFest = NULL; }
	if(clMenuManager){ delete(clMenuManager); clMenuManager = NULL; }
	if(LightTimeOn) { delete(LightTimeOn); LightTimeOn = NULL; }
	if(LightTimeOff) { delete(LightTimeOff); LightTimeOff = NULL; }
	if(FanTimeOn) { delete(FanTimeOn); FanTimeOn = NULL; }
	if(FanTimeOff) { delete(FanTimeOff); FanTimeOff = NULL; }
	if(ClockDisplay) { delete(ClockDisplay); ClockDisplay = NULL; }
	if(clExplosion) { delete (clExplosion); clExplosion = NULL; }
	if(clBallZ) { delete (clBallZ); clBallZ = NULL; }


	 // Clean up the scene 
    if( pScene ) 
    { 
      pScene->DestroyAllMeshes(); 
      delete(pScene); pScene = NULL; 
    } 
	
}



void CLArea::Init3DEnvironnement(void)
{
	InitScene();
	InitLightAndMaterial();
	LoadTextures();
	
	if(clGlobalVar->GetbUSE_TV_PHYSICS())
		InitPhysics();
	
	if(clGlobalVar->GetbLOAD_ATMOSPHERE()) 
	{
		clAtmos = new CLAtmosphere(); 
		clLand = new CLLand();
		clWater = new CLWater();
	}

	
	Create3DObjects();
		
	// Locate and create the clock display
	DisplayPos.x = 600; DisplayPos.y = 975; DisplayPos.z = 0;
	ClockDisplay = new CLClockDisplay(DisplayPos,3,0,BLUE, st3DFont);
	//  Doit etre apres la création de ClockDisplay sinon affiche pas...!?!?!?!?   Pkoi?
	CreateMenuDomotique();  // Load les 4 Clock Displays
	
	clMenuManager = new CLMenuManager();  
	SetMenuButtons();
	
	clExplosion = new CLExplosion();

	clNotesManager = CLNotesManager::getInstance();	

	p2DText = new CTVScreen2DText();
}

void CLArea::InitScene(void)
{
	pScene = new CTVScene();
	pScene->SetRenderMode(cTV_SOLID);
    pScene->SetShadowParameters(RGBA(0.0, 0.0, 0.0, 0.5), false);
}

void CLArea::InitLightAndMaterial(void)
{ 
	pLights = new CTVLightEngine();

	cTV_3DVECTOR VecDir;
	pLights->SetGlobalAmbient(0,0,0);

	VecDir.x = -1; VecDir.y = -1; VecDir.z = 1;
	iMainLight = pLights->CreateDirectionalLight(&VecDir,1,1,1,"GlobalLight",1.0);
	pLights->SetSpecularLighting(true);
	pLights->SetLightProperties(iMainLight,true,true,false);
}
void CLArea::InitPhysics(void)
{
	pTVPhysics = new CTVPhysics();
	pTVPhysics->Initialize();
    pTVPhysics->SetSolverModel(cTV_SOLVER_EXACT);
    pTVPhysics->SetFrictionModel(cTV_FRICTION_EXACT);
    pTVPhysics->SetGlobalGravity(new cTV_3DVECTOR(0, -9.800908285f, 0));
	pTVPhysics->SetWorldSize(new cTV_3DVECTOR(-10000,-10000,-10000),new cTV_3DVECTOR(10000,10000,10000));
}
void CLArea::LoadTextures(void)
{
	pTexFactory = new CTVTextureFactory();

	SoleilTex = pTexFactory->LoadTexture("Media\\gold_01.jpg", "SoleilTex", -1,-1,cTV_COLORKEY_MAGENTA,true);
	//TempSensTexture = pTexFactory->LoadTexture("Media\\SensTempGREEN.bmp", "SensTemp", -1,-1,cTV_COLORKEY_MAGENTA,true);
	//CursorTexture = pTexFactory->LoadTexture("Media\\AimCursor.bmp", "CursorTex", -1,-1,cTV_COLORKEY_MAGENTA,true);
	ClockTexture = pTexFactory->LoadTexture("Media\\gold_01.jpg","ClockTex",-1,-1,cTV_COLORKEY_NO,true);
	MarbleTexture = pTexFactory->LoadTexture("Media\\marble10.jpg","MarbleTex",-1,-1,cTV_COLORKEY_NO,true);
    EarthTexture = pTexFactory->LoadTexture("Media\\Earth.jpg","EarthTex",-1,-1,cTV_COLORKEY_NO,true);
	CloudsTexture = pTexFactory->LoadTexture("Media\\Clouds.jpg","CloudsTex",-1,-1,cTV_COLORKEY_BLACK,true);
	//TexturePanneauMenu = pTexFactory->LoadTexture("Media\\gold_01.jpg","CloudsTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBAlarms = pTexFactory->LoadTexture("Media\\AlarmClockIcon.jpg","AlarmsTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBSystem = pTexFactory->LoadTexture("Media\\SystemIcon.jpg","SystemTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBTemp = pTexFactory->LoadTexture("Media\\TemperatureMonitorIcon.jpg","TempTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBAtmel = pTexFactory->LoadTexture("Media\\memoryChip.jpg","AtmelTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBTools = pTexFactory->LoadTexture("Media\\ToolsIcon.png","ToolsTex",-1,-1,cTV_COLORKEY_NO,true);
	//ClosedIconTex = pTexFactory->LoadTexture("Media\\ClosedIcon.bmp","ClosedIconTex",-1,-1,cTV_COLORKEY_WHITE,true);
	//OpenedIconTex = pTexFactory->LoadTexture("Media\\OpenedIcon.bmp","OpenedIconTex",-1,-1,cTV_COLORKEY_WHITE,true);
	//RedLightTrailTex = pTexFactory->LoadTexture("Media\\RedLight.jpg","RedLightTrailTex",-1,-1,cTV_COLORKEY_BLACK,true);
	EarthGridTex = pTexFactory->LoadTexture("Media\\EarthGridXL.bmp","EarthGridTex",-1,-1,cTV_COLORKEY_BLACK,true);
	SnowTexture = pTexFactory->LoadTexture("Media\\Snow.dds","SnowTex",-1,-1,cTV_COLORKEY_BLACK,true);
}

void CLArea::Create3DObjects(void)
{	
	// variable pour permettre de detecter les bounding box   --  Pas encore utilisé
	//cTV_3DVECTOR BoxMin;
	//cTV_3DVECTOR BoxMax;

	//   testing purpose
	pSoleil = pScene->CreateMeshBuilder("SoleilMesh");
	pSoleil->CreateBox(20,20,20, false);
	pSoleil->SetPosition(0,500,0);
	pSoleil->SetLightingMode(cTV_LIGHTING_MANAGED);
	pSoleil->SetColor(RED,true);

	
	// Mesh qui peux etre mis a la meme position qu'un autre.  En fesant suivre CameraAim par la camera, ceci 
	// permet de faire rotationner l'objet que l'on regarde sans que la camera ne suive la rotation.
	pCameraAim = pScene->CreateMeshBuilder("CameraAim");
	pCameraAim->CreateBox(1,1,1, false);

	//  Load the 3D Font
	LoadLettersAndNumbers();


///// Section Menu niveau 0 - Main

	clCubX = new CLCubX(4, NULL, cTV_3DVECTOR(-400,500,0), true);
	
	clCubeSys = new CL3DObject("Media\\Box.tvm", 10, TexBSystem, cTV_3DVECTOR(-800, 300, -250), true);
	clCubeTools = new CL3DObject("Media\\Box.tvm", 10, TexBTools, cTV_3DVECTOR(-1100, 300, -500), true);
	
	clEarth = new CL3DObject("media\\Earth.tvm", 10, NULL, cTV_3DVECTOR(800,500,350), true);
	//clEarth->GetMesh()->SetShadowCast(false,false);  // Pour éviter que la terre fasse des ombrage dans la boule de la grid
	clEarthGrid = new CL3DObject("media\\Sphere.tvm", 1.2, EarthGridTex, cTV_3DVECTOR(800,500,350), true);
	clEarthGrid->GetMesh()->SetAlphaTest(true,0,true,-1);
	clEarthGrid->GetMesh()->SetBlendingMode(cTV_BLEND_ALPHA, -1);
	//clEarthGrid->GetMesh()->SetLightingMode(cTV_LIGHTING_NONE, -1,-1);

	clFleche = new CL3DObject("media\\Fleche.tvm", 1, MarbleTexture, cTV_3DVECTOR(1000,600,300), true);
	clFleche->GetMesh()->RotateY(180,true);

///// Section Menu niveau 1 - Atmel ctrl
	/*	for(i = 0; i<8;i++)
		{
			pIOButton[i] = pScene->CreateBillboard(ClosedIconTex, (i*150)-500,750,0,206,205,"IOIcon", true);
			pIOButton[i]->Enable(false);  	
		}
	*/
	
	clMenuDomotique = new CL3DObject("media\\MenuDomo.tvm", 8, ClockTexture, cTV_3DVECTOR(-350, 250, 0), false);

	clLightEtat = new CL3DObject("media\\Sphere.tvm", 0.3, ClockTexture, cTV_3DVECTOR(-1250, 800, 0), false);
	clLightEtat->GetMesh()->SetColor(YELLOW, true);

	clFanEtat = new CL3DObject("media\\Sphere.tvm", 0.3, ClockTexture, cTV_3DVECTOR(-1250, 300, 0), false);
	clFanEtat->GetMesh()->SetColor(YELLOW, true);

/////  Section Menu niveau 2 - SysInfo
	clInetConnected = new CL3DObject("media\\Sphere.tvm", 0.5, MarbleTexture, cTV_3DVECTOR(-500,-250,0), false);
	clInetConnected->GetMesh()->SetColor(RED, true);

}


void CLArea::CreateMenuDomotique(void)
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
}
void CLArea::LoadLettersAndNumbers(void)
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

void CLArea::ShowTimeDisplays(void)
{
LightTimeOn->ShowDisplay();
LightTimeOff->ShowDisplay();
FanTimeOn->ShowDisplay();
FanTimeOff->ShowDisplay();
}

void CLArea::HideTimeDisplays(void)
{
LightTimeOn->HideDisplay();
LightTimeOff->HideDisplay();
FanTimeOn->HideDisplay();
FanTimeOff->HideDisplay();
}

void CLArea::Refresh3DClock(void)
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




void CLArea::SetMenuButtons(void)
{
///// Section Menu niveau 0 - Main
clMenuManager->CreateBouton(clCubX->GetMesh(),0);
clMenuManager->CreateBouton(clCubeSys->GetMesh(), 0);
clMenuManager->CreateBouton(clCubeTools->GetMesh(), 0);
clMenuManager->CreateBouton(clEarth->GetMesh(), 0);
clMenuManager->CreateBouton(clEarthGrid->GetMesh(), 0);
clMenuManager->CreateBouton(clFleche->GetMesh(), ALLMENU);

///// Section Menu niveau 1 - Atmel ctrl
/*	for(i = 0; i<8;i++)
	{
	clMenuManager->CreateBouton(pIOButton[i], 1);
	}
*/
clMenuManager->CreateBouton(clMenuDomotique->GetMesh(), 1);
clMenuManager->CreateBouton(clLightEtat->GetMesh(), 1);
clMenuManager->CreateBouton(clFanEtat->GetMesh(), 1);

/////  Section Menu niveau 2 - SysInfo
clMenuManager->CreateBouton(clInetConnected->GetMesh(), 2);
}





void CLArea::WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color)
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








void CLArea::RenderArea(void)
{

}

void CLArea::Render4CubX(float TimeElapsed, bool bScreensaverRunning)
{
//	int i = 0;
	float SpeedFactor = TimeElapsed / 1000; // pour avoir un speedfactor en seconde

		//  Update some infos before rendering
		Refresh3DClock();
		clMenuManager->SetActiveMenu();
		
		
		if(clGlobalVar->GetRenderAtmos()) 
		{
			clWater->StartReflectRender();
			clAtmos->pTVAtmos->Atmosphere_Render();
			clLand->Render();
			clCubX->Render();
			clEarth->Render();
			clWater->StopReflectRender();
			
			clWater->StartRefractRender();
			clAtmos->pTVAtmos->Atmosphere_Render();
			clLand->Render();
			clCubX->Render();
			clEarth->Render();
			clWater->StopRefractRender();
		}
		
//////// Start rendering block
clTV3D->StartRenderingBlock();
		
		if(clGlobalVar->GetRenderAtmos())
		{
			clAtmos->UpdateAndRender(TimeElapsed);
			clWater->Render();
			clLand->Render();
		}
		
	//  CE QUI A ETE RENDERER POUR LES EFFET DE LEAU N'ONT PAS BESOINS DETRE RE_RENDERER?????!!!!
		// A DEMANDER PKOI A TV3D CAR CEST RENDERER EN DEHORS DE CLEAR et RENDERonSCREEN

		pSoleil->Render(); 
		clCubX->AnimateCubX(TimeElapsed);
		clCubX->Render();

		clNotesManager->Render();

		clMenuManager->Render();
		ClockDisplay->Render();
		LightTimeOn->Render();
		LightTimeOff->Render();
		FanTimeOn->Render();
		FanTimeOff->Render();
		
		
		if(clExplosion->IsFinished() == false)
			clExplosion->Render(TimeElapsed); 

		pScene->FinalizeShadows();
		
		if(clGlobalVar->GetbUSE_TV_PHYSICS())
			pTVPhysics->Simulate(SpeedFactor);
		if(clGlobalVar->GetRender3DAxis())
			clTV3D->DrawTV3DAxis();
		
		Render2DText();
	
		if(!bScreensaverRunning)
		  {
			  CL_TV3DCEGUI::getInstance()->Render();  
		  }
	pTVPhysics->RenderDebugInfo(true);	
//////// End rendering block
clTV3D->StopRenderingBlock();
}

void CLArea::Render4CarMode(float TimeElapsed)
{
 
pTVPhysics->Simulate(TimeElapsed * 0.0025f);
                                 
clTV3D->StartRenderingBlock();

  //Render Atmosphere
  if(clGlobalVar->GetRenderAtmos())   // A arranger car l'esu ne sera pas renderer..  manque le render sur les surface...
		{
			clAtmos->UpdateAndRender(TimeElapsed);
			clWater->Render();
		}
                    
  //Render Objects
  clCarScene->Render();
 
    
  pScene->FinalizeShadows();
	pTVPhysics->RenderDebugInfo(true);	
clTV3D->StopRenderingBlock();
}

void CLArea::Render4BallZMode(float TimeElapsed)
{
cTV_3DVECTOR CameraAimVec;

float SpeedFactor = TimeElapsed / 1000; // pour avoir un speedfactor en seconde    


clTV3D->StartRenderingBlock();


	clBallZ->Render();
	CameraAimVec = clBallZ->GetBallPosition();  // Donne une cible a la camera pour etre indépendant des moves de la balle
	pCameraAim->SetPosition(CameraAimVec.x, CameraAimVec.y, CameraAimVec.z);
//	clTV3D->pCamera->ChaseCamera(pCameraAim,new cTV_3DVECTOR(0,350,300),new cTV_3DVECTOR(0,0,0),1000,true,-1);
  
	pScene->FinalizeShadows();

	
		Render2DText();
		pTVPhysics->RenderDebugInfo(true);		
clTV3D->StopRenderingBlock();


pTVPhysics->Simulate(SpeedFactor);


}

//	Display anyway 2D Text on the interface
void CLArea::Render2DText(void)
{
char BufferTime[256];
char BufferBallSpeed[256];
cTV_3DVECTOR BallSpeedVec;

if(clGlobalVar->GetShowFPS())
		clTV3D->SetShowFPS(true);
	else clTV3D->SetShowFPS(false);

p2DText->Action_BeginText(false);
////////////////////////////////

if(clGlobalVar->GetRender3DAxis())
	{
		p2DText->NormalFont_DrawText("X = White, Y = Green, Z = Red",5,20,RGBA(1,1,1,1),0);
	}
if(clGlobalVar->GetRenderAtmos())
	{
		sprintf(BufferTime, "%d", int(clAtmos->lTimeOfDay));  //  ptete enlever le int() pour mettre /1000  a la place
		p2DText->NormalFont_DrawText("Time of Day: ",5,32,RGBA(1,1,1,1),0);
		p2DText->NormalFont_DrawText(BufferTime,85,32,RGBA(1,1,1,1),0);	
	}
	if(iSelectedArea == BALLZ_MODE)
	{
		BallSpeedVec = clBallZ->GetBallSpeed();
		sprintf(BufferBallSpeed, "%d, %d, %d", int(BallSpeedVec.x), int(BallSpeedVec.y), int(BallSpeedVec.z));
		p2DText->NormalFont_DrawText("Ball Speed: ",5,44,RGBA(1,1,1,1),0);
		p2DText->NormalFont_DrawText(BufferBallSpeed,85,44,RGBA(1,1,1,1),0);	
	}

//////////////////////////////
p2DText->Action_EndText();
}



void CLArea::MeshClickedAction(int iMeshClickedIndex)
{
	int i;
			if(iMeshClickedIndex == pSoleil->iMeshIndex)
				{
					iSelectedArea = BALLZ_MODE;
					clBallZ = new CLBallZ();
					//clGlobalVar->SetbCAM_CTRL(false);
				}
			if(iMeshClickedIndex == clCubX->GetMesh()->iMeshIndex)  //  Menu 1
				{
					clCubX->iActualState = POSITIONNING;
					/*
					clMenuManager->iAncienNiveau = clMenuManager->iNiveauActif;
					clMenuManager->iNiveauActif = 1; // ctrl atmel
					ShowTimeDisplays();
					*/
				}
			else if(iMeshClickedIndex == clCubeSys->GetMesh()->iMeshIndex)  //  Menu 2
				{
					//clMenuManager->iAncienNiveau = clMenuManager->iNiveauActif;
					//clMenuManager->iNiveauActif = 2; // info sys
					clExplosion->Start();     // Démarre l'explosion
				}
			else if(iMeshClickedIndex == clCubeTools->GetMesh()->iMeshIndex)
				{
					clCarScene	= new CLCarScene();
					iSelectedArea = CAR_MODE;
				}
			else if(iMeshClickedIndex == clFleche->GetMesh()->iMeshIndex)    //  Main Menu
				{
					clMenuManager->iAncienNiveau = clMenuManager->iNiveauActif;
					clMenuManager->iNiveauActif = 0; // Main
					HideTimeDisplays();
				}	

			if(iMeshClickedIndex == clEarth->GetMesh()->iMeshIndex)
				{
					
				}
//  Selection des Cubes du CubX
			if(iMeshClickedIndex == clCubX->pCubXBtn[0]->iMeshIndex)  //  Cube 1
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 1;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[1]->iMeshIndex)  //  Cube 2
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 2;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[2]->iMeshIndex)  //  Cube 3
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 3;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[3]->iMeshIndex)  //  Cube 4
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 4;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[4]->iMeshIndex)  //  Cube 5
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 5;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[5]->iMeshIndex)  //  Cube 6
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 6;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[6]->iMeshIndex)  //  Cube 7
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 7;
				}
			else if(iMeshClickedIndex == clCubX->pCubXBtn[7]->iMeshIndex)  //  Cube 8
				{
					clCubX->iActualState = SELECTING;
					clCubX->iSelectedCube = 8;
				}

			if(iMeshClickedIndex == clNotesManager->GetAddNoteBtnMesh()->iMeshIndex)
				{
					CL_TV3DCEGUI::getInstance()->EnableNoteBloc();
					clNotesManager->AddNote();
				}



			// Section Config des TimeDisplay
			if(iMeshClickedIndex == LightTimeOn->pDisplaySelect->iMeshIndex) 
				{
				LightTimeOn->ShowControls();
				pScene->SetCamera(LightTimeOn->DisplayPos.x - 50, LightTimeOn->DisplayPos.y + 50, LightTimeOn->DisplayPos.z -750,
											LightTimeOn->DisplayPos.x, LightTimeOn->DisplayPos.y, LightTimeOn->DisplayPos.z);
				}
			else if(iMeshClickedIndex == LightTimeOff->pDisplaySelect->iMeshIndex)
				{
				LightTimeOff->ShowControls();
				pScene->SetCamera(LightTimeOff->DisplayPos.x - 50, LightTimeOff->DisplayPos.y + 50, LightTimeOff->DisplayPos.z -750,
											LightTimeOff->DisplayPos.x, LightTimeOff->DisplayPos.y, LightTimeOff->DisplayPos.z);
				}
			else if(iMeshClickedIndex == FanTimeOn->pDisplaySelect->iMeshIndex)
				{
				FanTimeOn->ShowControls();
				pScene->SetCamera(FanTimeOn->DisplayPos.x - 50, FanTimeOn->DisplayPos.y + 50, FanTimeOn->DisplayPos.z -750,
											FanTimeOn->DisplayPos.x, FanTimeOn->DisplayPos.y, FanTimeOn->DisplayPos.z);
				}
			else if(iMeshClickedIndex == FanTimeOff->pDisplaySelect->iMeshIndex)
				{
				FanTimeOff->ShowControls();
				pScene->SetCamera(FanTimeOff->DisplayPos.x - 50, FanTimeOff->DisplayPos.y + 50, FanTimeOff->DisplayPos.z -750,
											FanTimeOff->DisplayPos.x, FanTimeOff->DisplayPos.y, FanTimeOff->DisplayPos.z);
				}
	
		//  Send the configured alarm to the atmel card
				//LightTimeOn
			if(iMeshClickedIndex == LightTimeOn->pAccept->iMeshIndex)
				{
				LightTimeOn->HideControls();
				for(i=0; i<4;i++)
					{
						//clAtmel.stAtmelIO[0].stAlarm.iStartTime[i] = LightTimeOn->iTime[i]; 
					}
					LightTimeOn->WriteTime(ClockTexture, NULL);
				clTV3D->ResetCamera();
				//pScene->SetCamera(CamPos.x, CamPos.y, CamPos.z, CamLookAt.x, CamLookAt.y, CamLookAt.z);
				}
			else if(iMeshClickedIndex == LightTimeOn->pLeftArrow->iMeshIndex)
				{
				LightTimeOn->iSelectorIndex--;
				if(LightTimeOn->iSelectorIndex == -1)  // reste a trouver ou dans le code rafraichir le pNumSelect
					LightTimeOn->iSelectorIndex = 0;
				LightTimeOn->SetNumSelectPos();
				}
			else if(iMeshClickedIndex == LightTimeOn->pRightArrow->iMeshIndex)
				{
				LightTimeOn->iSelectorIndex++;
				if(LightTimeOn->iSelectorIndex == 4)
					LightTimeOn->iSelectorIndex = 3;
				LightTimeOn->SetNumSelectPos();
				}
			else if(iMeshClickedIndex == LightTimeOn->pPlus->iMeshIndex)
				{
				LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = LightTimeOn->iTime[LightTimeOn->iSelectorIndex] +1;
				if(LightTimeOn->iTime[LightTimeOn->iSelectorIndex] == 10)
					LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = 0;
				LightTimeOn->WriteTime(ClockTexture, NULL);   
				}
			else if(iMeshClickedIndex == LightTimeOn->pMinus->iMeshIndex)
				{
				LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = LightTimeOn->iTime[LightTimeOn->iSelectorIndex] -1;
				if(LightTimeOn->iTime[LightTimeOn->iSelectorIndex] == -1)
					LightTimeOn->iTime[LightTimeOn->iSelectorIndex] = 9;
				LightTimeOn->WriteTime(ClockTexture, NULL);   
				}


			//LightTime Off
			if(iMeshClickedIndex == LightTimeOff->pAccept->iMeshIndex)
				{
				LightTimeOff->HideControls();
				for(i=0; i<4;i++)
					{
//						clAtmel.stAtmelIO[0].stAlarm.iStopTime[i] = LightTimeOff->iTime[i]; 
					}
				clTV3D->ResetCamera();
				}
			else if(iMeshClickedIndex == FanTimeOn->pAccept->iMeshIndex)
				{
				FanTimeOn->HideControls();
				for(i=0; i<4;i++)
					{
//						clAtmel.stAtmelIO[1].stAlarm.iStartTime[i] = FanTimeOn->iTime[i]; 
					}
				clTV3D->ResetCamera();
				}
			else if(iMeshClickedIndex == FanTimeOff->pAccept->iMeshIndex)
				{
				FanTimeOff->HideControls();
				for(i=0; i<4;i++)
					{
//						clAtmel.stAtmelIO[1].stAlarm.iStopTime[i] = FanTimeOff->iTime[i]; 
					}
				clTV3D->ResetCamera();
				}
}