#include "CubXScene.h"

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 
#pragma warning(disable: 4244) // conversion double -> float 

CLCubXScene::CLCubXScene(void)
{
	
	clMenuManager = NULL;  // Cette classe devra etre retravailler pour etre mise en Singleton
	clTV3D = CLTV3DMoteur::getInstance();
	pTVScene = new CTVScene();
	InitTextures();
	
	//  Load the 3D Font
	LoadLettersAndNumbers();

	//   testing purpose
	pSoleil = pTVScene->CreateMeshBuilder("SoleilMesh");
	pSoleil->CreateBox(20,20,20, false);
	pSoleil->SetPosition(0,500,0);
	pSoleil->SetLightingMode(cTV_LIGHTING_MANAGED);
	pSoleil->SetColor(RED,true);

	clEffect = new CLEffect("media\\PartSys\\Explosion1.tvp", cTV_3DVECTOR(0,500,0),cTV_3DVECTOR(0,0,0), NULL, 0);

	clLand = new CLLand("media\\Heightmaps\\heightmapSplit.jpg", "media\\grass.bmp",16,16, cTV_3DVECTOR(-2048,0,-2048));

// variable pour permettre de detecter les bounding box   --  Pas encore utilisé
	//cTV_3DVECTOR BoxMin;
	//cTV_3DVECTOR BoxMax;
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


	// Locate and create the clock display
	DisplayPos.x = 600; DisplayPos.y = 975; DisplayPos.z = 0;
	ClockDisplay = new CLClockDisplay(DisplayPos,3,0,BLUE, st3DFont);
	//  Doit etre apres la création de ClockDisplay sinon affiche pas...!?!?!?!?   Pkoi?
	CreateMenuDomotique();  // Load les 4 Clock Displays



	clMenuManager = new CLMenuManager();  
	SetMenuButtons();
}

CLCubXScene::~CLCubXScene(void)
{
	if(clEffect) { delete (clEffect); clEffect = NULL; }
	if(clMenuManager){ delete(clMenuManager); clMenuManager = NULL; }
	if(LightTimeOn) { delete(LightTimeOn); LightTimeOn = NULL; }
	if(LightTimeOff) { delete(LightTimeOff); LightTimeOff = NULL; }
	if(FanTimeOn) { delete(FanTimeOn); FanTimeOn = NULL; }
	if(FanTimeOff) { delete(FanTimeOff); FanTimeOff = NULL; }
	if(ClockDisplay) { delete(ClockDisplay); ClockDisplay = NULL; }
}

void CLCubXScene::Render(void)
{

clLand->Render();
clEarth->Render();


	//  CE QUI A ETE RENDERER POUR LES EFFET DE LEAU N'ONT PAS BESOINS DETRE RE_RENDERER?????!!!!
				// A DEMANDER PKOI A TV3D CAR CEST RENDERER EN DEHORS DE CLEAR et RENDERonSCREEN
				pSoleil->Render(); 
				clCubX->AnimateCubX(clTV3D->GetTimeElapsed());
				clCubX->Render();

				clMenuManager->Render();
				ClockDisplay->Render();
				LightTimeOn->Render();
				LightTimeOff->Render();
				FanTimeOn->Render();
				FanTimeOff->Render();

	if(clEffect->IsFinished() == false)
			clEffect->Render(clTV3D->GetTimeElapsed()); 

}

void CLCubXScene::InitTextures(void)
{
	pTexFactory = new CTVTextureFactory();

	ClockTexture = pTexFactory->LoadTexture("Media\\gold_01.jpg","ClockTex",-1,-1,cTV_COLORKEY_NO,true);
	MarbleTexture = pTexFactory->LoadTexture("Media\\marble10.jpg","MarbleTex",-1,-1,cTV_COLORKEY_NO,true);
    
	//CloudsTexture = pTexFactory->LoadTexture("Media\\Clouds.dds","CloudsTex",-1,-1,cTV_COLORKEY_NO,true);
	//TexBAlarms = pTexFactory->LoadTexture("Media\\AlarmClockIcon.jpg","AlarmsTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBSystem = pTexFactory->LoadTexture("Media\\SystemIcon.jpg","SystemTex",-1,-1,cTV_COLORKEY_NO,true);
	//TexBTemp = pTexFactory->LoadTexture("Media\\TemperatureMonitorIcon.jpg","TempTex",-1,-1,cTV_COLORKEY_NO,true);
	//TexBAtmel = pTexFactory->LoadTexture("Media\\memoryChip.jpg","AtmelTex",-1,-1,cTV_COLORKEY_NO,true);
	TexBTools = pTexFactory->LoadTexture("Media\\ToolsIcon.png","ToolsTex",-1,-1,cTV_COLORKEY_NO,true);
	EarthTexture = pTexFactory->LoadTexture("Media\\Earth.jpg","EarthTex",-1,-1,cTV_COLORKEY_NO,true);
	EarthGridTex = pTexFactory->LoadTexture("Media\\EarthGridXL.bmp","EarthGridTex",-1,-1,cTV_COLORKEY_BLACK,true);
}

