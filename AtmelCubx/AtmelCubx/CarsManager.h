#pragma once
#include "StdAfx.h"
#include "TCPComm.h"
#include "Vehicule.h"
 
/////////////////////////////////////////////////////////////////////////////
//
// CarsManager - une classe à instance unique.
//
/////////////////////////////////////////////////////////////////////////////

class CLCarsManager
{
private:
  // Constructeur/destructeur
  CLCarsManager();
  ~CLCarsManager();

public:
	void Init(bool bCommMode);
	void CloseConnection(void);
	void GetPlayersInput(void);
	void SendPosMatrix(void);
	void AddCar(CLVehicule* clCar);
  
	void setNbrCarsValue (int val) { iNbrCars = val; }
	int getNbrCarsValue () { return iNbrCars; }

	CLVehicule* clCars[4];

	CLTCPComm* clTCPComm;
	bool bIsServer;

  // Fonctions de création et destruction du singleton
  static CLCarsManager *getInstance ()
  {
    if (NULL == _singleton)
      {
        _singleton =  new CLCarsManager;
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
  // Variables membres
  int iNbrCars;
  static CLCarsManager *_singleton;

 

};