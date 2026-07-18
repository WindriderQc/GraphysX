#include "3DScenes.h"

// CARSCENE /////////////

void CLCarScene::UpdateOpponentsCar(void)
{
clCarsManager->GetPlayersInput();

int i;
	
for(i=0; i<NUMOPPONENTCARS; i++)
	{
		clOpponentCars[i]->SetEngineValues(stOpponentsEng[i]);// Des fois ca peut planter car rien d'initialiser pour les PlayersInput si ya rien..
	}

}

///////////////////////