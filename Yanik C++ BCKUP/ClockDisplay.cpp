#include "ClockDisplay.h"

// suppression de warning a la compilation 
#pragma warning(disable: 4244) // conversion double -> float 

CLClockDisplay::CLClockDisplay(cTV_3DVECTOR Pos, double sizeFactor, double angle_y, int Color, ST3DFont st3DFont)
{
unsigned int i;

pTVGlobals = new CTVGlobals();
pTVScene = new CTVScene();


DisplayPos.x = Pos.x;
DisplayPos.y = Pos.y;
DisplayPos.z = Pos.z;
size = sizeFactor;
iSelectorIndex = 0;


for(i=0;i<10;i++)
{
HrsDiz[i] = st3DFont.Numbers[i]->Duplicate("HrsDiz", true);
HrsUni[i] = st3DFont.Numbers[i]->Duplicate("HrsUni", true);
MinDiz[i] = st3DFont.Numbers[i]->Duplicate("UniDiz", true);
MinUni[i] = st3DFont.Numbers[i]->Duplicate("MinUni", true);
}

for(i=0;i<10;i++)
{
HrsDiz[i]->SetPosition(DisplayPos.x, DisplayPos.y, DisplayPos.z);
HrsDiz[i]->RotateY(angle_y,true);  // Rotate around global Y axis.
HrsDiz[i]->SetScale(size,size,size);
HrsDiz[i]->SetColor(Color,true);
}

DisplayPos.x = DisplayPos.x+(55*size) ;   // 55 = decallage entre les lettres * Size pour fitter avec la grosseur des lettres 

for(i=0;i<10;i++)
{
HrsUni[i]->SetPosition(DisplayPos.x, DisplayPos.y, DisplayPos.z);
HrsUni[i]->RotateY(angle_y,false);  // Rotate around global Y axis.
HrsUni[i]->SetScale(size,size,size);
HrsUni[i]->SetColor(Color,true);
}

DisplayPos.x = DisplayPos.x+(55*size) ;    

p2pointHaut = st3DFont.SpecialChar[0]->Duplicate("2pointH", true);
p2pointHaut->SetPosition(DisplayPos.x,DisplayPos.y + size*50,DisplayPos.z);
p2pointHaut->SetScale(size,size,size);
p2pointHaut->SetColor(Color,true);

p2pointBas = st3DFont.SpecialChar[0]->Duplicate("2pointB", true);
p2pointBas->SetPosition(DisplayPos.x,DisplayPos.y + size*15,DisplayPos.z);
p2pointBas->SetScale(size,size,size);
p2pointBas->SetColor(Color,true);


DisplayPos.x = DisplayPos.x+(55*size) ;

for(i=0;i<10;i++)
{
MinDiz[i]->SetPosition(DisplayPos.x, DisplayPos.y, DisplayPos.z);
MinDiz[i]->RotateY(angle_y,true);  // Rotate around global Y axis.
MinDiz[i]->SetScale(size,size,size);
MinDiz[i]->SetColor(Color,true);
}

DisplayPos.x = DisplayPos.x+(55*size) ;  

for(i=0;i<10;i++)
{
MinUni[i]->SetPosition(DisplayPos.x, DisplayPos.y, DisplayPos.z);
MinUni[i]->RotateY(angle_y,true);  // Rotate around global Y axis.
MinUni[i]->SetScale(size,size,size);
MinUni[i]->SetColor(Color,true);
}

WrittenTime[0]= HrsDiz[0];
WrittenTime[0]->Enable(true);
WrittenTime[1]= HrsUni[0];
WrittenTime[1]->Enable(true);
p2pointHaut->Enable(true);
p2pointBas->Enable(true);
WrittenTime[2]= MinDiz[0];
WrittenTime[2]->Enable(true);
WrittenTime[3]= MinUni[0];
WrittenTime[3]->Enable(true);


DisplayPos.x = Pos.x;   // ¨Pour remettre les bonne valeur a DisplayPOs.....  pas tres propre :)  a arranger
DisplayPos.y = Pos.y;
DisplayPos.z = Pos.z;

LoadDisplayControls();

}

CLClockDisplay::~CLClockDisplay(void)
{
int i;

for(i=0; i<4; i++) 
	{
	WrittenTime[i] = NULL;
	}
/*
for(i=0;i<10;i++)    //   Crash  Pkoi????
{
	HrsDiz[i]->Destroy();  HrsDiz[i] = NULL;
	HrsUni[i]->Destroy();  HrsUni[i] = NULL;
	MinDiz[i]->Destroy();  MinDiz[i] = NULL;
	MinUni[i]->Destroy();  MinUni[i] = NULL;
}
p2pointHaut->Destroy();  p2pointHaut = NULL;
p2pointBas->Destroy();  p2pointBas = NULL;
*/

delete(pTVGlobals);
pTVGlobals = NULL;
delete(pTVScene);
pTVScene = NULL;

}



