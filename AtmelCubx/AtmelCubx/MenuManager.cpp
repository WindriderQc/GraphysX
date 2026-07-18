#include ".\menumanager.h"


// Reste a implementer la suppression de bouton et menu.

CLMenuManager::CLMenuManager(void)
{
int i;
iNiveauActif = 0;
iAncienNiveau = 0;
NbrActuelBoutons = 0;
NbrActuelMenus = 0;

for(i=0; i<MAXBTN; i++)
   {
	stBtn3D[i].MeshID = NULL;
	stBtn3D[i].NiveauMenu =-1;
   }
for(i=0; i<MAXNIVEAU; i++)
   {
	stManagedMenu[i].Niveau =-1;
   }
}

CLMenuManager::~CLMenuManager(void)
{
}


void CLMenuManager::SetActiveMenu(void)
{
int i;
if(iNiveauActif != iAncienNiveau) 
{
	//  Enables Actives Buttons per menu
	for(i=0; i<MAXBTN;i++)
		{
		if(stBtn3D[i].MeshID != NULL)
		 {
			if((stBtn3D[i].NiveauMenu == iNiveauActif) || (stBtn3D[i].NiveauMenu == ALLMENU))
			{
				stBtn3D[i].MeshID->Enable(true);
			}
			else if(stBtn3D[i].NiveauMenu == ALLMENU_EXCEPT_MAIN)
			{
			 if(iNiveauActif != 0)
				 stBtn3D[i].MeshID->Enable(true);
			}
			else
				{	
				stBtn3D[i].MeshID->Enable(false);
		  		}
		 }
		}// for

	iAncienNiveau = iNiveauActif;
	}
}

void CLMenuManager::CreateBouton(CTVMesh* Mesh, int Niveau)
{
if(NbrActuelBoutons != MAXBTN)
	{
	stBtn3D[NbrActuelBoutons].MeshID = Mesh;
	stBtn3D[NbrActuelBoutons].NiveauMenu = Niveau;
	
	cTV_3DVECTOR tmpvec = Mesh->GetPosition();
	stBtn3D[NbrActuelBoutons].Pos.x = tmpvec.x;
	stBtn3D[NbrActuelBoutons].Pos.y = tmpvec.y;
	stBtn3D[NbrActuelBoutons].Pos.z = tmpvec.z;
	
	NbrActuelBoutons++;
	}
}

void CLMenuManager::CreateManagedMenu(cTV_3DVECTOR Pos, cTV_3DVECTOR Dir, int Niveau)
{
if(NbrActuelMenus != MAXNIVEAU)
	{
	stManagedMenu[NbrActuelMenus].CamPos = Pos;
	stManagedMenu[NbrActuelMenus].CamDir = Dir;
	stManagedMenu[NbrActuelMenus].Niveau = Niveau;
	NbrActuelMenus++;
	}
}

void CLMenuManager::Render(void)
{
int i;

for(i=0;i < NbrActuelBoutons;i++)
{
	if(stBtn3D[i].MeshID->IsEnabled())
		stBtn3D[i].MeshID->Render();
}
}