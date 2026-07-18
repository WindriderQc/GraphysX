#pragma once
#include "StdAfx.h"


class CLClockDisplay
{
public:
	CLClockDisplay(cTV_3DVECTOR Pos, double sizeFactor, double angle_y, int Color, ST3DFont st3DFont);
	~CLClockDisplay(void);

	
	int iTime[4];
	double size;

	CTVMesh* HrsDiz[10];
	CTVMesh* HrsUni[10];
	CTVMesh* MinDiz[10];
	CTVMesh* MinUni[10];
	CTVMesh* WrittenTime[4];

	CTVMesh* pLeftArrow; CTVMesh* pRightArrow; CTVMesh* pPlus; CTVMesh* pMinus; CTVMesh* pNumSelect;
	CTVMesh*pDisplaySelect; CTVMesh* pAccept;

	CTVMesh* p2pointHaut;
	CTVMesh* p2pointBas;

	cTV_3DVECTOR DisplayPos;
	int iSelectorIndex;

	void WriteTime(int Texture, int Material);
	void HideDisplay(void);
	void ShowDisplay(void);
	void HideControls(void);
	void ShowControls(void);
	void LoadDisplayControls(void);
	void SetNumSelectPos(void);
	void Render(void);

private:
	CTVGlobals* pTVGlobals;
	CTVScene* pTVScene;



};
