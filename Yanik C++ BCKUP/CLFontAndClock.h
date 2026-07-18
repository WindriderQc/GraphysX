#pragma once
#include "StdAfx.h"


class CLFontAndClock
{
public:
	CLFontAndClock(void);
	~CLFontAndClock(void);

	void LoadLettersNNumbers();
//	void InitTime(double size, double x, double y, double z, double angle_y, long Color);
	void WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color);
	
	
/*	ITVMesh HrsDiz[10];
	ITVMesh HrsUni[10];
	ITVMesh MinDiz[10];
	ITVMesh MinUni[10];
	ITVMesh WrittenTime[4];*/

	CTVGlobals* pTVGlobals;
	CTVScene* pTVScene;


};

class CLTimeDisplay 
{
	public:
	CLTimeDisplay(cTV_3DVECTOR Pos, double sizeFactor, double angle_y, int Color);
	~CLTimeDisplay(void);

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

	CTVGlobals* pTVGlobals;
	CTVScene* pTVScene;

};