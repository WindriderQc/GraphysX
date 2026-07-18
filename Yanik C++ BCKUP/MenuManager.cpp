#include ".\menumanager.h"


// Reste a implementer la suppression de bouton et menu.

CLMenuManager::CLMenuManager(void)
{
int i, j;
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

for(i = 0; i<MAXNIVEAU;i++)
	{
	for(j = 0; j<MAX_OBJ_PASSIF;j++)
		{
			stObjPassifs[i].ListeMeshesPassifs[j] = NULL;

		}
	}


}

CLMenuManager::~CLMenuManager(void)
{
}


void CLMenuManager::ShowActiveMenu(CTVScene* pScene)
{
int i,j;
//if(iNiveauActif != iAncienNiveau)    Ce test était pour économiser du process et ne pas fair tout le reste a chaque render mais maintenant cette fonction fait le reder donc doit etre executé tout le temps...   ptete que le render devrait se faire ailleur....
//	{

	/* // Moves camera according to the menu position
	for(i=0; i<MAXNIVEAU;i++)
	{
	if(stManagedMenu[i].Niveau == iNiveauActif)
		pScene->SetCamera(stManagedMenu[i].CamPos.x, stManagedMenu[i].CamPos.y, stManagedMenu[i].CamPos.z,
								stManagedMenu[i].CamDir.x, stManagedMenu[i].CamDir.y, stManagedMenu[i].CamDir.z);
	}*/


	//  Show Actives Buttons per menu
	for(i=0; i<MAXBTN;i++)
		{
		if(stBtn3D[i].MeshID != NULL)
		 {
			if((stBtn3D[i].NiveauMenu == iNiveauActif) || (stBtn3D[i].NiveauMenu == ALLMENU))
			{
				stBtn3D[i].MeshID->Enable(true);
				stBtn3D[i].MeshID->Render();
				//stBtn3D[i].MeshID->SetCollisionEnable(true,-1);
			}
			else
				{	
				//stBtn3D[i].MeshID->SetCollisionEnable(false,-1);
				stBtn3D[i].MeshID->Enable(false);
				}
		 }
		}// for


	//  Show Passives Object per menu
	for(i=0; i<MAXNIVEAU;i++)
		{
		for(j = 0; j<MAX_OBJ_PASSIF;j++)
			{
			if(stObjPassifs[i].ListeMeshesPassifs[j] != NULL)
			  {
				if(i == iNiveauActif)
				  {
					stObjPassifs[i].ListeMeshesPassifs[j]->Enable(true);
					stObjPassifs[i].ListeMeshesPassifs[j]->Render();
				  }
				else
					stObjPassifs[i].ListeMeshesPassifs[j]->Enable(false);
			  }	
			}
		}	

	iAncienNiveau = iNiveauActif;
//	}
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
