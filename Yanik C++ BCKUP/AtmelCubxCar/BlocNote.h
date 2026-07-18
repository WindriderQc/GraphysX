#pragma once
#include "StdAfx.h"
#include "Note.h"

#define CAPACITY 50
#define DEPTH    10


class CLBlocNote
{
public:
	CLBlocNote(float Height, float Width, float Depth, char* name, int rank);
	~CLBlocNote(void);

	void Render(void);
	void AddNote(void);

	char* Name;
	int iRank;
	CLNote* clNotes[CAPACITY];
	int iActiveNotes;
	
	CTVMesh* pAddNoteBtn;

private:
	CTVScene* pTVScene;
	//CTVCamera* pCamera;
	CTVTextureFactory* pTexFactory;
	CTVMaterialFactory* pMatFactory;
	CTVMesh* pOriginalNoteMesh;

	
	int NoteTexture;
	int iNoteMaterial;

};