void CLCubXScene::CreateMenuDomotique(void)
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

void CLCubXScene::ShowTimeDisplays(void)
{
LightTimeOn->ShowDisplay();
LightTimeOff->ShowDisplay();
FanTimeOn->ShowDisplay();
FanTimeOff->ShowDisplay();
}

void CLCubXScene::HideTimeDisplays(void)
{
LightTimeOn->HideDisplay();
LightTimeOff->HideDisplay();
FanTimeOn->HideDisplay();
FanTimeOff->HideDisplay();
}


void CLCubXScene::Refresh3DClock(void)
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

void CLCubXScene::SetMenuButtons(void)
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




void CLCubXScene::CubXClickedAction(int iMeshClickedIndex)
{
	if(iMeshClickedIndex == clCubX->GetMesh()->iMeshIndex)  //  Menu 1
				{
					clCubX->iActualState = POSITIONNING;
					/*
					clMenuManager->iAncienNiveau = clMenuManager->iNiveauActif;
					clMenuManager->iNiveauActif = 1; // ctrl atmel
					ShowTimeDisplays();
					*/
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



			if(iMeshClickedIndex == clFleche->GetMesh()->iMeshIndex)    //  Main Menu
				{
					clMenuManager->iAncienNiveau = clMenuManager->iNiveauActif;
					clMenuManager->iNiveauActif = 0; // Main
					HideTimeDisplays();
				}	

	

}



void CLCubXScene::DomoticClickedAction(int iMeshClickedIndex)
{
	int i;
// Section Config des TimeDisplay
			if(iMeshClickedIndex == LightTimeOn->pDisplaySelect->iMeshIndex) 
				{
				LightTimeOn->ShowControls();
				pTVScene->SetCamera(LightTimeOn->DisplayPos.x - 50, LightTimeOn->DisplayPos.y + 50, LightTimeOn->DisplayPos.z -750,
											LightTimeOn->DisplayPos.x, LightTimeOn->DisplayPos.y, LightTimeOn->DisplayPos.z);
				}
			else if(iMeshClickedIndex == LightTimeOff->pDisplaySelect->iMeshIndex)
				{
				LightTimeOff->ShowControls();
				pTVScene->SetCamera(LightTimeOff->DisplayPos.x - 50, LightTimeOff->DisplayPos.y + 50, LightTimeOff->DisplayPos.z -750,
											LightTimeOff->DisplayPos.x, LightTimeOff->DisplayPos.y, LightTimeOff->DisplayPos.z);
				}
			else if(iMeshClickedIndex == FanTimeOn->pDisplaySelect->iMeshIndex)
				{
				FanTimeOn->ShowControls();
				pTVScene->SetCamera(FanTimeOn->DisplayPos.x - 50, FanTimeOn->DisplayPos.y + 50, FanTimeOn->DisplayPos.z -750,
											FanTimeOn->DisplayPos.x, FanTimeOn->DisplayPos.y, FanTimeOn->DisplayPos.z);
				}
			else if(iMeshClickedIndex == FanTimeOff->pDisplaySelect->iMeshIndex)
				{
				FanTimeOff->ShowControls();
				pTVScene->SetCamera(FanTimeOff->DisplayPos.x - 50, FanTimeOff->DisplayPos.y + 50, FanTimeOff->DisplayPos.z -750,
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


void CLCubXScene::LoadLettersAndNumbers(void)
{
int i;

char String[22]="Media\\alphabet\\ .tvm";


//  Load l'alphabet A-Z
for(i = 0; i < 26; i++)
  {
    String[15]=char(i) + 0x41;  // ajustement selon la table ascii
	st3DFont.Alphabet[i]= pTVScene->CreateMeshBuilder("Alphabet");
	st3DFont.Alphabet[i]->LoadTVM(String,true,true);	
	st3DFont.Alphabet[i]->RotateX(-90,true);
	//Alphabet[i]->SetMaterial(pTVGlobals->GetMat("matname"), -1);
	st3DFont.Alphabet[i]->Enable(false);
  }

// Load les chiffres  0-9
for(i=0;i<10;i++)
  {
	String[15] = char(i) + 0x30;    // ajustement selon la table ascii
	st3DFont.Numbers[i]= pTVScene->CreateMeshBuilder("Numbers");
	st3DFont.Numbers[i]->LoadTVM(String,true,true);
	st3DFont.Numbers[i]->RotateX(-90,true);
	//Numbers[i]->SetMaterial(pTVGlobals->GetMat("matname"), -1);
	st3DFont.Numbers[i]->Enable(false);
  }

st3DFont.SpecialChar[0] = pTVScene->CreateMeshBuilder("SpecialChar");   // A rajouter : Un vrai ':' et modifier inittime pour l'utiliser.
st3DFont.SpecialChar[0]->CreateSphere(10,8,8);
//SpecialChar[0]->SetMaterial(pTVGlobals->GetMat("matname"), -1);
st3DFont.SpecialChar[0]->Enable(false);
}

void CLCubXScene::WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color)
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
