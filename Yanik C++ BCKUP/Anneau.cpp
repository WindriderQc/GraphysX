#include "Anneau.h"

CLAnneau::CLAnneau(void)
{
int i;
OriginalRing = NULL;

pTVScene = new CTVScene();
OriginalRing = pTVScene->CreateMeshBuilder("Ring");
OriginalRing->CreateSphere(20,12,12);
OriginalRing->Enable(false);

for(i=0;i<100;i++)
  {
	stRings[i].Mesh = OriginalRing->Duplicate("Rings",true);
	stRings[i].Pos.x = 0;
	stRings[i].Pos.y = 0;
	stRings[i].Pos.z = 0;
	stRings[i].iSize = 1;
	stRings[i].iPtsValue = 1;
	stRings[i].Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	stRings[i].Mesh->SetColor(RGBA(1,1,0,1),true);
	stRings[i].Mesh->Enable(true);
  }
}
	
CLAnneau::~CLAnneau(void)
{

}

int CLAnneau::GetPtsValue(int RingIndex)
{
	return(stRings[RingIndex].iPtsValue);
}

void CLAnneau::SetPtsValue(int value, int RingIndex)
{
stRings[RingIndex].iPtsValue = value;
}

cTV_3DVECTOR CLAnneau::GetRingPos(int RingIndex)
{
	return(stRings[RingIndex].Pos);
}
void CLAnneau::SetRingPos(cTV_3DVECTOR pos, int RingIndex)
{
stRings[RingIndex].Pos = pos;
stRings[RingIndex].Mesh->SetPosition(pos.x, pos.y, pos.z);
}

int CLAnneau::GetRingSize(int RingIndex)
{
	return(stRings[RingIndex].iSize);
}

void CLAnneau::SetRingSize(int size, int RingIndex)
{
	stRings[RingIndex].iSize = size;
}

CTVMesh* CLAnneau::GetRingMesh(int RingIndex)
{
	return(stRings[RingIndex].Mesh);
}

void CLAnneau::Render(void)
{
int i;
for(i=0;i<100;i++)
  {
	  stRings[i].Mesh->Render();
  }
}