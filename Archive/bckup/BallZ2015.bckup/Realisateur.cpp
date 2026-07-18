#include "stdafx.h"
#include "Realisateur.h"

// Initialisation du singleton à NULL
CLRealisateur *CLRealisateur::_singleton = NULL;

CLRealisateur::CLRealisateur()
{

	clBallZScene = new CLBallZScene();//  Create the 3D Scene
	pActiveScene = clBallZScene;
}


CLRealisateur::~CLRealisateur()
{
	pActiveScene = NULL;
	if (clBallZScene) delete(clBallZScene); clBallZScene = NULL; 
}

CLBallZScene* CLRealisateur::GetActiveScene()
{
	return(pActiveScene);
}
void CLRealisateur::OnTourne()
{
	pActiveScene->Render();
}
