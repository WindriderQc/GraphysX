#include "Effect.h"

CLEffect::CLEffect(char* PathToTVM, cTV_3DVECTOR position, cTV_3DVECTOR offset,  CTVMesh* AttachedTo, int duration )
{

Position.x = position.x;
Position.y = position.y;
Position.z = position.z;
Offset.x = offset.x;
Offset.y = offset.y;
Offset.z = offset.z;

fDuration = duration;  
fTimerCount = 0;        
bFinished = true;  
bSystemStarted = false; 
MeshAttachedTo = AttachedTo;  


pTVScene = new CTVScene();
pPartSys = pTVScene->CreateParticleSystem("Effect");
pPartSys->Load(PathToTVM); 
pPartSys->SetGlobalPosition(Position.x, Position.y, Position.z);
//pPartSys->Enable(false);  //  pkoi ca plante
}

CLEffect::~CLEffect(void)
{
	pPartSys->Destroy();
	if(pPartSys) DeleteTVObject(pPartSys);  // Vérifier si la difference entre le Destroy et DeleteTVObj()
	pPartSys = NULL;
	
	delete(pTVScene); pTVScene = NULL;
}

void CLEffect::Render(float fTimeElapsed)
{
int i;

if(fTimerCount <= fDuration)  
  {
	fTimerCount += fTimeElapsed;
  }
else   // Arrete la répétition du systeme de particule
  {
   fTimerCount = fDuration;  //  Probablement a enlever....  sert a rien!?!?
   iEmitterCount = pPartSys->GetEmitterCount();
   for(i=0; i < iEmitterCount; i++)
	{
     pPartSys->SetEmitterLooping(i, false);   
	}
  }

if(MeshAttachedTo != NULL)
  {
	  Position = Offset + MeshAttachedTo->GetPosition();
      pPartSys->SetGlobalPosition(Position.x, Position.y, Position.z);
    }

pPartSys->Render();
pPartSys->Update();

 int Count = pPartSys->GetGlobalParticleCount();
 
 if(Count > 0)
	{
     bSystemStarted = true;
	}
 
 if((Count == 0)  &&  bSystemStarted)  
  {												
	bFinished = true;
	bSystemStarted = false;
  }
 
 
}

void CLEffect::Start(void)
{
bFinished = false;
pPartSys->ResetAll();
}

bool CLEffect::IsFinished(void)
{
return(bFinished);
}