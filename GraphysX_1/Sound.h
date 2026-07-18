#pragma once
#include <TV3D65\tv_types.h>  // cTV_3DVECTOR
#include "FMOD\fmod.hpp"
#include "FMOD\fmod.h"
#include "FMOD\fmod_errors.h"

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

namespace GraphysX
{
	class GRAPHYSX_API CLSound
	{
	public:
		 CLSound();
		 ~CLSound();
		 static void Update();
		 static FMOD::Sound* LoadSound(const char * filename, bool bLoop);
		 static void PlaySound(FMOD::Sound* sound);
		 static void soundSetLoop(FMOD::Sound *sound, bool bLoop);
		 static void release(FMOD::Sound* sound);

		 static void updateListener(cTV_3DVECTOR &position, cTV_3DVECTOR &velocity, cTV_3DVECTOR & forward, cTV_3DVECTOR & up);

		//GRAPHYSX_API static FMOD::Sound* Load3DSound(const char * filename, bool bLoop);

	
	private:
		static FMOD::System     *system;
		//std::vector<FMOD::Sound*> vecSounds;
		static FMOD_RESULT       result;
		static FMOD::Channel    *channel;
		static FMOD::ChannelGroup* channelgroup;
	};
}