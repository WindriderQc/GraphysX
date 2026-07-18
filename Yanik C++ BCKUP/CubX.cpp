#include "StdAfx.h"
#include "CubX.h"

CLCubX::CLCubX(void)
{
	pScene = new CTVScene();

	
	pCubX = new CTVActor();
	pCubX = pScene->CreateActor("CubX");
	
	int ans = pCubX->LoadTVA("media\\CubXScreensaver.tva", true, true);
	pCubX->Enable(true);
	if(ans)
		pCubX->PlayAnimation(1.000);

}

CLCubX::~CLCubX(void)
{
}
