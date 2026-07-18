#include "BlocNote.h"

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 

CLBlocNote::CLBlocNote(float Height, float Width, float Depth, char* name, int rank)
{
	int i, Column, ColumnRank;

	Name = name;
	iRank = rank;
	iActiveNotes = 0;

	pTVScene = new CTVScene();
	pTexFactory	= new CTVTextureFactory();
	pMatFactory = new CTVMaterialFactory();

	// Load the Mesh Texture
	NoteTexture = pTexFactory->LoadTexture("Media\\marble10.jpg","MarbleTex",-1,-1,cTV_COLORKEY_NO,true);

	// Create the Standard lighting material
	iNoteMaterial = pMatFactory->CreateMaterial("NoteMaterial");
	pMatFactory->SetAmbient(iNoteMaterial, 0.1, 0.1, 0.1, 1);
	pMatFactory->SetDiffuse(iNoteMaterial, 0.8, 0.8, 0.8, 1);
	pMatFactory->SetSpecular(iNoteMaterial, 0.2, 0.2, 0.2, 1);
	pMatFactory->SetPower(iNoteMaterial, 20);
	pMatFactory->SetEmissive(iNoteMaterial, 0, 0, 0, 1);
	
	//  Create and set Mesh
	pOriginalNoteMesh = pTVScene->CreateMeshBuilder("OriginalNoteMesh");
	pOriginalNoteMesh->CreateBox(Width, Height,Depth, false);
	pOriginalNoteMesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	pOriginalNoteMesh->SetMaterial(iNoteMaterial, -1);
	pOriginalNoteMesh->SetShadowCast(true,true);
	pOriginalNoteMesh->SetTexture(NoteTexture, -1);
	pOriginalNoteMesh->Enable(false);
	
	pAddNoteBtn = pTVScene->CreateMeshBuilder("AddNoteBtn");
	pAddNoteBtn->CreateBox(30, 30,30, false);
	pAddNoteBtn->SetLightingMode(cTV_LIGHTING_MANAGED);
	pAddNoteBtn->SetMaterial(iNoteMaterial, -1);
	pAddNoteBtn->SetShadowCast(true,true);
	pAddNoteBtn->SetTexture(NoteTexture, -1);
	pAddNoteBtn->SetPosition(0,100,0);

	//  Creates all the notes and set their locations
	for(i=0; i < CAPACITY; i++)
	{
	clNotes[i] = new CLNote(pOriginalNoteMesh);
	
	// Determines all notes positions value
	Column = i/DEPTH ;
	ColumnRank = i % DEPTH;
	clNotes[i]->Pos.x =  Column * (Width + Width/2) ;
	clNotes[i]->Pos.y = iRank*Height + (iRank-1)*Height ;
	clNotes[i]->Pos.z = ColumnRank*Depth ;
	if(ColumnRank > 0)
		clNotes[i]->Pos.z = clNotes[i]->Pos.z + (ColumnRank)*Depth;
	
	clNotes[i]->SetPos();
	}
}

CLBlocNote::~CLBlocNote(void)
{

delete(pTVScene);		pTVScene = NULL; 
delete(pTexFactory);	pTexFactory = NULL;
delete(pMatFactory);	pMatFactory = NULL;

}


void CLBlocNote::Render(void)
{
int i;
for(i=0; i < CAPACITY; i++)
  {
	  if(clNotes[i]->pNoteMesh->IsEnabled())
		clNotes[i]->pNoteMesh->Render();
  }

pAddNoteBtn->Render();
}


void CLBlocNote::AddNote(void)
{
if(iActiveNotes < CAPACITY)
  {
	clNotes[iActiveNotes]->pNoteMesh->Enable(true);
	iActiveNotes++;
  }
else
  {
	  //  Afficher un message qui avise que le bloc est plein
  }
}