void CLClockDisplay::WriteTime(int Texture, int Material)
{//  iTime[] est la valeur que cette fonction affiche
unsigned int i;

for(i=0; i<4; i++) 
{
  WrittenTime[i]->Enable(false);
}

HrsDiz[iTime[0]]->Enable(true);
HrsDiz[iTime[0]]->SetTexture(Texture,-1);
if(Material != NULL)
	HrsDiz[iTime[0]]->SetMaterial(Material, -1);
WrittenTime[0]= HrsDiz[iTime[0]];

HrsUni[iTime[1]]->Enable(true);
HrsUni[iTime[1]]->SetTexture(Texture,-1);
if(Material != NULL)
	HrsUni[iTime[1]]->SetMaterial(Material, -1);
WrittenTime[1]= HrsUni[iTime[1]];

p2pointHaut->SetTexture(Texture,-1);
p2pointBas->SetTexture(Texture,-1);

if(Material != NULL)
{
	p2pointHaut->SetMaterial(Material, -1);
	p2pointBas->SetMaterial(Material, -1);
}

MinDiz[iTime[2]]->Enable(true);
MinDiz[iTime[2]]->SetTexture(Texture,-1);
if(Material != NULL)
	MinDiz[iTime[2]]->SetMaterial(Material, -1);
WrittenTime[2]= MinDiz[iTime[2]];

MinUni[iTime[3]]->Enable(true);
MinUni[iTime[3]]->SetTexture(Texture,-1);
if(Material != NULL)
	MinUni[iTime[3]]->SetMaterial(Material, -1);
WrittenTime[3]= MinUni[iTime[3]];

}

void CLClockDisplay::HideDisplay(void)
{
unsigned int i;

for(i=0; i<4; i++) 
	{
	WrittenTime[i]->Enable(false);
	}
p2pointHaut->Enable(false);
p2pointBas->Enable(false);

}
void CLClockDisplay::ShowDisplay(void)
{
unsigned int i;

for(i=0; i<4; i++) 
	{
	WrittenTime[i]->Enable(true);
	}
p2pointHaut->Enable(true);
p2pointBas->Enable(true);
}


void CLClockDisplay::HideControls(void)
{
	pLeftArrow->Enable(false);
	pRightArrow->Enable(false);
	pPlus->Enable(false);
	pMinus->Enable(false);
	pNumSelect->Enable(false);
	pAccept->Enable(false);

	pDisplaySelect->SetCollisionEnable(true, -1, cTV_TESTTYPE_DEFAULT);
}

void CLClockDisplay::ShowControls(void)
{
	pLeftArrow->Enable(true);
	pRightArrow->Enable(true);
	pPlus->Enable(true);
	pMinus->Enable(true);
	pNumSelect->Enable(true);
	pAccept->Enable(true);

	pDisplaySelect->SetCollisionEnable(false, -1, cTV_TESTTYPE_DEFAULT);
}

