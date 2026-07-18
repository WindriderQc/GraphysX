#ifndef __REALISATEUR_H__
#define __REALISATEUR_H__
#pragma once
#include "3DScenes.h"

class CLRealisateur
{
public:
	CLRealisateur();
	~CLRealisateur();
	CLBallZScene* GetActiveScene();
	void OnTourne(void);
	
	static CLRealisateur *getInstance()
	{
		if (NULL == _singleton)
		{

			_singleton = new CLRealisateur;
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
private:
	// Singleton
	static CLRealisateur *_singleton;
	CLBallZScene* pActiveScene;
	// Declarations des différentes Scene de l'Area
	CLBallZScene* clBallZScene; 
};
#endif 
