#pragma once
#include "StdAfx.h"

#define MAXBTN 48
#define MAXNIVEAU 16
#define ALLMENU 666
#define ALLMENU_EXCEPT_MAIN 777

class CLMenuManager
{
public:
	CLMenuManager(void);
	~CLMenuManager(void);

	struct STBouton3D
	{
	cTV_3DVECTOR Pos, dir; // a rajouter : matrix pour Physics
	int NiveauMenu;
	UINT Action;  //  message windows
	CTVMesh* MeshID;
	long Texture;
	}stBtn3D[MAXBTN];

	struct STMenu
	{
	cTV_3DVECTOR CamPos, CamDir;
	int Niveau;
    }stManagedMenu[MAXNIVEAU];

	int iNiveauActif;
	int iAncienNiveau;

	int NbrActuelBoutons;
	int NbrActuelMenus;
	void SetActiveMenu(void);
	void CreateBouton(CTVMesh* Mesh, int Niveau);
	void CreateManagedMenu(cTV_3DVECTOR Pos, cTV_3DVECTOR Dir, int Niveau);
	void Render(void);

};
