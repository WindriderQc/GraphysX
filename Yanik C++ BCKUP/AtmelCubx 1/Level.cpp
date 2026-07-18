#include "Level.h"

// suppression de warning a la compilation 
#pragma warning(disable: 4244) // conversion double -> float 

CLLevel::CLLevel(void)
{
int i;
iLevelValue = 0;

pTVPhysics = new CTVPhysics();


clAnneaux = new CLAnneau();

for(i=0;i<100;i++)
  {
	VecPosAnneaux[i].x = 0 + i*10;   // Valeur de offset pour le fun de les voir croche au démarrage :)
	VecPosAnneaux[i].y = 0 + 100-i;
	VecPosAnneaux[i].z = 0 + i*10;
  }

}

CLLevel::~CLLevel(void)
{
delete(pTVPhysics); pTVPhysics = NULL;
delete(clAnneaux); clAnneaux = NULL;
}

void CLLevel::Render(void)
{
	if(clSlideObject->GetMesh()->IsEnabled())
	{
		clSlideObject->GetMesh()->Render();
		clAnneaux->Render();
	}
	
}

void CLLevel::LoadLevel(char* PathToSlide, int level)
{
clSlideObject = new CLSlideObject(PathToSlide, 1, cTV_3DVECTOR(0,-5000,0),true); 
pTVPhysics->EnableDebugInfo(clSlideObject->GetPhysicBody(),true,-1);
LoadRings();
}

void CLLevel::LoadRings(void)  //  Positionne les anneaux dans le tableau  // implementer pour avoir un XML qui defini les 100 positions
{
int i;
for(i=0;i<100;i++)
  {
	clAnneaux->SetRingPos(VecPosAnneaux[i],i);
  }
}

/////////////////////
//  GET/SET Method
/////////////////////
CTVMesh* CLLevel::GetSlideMesh(void)
{
	return(clSlideObject->GetMesh());
}

int CLLevel::GetSlidePhysicBody(void)
{
	return(clSlideObject->GetPhysicBody());
}

int CLLevel::GetLevelValue(void)
{
return(iLevelValue);
}