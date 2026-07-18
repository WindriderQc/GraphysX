#include "CLBallZ.h"

CLBallZ::CLBallZ(void)
{
	clBallZShell = new CL3DObject("media\\Ball\\BallShell.tvm", 1, NULL, cTV_3DVECTOR(-20,310,225), true, 50);
	clBallZShell->SetOpacity(.5);
	clBallZShell->GetMesh()->SetAlphaTest();
	clBallZShell->GetMesh()->SetBlendingMode(cTV_BLEND_ALPHA);
	clBallZCtrl = new CL3DObject("media\\Ball\\BallCtrl.tvm", 0.8, NULL, cTV_3DVECTOR(-20,310,225), true, 50);
}

CLBallZ::~CLBallZ(void)
{
	delete(clBallZShell); clBallZShell = NULL;
	delete(clBallZCtrl); clBallZCtrl = NULL;

}

CTVMesh* CLBallZ::GetBallZMesh()
{
	return(clBallZShell->GetMesh()  );
}

cTV_3DVECTOR CLBallZ::GetBallZPos()
{
	return(clBallZShell->GetPosition());
}
void CLBallZ::SetBallZPos(cTV_3DVECTOR pos)
{
	clBallZShell->SetPosition(pos);
}



void CLBallZ::Render(void)
{

	  clBallZShell->Render();
	  clBallZCtrl->Render();
}