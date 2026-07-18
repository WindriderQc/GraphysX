#include "NotesManager.h"

// Initialisation du singleton à NULL
CLNotesManager *CLNotesManager::_singleton = NULL;


CLNotesManager::CLNotesManager(void)
{
	clDefaultNoteBlock = new CLBlocNote(50,50,10, "Default", 1);
}

CLNotesManager::~CLNotesManager(void)
{
	delete(clDefaultNoteBlock); clDefaultNoteBlock = NULL;
}

void CLNotesManager::Render(void)
{
	clDefaultNoteBlock->Render();

}
CTVMesh* CLNotesManager::GetAddNoteBtnMesh(void)
{
	return(clDefaultNoteBlock->pAddNoteBtn);
}

void CLNotesManager::AddNote(void)
{
	clDefaultNoteBlock->AddNote();


}