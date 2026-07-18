#ifndef __Actor_H__
#define __Actor_H__
#pragma once
#include "TV3DMoteur.h"

class CLActor : public CTVActor
{
public:
	CLActor();
	CLActor(char* Name, char* Path, cTV_3DVECTOR Pos);
	~CLActor();

	CTVActor* tvActor;
};

class CLActorFactory 
{
private:
	CLActorFactory(){
		clActor = new CLActor();
		clActorGate = new CLActor("Gate", "c:\\Test\\Gate1.x", cTV_3DVECTOR(0,0,0));
	
	
	};
	~CLActorFactory();
	static CLActorFactory *_singleton;
	
	CLActor* clActor;
public:
	CLActor* clActorGate;


public:
	// Fonctions de création et destruction du singleton
	static CLActorFactory *getInstance()
	{
		if (NULL == _singleton)
		{
			_singleton = new CLActorFactory;
		}
		return _singleton;
	}
	static void kill()
	{
		if (NULL != _singleton)
		{
			delete _singleton;
			_singleton = NULL;
		}
	}
	
};

#endif