void CLClockDisplay::LoadDisplayControls(void)
{
float Alpha = 1;

pLeftArrow = pTVScene->CreateMeshBuilder("LeftArrow");
pLeftArrow->LoadTVM("Media\\Fleche.tvm", false,false);
pLeftArrow->SetPosition(DisplayPos.x + 25*size, DisplayPos.y + 150*size, DisplayPos.z);
pLeftArrow->SetScale(0.6*size, 0.6*size, 0.6*size);
pLeftArrow->RotateZ(90,false);
pLeftArrow->SetLightingMode(cTV_LIGHTING_MANAGED);
pLeftArrow->SetBlendingMode( cTV_BLEND_ALPHA, -1);
pLeftArrow->SetColor(RGBA(0,0,1,Alpha), true);

pRightArrow = pLeftArrow->Duplicate("RightArrow", true);
pRightArrow->SetPosition(DisplayPos.x + 175*size, DisplayPos.y + 150*size, DisplayPos.z);
pRightArrow->RotateZ(180,false);
pRightArrow->SetLightingMode(cTV_LIGHTING_MANAGED);
pRightArrow->SetBlendingMode( cTV_BLEND_ALPHA, -1);
pRightArrow->SetColor(RGBA(0,0,1,Alpha), true);

pPlus = pTVScene->CreateMeshBuilder("Plus");
pPlus->LoadTVM("Media\\Plus.tvm", false,false);
pPlus->SetPosition(DisplayPos.x + 350*size, DisplayPos.y + 75*size, DisplayPos.z);
pPlus->SetScale(5*size,5*size,5*size);
pPlus->SetLightingMode(cTV_LIGHTING_MANAGED);
pPlus->SetBlendingMode( cTV_BLEND_ALPHA, -1);
pPlus->SetColor(RGBA(0,0,1,Alpha), true);

pMinus = pTVScene->CreateMeshBuilder("Minus"); 
pMinus->CreateBox(80* size,15* size,10* size,false);
pMinus->SetPosition(DisplayPos.x + 350*size, DisplayPos.y - 25*size , DisplayPos.z);
pMinus->SetLightingMode(cTV_LIGHTING_MANAGED);
pMinus->SetBlendingMode( cTV_BLEND_ALPHA, -1);
pMinus->SetColor(RGBA(0,0,1,Alpha), true);

pNumSelect = pTVScene->CreateMeshBuilder("NumSelect"); 
pNumSelect->LoadTVM("Media\\NumSelect.tvm", false,false);
pNumSelect->SetScale(1.25*size,1.25*size,1.75*size);
pNumSelect->SetLightingMode(cTV_LIGHTING_MANAGED);
pNumSelect->SetBlendingMode( cTV_BLEND_ALPHA, -1);
pNumSelect->SetColor(RGBA(0,1,0,Alpha), true);

pDisplaySelect = pTVScene->CreateMeshBuilder("DisplaySelect");
pDisplaySelect->CreateBox(300*size,125*size,10*size,false); 
pDisplaySelect->SetPosition(DisplayPos.x + 125*size, DisplayPos.y + 35*size, DisplayPos.z - 25);
pDisplaySelect->SetLightingMode(cTV_LIGHTING_MANAGED);
pDisplaySelect->SetAlphaTest(true, 0,true,-1);
pDisplaySelect->SetBlendingMode( cTV_BLEND_ALPHA, -1);
pDisplaySelect->SetColor(RGBA(1,1,1,0), false);   // setter alpha a 0 pour etre totalement transparent 

pAccept = pTVScene->CreateMeshBuilder("Accept");
pAccept->LoadTVM("Media\\Accept.tvm", true,true);
pAccept->SetPosition(DisplayPos.x + 500 *size, DisplayPos.y + 25*size, DisplayPos.z);
pAccept->SetScale(0.8*size,0.8*size,0.8*size);
pAccept->SetLightingMode(cTV_LIGHTING_MANAGED);

SetNumSelectPos();
HideControls();
}
void CLClockDisplay::SetNumSelectPos(void)
{
	cTV_3DVECTOR tmpvec;

switch(iSelectorIndex)
	{
case 0:
	tmpvec = HrsDiz[0]->GetPosition();
	pNumSelect->SetPosition(tmpvec.x, tmpvec.y + 25*size, tmpvec.z);
	break;
case 1:
	tmpvec = HrsUni[0]->GetPosition();
	pNumSelect->SetPosition(tmpvec.x, tmpvec.y + 25*size, tmpvec.z);
	break;
case 2:
	tmpvec = MinDiz[0]->GetPosition();
	pNumSelect->SetPosition(tmpvec.x, tmpvec.y + 25*size, tmpvec.z);
	break;
case 3:
	tmpvec = MinUni[0]->GetPosition();
	pNumSelect->SetPosition(tmpvec.x, tmpvec.y + 25*size, tmpvec.z);
	break;

	}
}
void CLClockDisplay::Render(void)
{
	if(HrsDiz[iTime[0]]->IsEnabled())
		HrsDiz[iTime[0]]->Render();
	if(HrsUni[iTime[1]]->IsEnabled())
		HrsUni[iTime[1]]->Render();
	if(p2pointHaut->IsEnabled())
		p2pointHaut->Render();
	if(p2pointBas->IsEnabled())
		p2pointBas->Render();
	if(MinDiz[iTime[2]]->IsEnabled())
		MinDiz[iTime[2]]->Render();
	if(MinUni[iTime[3]]->IsEnabled())
		MinUni[iTime[3]]->Render();

	if(pLeftArrow->IsEnabled())
		pLeftArrow->Render();
	if(pRightArrow->IsEnabled())
		pRightArrow->Render();
	if(pPlus->IsEnabled())
		pPlus->Render();
	if(pMinus->IsEnabled())
		pMinus->Render(); 
	if(pNumSelect->IsEnabled())
		pNumSelect->Render();
	if(pAccept->IsEnabled())
		pAccept->Render();
	/*
	if(pDisplaySelect->IsEnabled())
		pDisplaySelect->Render();
		*/

}