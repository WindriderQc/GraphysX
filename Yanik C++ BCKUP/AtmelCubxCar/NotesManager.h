#pragma once
#include "StdAfx.h"
#include "BlocNote.h"

//  INCLURE UN RENDER COMPLET INDEPENDANT DE TV3DMoteur qui pourrra etre caller directement de Simulation
//  rearranger aussi la gestion des input pour que clInput aille chercher l'info mais que chaque Manager
// gere indépendemment les action...   et mette un switch dans simulation qui décidera de quelle action caller
// selon le manager actif.->Setter un flag dans GlobalVar pour ca!1! :)

class CLNotesManager
{
private:
	CLNotesManager(void);
	~CLNotesManager(void);

public:
  // Interface publique
	void Render(void);
	
	CTVMesh* GetAddNoteBtnMesh(void);
	void AddNote(void);


	
  // Fonctions de création et destruction du singleton
  static CLNotesManager *getInstance ()
  {
    if (NULL == _singleton)
      {
        _singleton =  new CLNotesManager;
      }
    return _singleton;
  }

  static void kill ()
  {
    if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
  }



private:
	CLBlocNote* clDefaultNoteBlock;
	

	static CLNotesManager *_singleton;
};

