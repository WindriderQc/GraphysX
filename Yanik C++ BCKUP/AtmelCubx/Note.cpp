#include "Note.h"

CLNote::CLNote(CTVMesh* OriginalNoteMesh)
{
	pTVScene = new CTVScene();

	pNoteMesh = OriginalNoteMesh->Duplicate("NoteMesh", true);
	//pNoteMesh->Enable(true);

}

CLNote::~CLNote(void)
{
}

void CLNote::SetPos(void)
{
	pNoteMesh->SetPosition(Pos.x, Pos.y, Pos.z);
}
