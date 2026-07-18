#include "CarsManager.h"


// Initialisation du singleton à NULL
CLCarsManager *CLCarsManager::_singleton = NULL;

CLCarsManager::CLCarsManager()
{
iNbrCars = 0;

for(int i =0; i<4;i++)
	{
	clCars[i] = NULL;
	}

}

CLCarsManager::~CLCarsManager()
{

}


void CLCarsManager::AddCar(CLVehicule* clCar)
{
	clCars[iNbrCars] = clCar;
	iNbrCars++;
}

void CLCarsManager::Init(bool bCommMode)
{
bIsServer = bCommMode;
clTCPComm = new CLTCPComm(bIsServer);//
//clTCPComm->Run();				//  A setter dans un autre thread pour que le tout soit indépendant de la comm réseau.
if(bIsServer)
	{
	clTCPComm->SetListen(33333);  // Listen on Port 33333
	clTCPComm->Wait4SessionConnection();   //  Va geler si ya pas de connection..... :(
	clTCPComm->AcceptSessionConnection();
	}
else
	{
	clTCPComm->Connect("192.168.1.112", 33333);  // Connect to IP adress and Port
	}



}

void CLCarsManager::CloseConnection(void)
{
clTCPComm->Disconnect();
}

void CLCarsManager::GetPlayersInput(void)
{
clTCPComm->ReceiveData();


}

void CLCarsManager::SendPosMatrix(void)
{
int i;

for(i=0; i<4; i++)
{
clCars[i]->
}

clTCPComm->Send("Alouette");   
}