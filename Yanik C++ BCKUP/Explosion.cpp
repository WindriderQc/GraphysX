#include "Explosion.h"

CLExplosion::CLExplosion(void)
{
i=0;
Position.x = 0;
Position.y = 0;
Position.z = 0;
Offset.x = 0;
Offset.y = 0;
Offset.z = 0;

// a mettre dans un constructeur avec le nom du .TVP en param
fDuration = 0;       // 0 = Durée infinie
fTimerCount = 0;        //
bFinished = true;       //
MeshAttachedTo = NULL;  //


pTVScene = new CTVScene();
pPartSys = pTVScene->CreateParticleSystem("Explosion1");
pPartSys->Load("media\\PartSys\\Explosion1.tvp");
pPartSys->SetGlobalPosition(Position.x, Position.y, Position.z);
//pPartSys->Enable(false);  //  pkoi ca plante
}

CLExplosion::~CLExplosion(void)
{
	pPartSys->Destroy();
	if(pPartSys) DeleteTVObject(pPartSys);  // Vérifier si la difference entre le Destroy et DeleteTVObj()
	pPartSys = NULL;
	
	delete(pTVScene); pTVScene = NULL;
}

void CLExplosion::Render(float fTimeElapsed)
{

if(fTimerCount < fDuration)  
	fTimerCount += fTimeElapsed;

if(fTimerCount > fDuration)   // Arrete la répétition du systeme de particule
  {
   fTimerCount = fDuration;
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
i++;

if(i>8)   // petite loop pour s'assurer que des particule soit sortie lors de la vérification
 {
 int Count = pPartSys->GetGlobalParticleCount();
 if(Count == 0)  
  {												
   bFinished = true;    //  Bug car au démarrage, PartCount = automatiquement 0.
   i=0;
  }
 }

}

void CLExplosion::Start(void)
{
bFinished = false;
pPartSys->ResetAll();
}

bool CLExplosion::IsFinished(void)
{
return(bFinished);
}