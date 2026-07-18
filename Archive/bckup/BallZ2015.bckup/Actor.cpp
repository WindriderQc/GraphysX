#include "stdafx.h"
#include "Actor.h"

//Initialisation du singleton à NULL
CLActorFactory *CLActorFactory::_singleton = NULL;

CLActor::CLActor()
{
	tvActor = CLTV3DMoteur::getInstance()->pTVScene->CreateActor("CubX");
	if (!tvActor->LoadTVA("C:\\Media\\CubX\\CubXScreensaver.tva")){
		MSGBOX("Actor", "Loading Error");
	}
	else
	{
		tvActor->SetScale(0.1, 0.1, 0.1);
		tvActor->SetPosition(20, 5, 10);
		//tvActor->Enable(true);
		tvActor->SetShadowCast(true, true);
		tvActor->SetLightingMode(cTV_LIGHTING_MANAGED);
		int animcount = tvActor->GetAnimationCount();
		char* test = tvActor->GetAnimationName(0);
		tvActor->PlayAnimation(1);
	}
}
CLActor::CLActor(char* Name, char* Path, cTV_3DVECTOR Pos)
{
	tvActor = CLTV3DMoteur::getInstance()->pTVScene->CreateActor(Name);
	if (!tvActor->LoadXFile(Path, true, true)){
		MSGBOX("Actor", "Loading Error");
	}
	else
	{
		tvActor->SetScale(1, 1, 1);
		tvActor->SetPosition(Pos.x, Pos.y, Pos.z);
		tvActor->Enable(true);
		tvActor->SetShadowCast(true, true);
		tvActor->SetLightingMode(cTV_LIGHTING_MANAGED);
		int animcount = tvActor->GetAnimationCount();
		char* test = tvActor->GetAnimationName(0);
		
		tvActor->PlayAnimation(0.005);
		tvActor->SetAnimationLoop(false);
		cTV_3DVECTOR min, max;
		tvActor->GetBoundingBox(&min, &max);
		//CLTV3DMoteur::getInstance()->DrawTV3DLine(min, max, YELLOW_RGBA);
	}
}
CLActor::~CLActor()
{}