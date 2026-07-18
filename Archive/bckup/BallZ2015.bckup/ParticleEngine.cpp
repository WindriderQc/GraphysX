#include "StdAfx.h"
#include "ParticleEngine.h"

CLParticleEngine::CLParticleEngine()
{
	clGlobalVar = CLGlobalVar::getInstance();
	clExplosion = new CLExplosion();
	clExplosion->Start();
	//clEffect = new CLEffect();
}
CLParticleEngine::~CLParticleEngine()
{
}
void CLParticleEngine::UpdateRender()
{
	clExplosion->Render(clGlobalVar->fTimeElapsed);
}

CLExplosion::CLExplosion(void)
{
	i = 0;
	Position.x = -5;
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
	if (pPartSys) DeleteTVObject(pPartSys);  // Vérifier si la difference entre le Destroy et DeleteTVObj()
	pPartSys = NULL;

	delete(pTVScene); pTVScene = NULL;
}
void CLExplosion::Render(float fTimeElapsed)
{

	if (fTimerCount < fDuration)
		fTimerCount += fTimeElapsed;

	if (fTimerCount > fDuration)   // Arrete la répétition du systeme de particule
	{
		fTimerCount = fDuration;
		iEmitterCount = pPartSys->GetEmitterCount();
		for (i = 0; i < iEmitterCount; i++)
		{
			pPartSys->SetEmitterLooping(i, false);
		}
	}

	if (MeshAttachedTo != NULL)
	{
		Position = Offset + MeshAttachedTo->GetPosition();
		pPartSys->SetGlobalPosition(Position.x, Position.y, Position.z);
	}

	pPartSys->Render();
	pPartSys->Update();
	i++;

	if (i>8)   // petite loop pour s'assurer que des particule soit sortie lors de la vérification
	{
		int Count = pPartSys->GetGlobalParticleCount();
		if (Count == 0)
		{
			bFinished = true;    //  Bug car au démarrage, PartCount = automatiquement 0.
			i = 0;
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

CLEffect::CLEffect(char* PathToTVM, cTV_3DVECTOR position, cTV_3DVECTOR offset, CTVMesh* AttachedTo, int duration)
{
	Position.x = position.x;
	Position.y = position.y;
	Position.z = position.z;
	Offset.x = offset.x;
	Offset.y = offset.y;
	Offset.z = offset.z;

	fDuration = float(duration);
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
	if (pPartSys) DeleteTVObject(pPartSys);  // Vérifier si la difference entre le Destroy et DeleteTVObj()
	pPartSys = NULL;

	delete(pTVScene); pTVScene = NULL;
}
void CLEffect::Render(float fTimeElapsed)
{
	int i;

	if (fTimerCount <= fDuration)
	{
		fTimerCount += fTimeElapsed;
	}
	else   // Arrete la répétition du systeme de particule
	{
		fTimerCount = fDuration;  //  Probablement a enlever....  sert a rien!?!?
		iEmitterCount = pPartSys->GetEmitterCount();
		for (i = 0; i < iEmitterCount; i++)
		{
			pPartSys->SetEmitterLooping(i, false);
		}
	}

	if (MeshAttachedTo != NULL)
	{
		Position = Offset + MeshAttachedTo->GetPosition();
		pPartSys->SetGlobalPosition(Position.x, Position.y, Position.z);
	}

	pPartSys->Render();
	pPartSys->Update();

	int Count = pPartSys->GetGlobalParticleCount();

	if (Count > 0)
	{
		bSystemStarted = true;
	}

	if ((Count == 0) && bSystemStarted)
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