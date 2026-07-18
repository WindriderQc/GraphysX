#include "StdAfx.h"
#include "Anneau.h"

CLAnneau::CLAnneau(void)
{
int i;

for(i=0;i<100;i++)
  {
	stRings[i].fSize = 0.25;
	stRings[i].iPtsValue = 1;
	stRings[i].clRingMesh = new CL3DObject("media\\Ring.tvm", stRings[i].fSize, NULL, cTV_3DVECTOR(0,1000,0), true);
	stRings[i].clRingMesh->GetMesh()->SetColor(RGBA(1,1,0,1),true);
  }
}
	
CLAnneau::~CLAnneau(void)
{
	int i;
	for(i=0; i<100; i++)
	{
		delete(stRings[i].clRingMesh); stRings[i].clRingMesh = NULL;
	}
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
	return(stRings[RingIndex].clRingMesh->GetMesh()->GetPosition());
}
void CLAnneau::SetRingPos(cTV_3DVECTOR pos, int RingIndex)
{
	stRings[RingIndex].clRingMesh->GetMesh()->SetPosition(pos.x, pos.y, pos.z);
}

float CLAnneau::GetRingSize(int RingIndex)
{
	return(stRings[RingIndex].fSize);
}

void CLAnneau::SetRingSize(float size, int RingIndex)
{
	stRings[RingIndex].fSize = size;
	stRings[RingIndex].clRingMesh->GetMesh()->SetScale(size, size, size);
}

CTVMesh* CLAnneau::GetRingMesh(int RingIndex)
{
	return(stRings[RingIndex].clRingMesh->GetMesh());
}

void CLAnneau::Render(void)
{
int i;
for(i=0;i<100;i++)
  {
	  stRings[i].clRingMesh->GetMesh()->Render();
  }
}