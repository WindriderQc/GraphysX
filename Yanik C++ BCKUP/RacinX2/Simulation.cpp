#include "simulation.h"



// Constructeur
Simulation::Simulation(void) 

{
	clCEGUI = NULL;
	
	
	clGlobalVar = CLGlobalVar::getInstance();
	clGlobalVar->ConfigBooleans();

	clClock = CLClock::getInstance();
	
}

// Destructeur
Simulation::~Simulation(void)
{
	

	clCEGUI->kill();
	clGlobalVar->kill();
	clArea->kill();
	clInput->kill();
	// Last Step, Kill the 3D Engine...
	clTV3D->kill();
}
// Méthode d'initialisation du simulateur
bool Simulation::initialiser(HWND WindowHandle, char *srchpath)
{
	WindowHWND = WindowHandle;
		
	clTV3D = CLTV3DMoteur::getInstance();
	clTV3D->Init3DEngine(WindowHWND, srchpath);
	
	clInput = CLInput::getInstance();
	
	

	if(clGlobalVar->IsCEGUIEnabled())
	{
		clCEGUI = CLTV3DCEGUI::getInstance(); // Initialisation du GUI  
	}

	clArea = CLArea::getInstance();  // Initialize l'Area qui selectionne aussi un GUI...
	return true;
} 

// Méthode d'animation du simulateur
void Simulation::animer(void)
{
float TimeElapsed = clTV3D->GetTimeElapsed();	

// Faire uneclasse Scene de laquelle les autre hériterons, et initialiser un pointeur Scene 
// en question et utiliser cela dans un fonction plus général ;)
// Meme chose pour la Scene :)


	long lTimeOfDay = clClock->getTimeOfDay();
	clArea->RenderArea(TimeElapsed, lTimeOfDay);
	// 1- GET OPPONENT LOCATION ON NETWORK COMM
	// 2- UPDATE OPPONENT CAR POSITION IN SCENE
			
	//CLCarsManager::getInstance()-
	if(clArea->clCarScene)
	{
//	clArea->clCarScene->UpdateOpponentsCar();  
		clArea->clCarScene->UpdateScene(); 
	}
	////  Section InputControl  ////
	clInput->GetUserKBInput();	
	clInput->GetUserMouseState();
	clCEGUI->GetGUISystem()->injectTimePulse(TimeElapsed);
}

