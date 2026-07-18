#pragma once
#include "StdAfx.h"

#define MAXBTN 32
#define MAXNIVEAU 8
#define MAX_OBJ_PASSIF 32
#define ALLMENU 666

class CLMenuManager
{
public:
	CLMenuManager(void);
	~CLMenuManager(void);

	struct STBouton3D
	{
	cTV_3DVECTOR Pos, dir; // a rajouter : matrix pour ODE
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

	//ITVCamera CamID;

	int iNiveauActif;
	int iAncienNiveau;



	CTVMesh* TableMeshBouton[MAXBTN];
	CTVMesh* TableMeshBoutonActif[MAXBTN];

	struct STObjPassifs
	{
	CTVMesh* ListeMeshesPassifs[MAX_OBJ_PASSIF];
	}stObjPassifs[MAXNIVEAU];

	int NbrActuelBoutons;
	int NbrActuelMenus;
	void ShowActiveMenu(CTVScene* pScene);
	void CreateBouton(CTVMesh* Mesh, int Niveau);
	void CreateManagedMenu(cTV_3DVECTOR Pos, cTV_3DVECTOR Dir, int Niveau);

};
