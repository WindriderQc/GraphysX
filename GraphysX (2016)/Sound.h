#pragma once
#include "GraphysXdef.h"
#include <TV3D65\tv_types.h>
#include "FMOD\fmod.hpp"
#include "FMOD\fmod.h"
#include "FMOD\fmod_errors.h"

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

namespace GraphysX
{
	class CLSound
	{
	public:
		GRAPHYSX_API CLSound();
		GRAPHYSX_API ~CLSound();
		GRAPHYSX_API static void Update();
		GRAPHYSX_API static FMOD::Sound* LoadSound(const char * filename, bool bLoop);
		GRAPHYSX_API static void PlaySound(FMOD::Sound* sound);
		GRAPHYSX_API static void soundSetLoop(FMOD::Sound *sound, bool bLoop);
		GRAPHYSX_API static void release(FMOD::Sound* sound);

		GRAPHYSX_API static void updateListener(cTV_3DVECTOR &position, cTV_3DVECTOR &velocity, cTV_3DVECTOR & forward, cTV_3DVECTOR & up);

		//GRAPHYSX_API static FMOD::Sound* Load3DSound(const char * filename, bool bLoop);

	
	private:
		static FMOD::System     *system;
		//std::vector<FMOD::Sound*> vecSounds;
		static FMOD_RESULT       result;
		static FMOD::Channel    *channel;
		static FMOD::ChannelGroup* channelgroup;
	};
}