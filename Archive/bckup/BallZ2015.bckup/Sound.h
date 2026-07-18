#pragma once
#ifndef _SOUND_h__
#define _SOUND_h__

#include "FMod\fmod.hpp"
#include "FMod\fmod.h"
#include "FMod\fmod_errors.h"

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class CLSound
{
public:
	CLSound();
	~CLSound();
	void Update();
	int LoadSound(const char * filename, bool bLoop);
	void PlaySound(int iID);

	int iIntro, iPiano, iCoin, iJump;

	// Fonctions de création et destruction du singleton
	static CLSound *getInstance()
	{
		if (NULL == _singleton)
		{
			// std::cout << "creating singleton." << std::endl;
			_singleton = new CLSound();
		}
		else
		{
			// std::cout << "singleton already created!" << std::endl;
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
	static CLSound *_singleton;

	FMOD::System     *system;
	vector<FMOD::Sound*> vecSounds;
	FMOD_RESULT       result;
	FMOD::Channel    *channel;
};
#endif