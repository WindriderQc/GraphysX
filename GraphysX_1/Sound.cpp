#include "StdAfx.h"
#include "Sound.h"


//void(*Common_Private_Error)(FMOD_RESULT, const char *, int);
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		MSGBOX(file << "(" << line << "): FMOD error " << result << FMOD_ErrorString(result), "Sound ERRCHECK");
	}
}

FMOD::System*  GraphysX::CLSound::system = nullptr;
FMOD::Channel* GraphysX::CLSound::channel = nullptr;
FMOD_RESULT    GraphysX::CLSound::result;
FMOD::ChannelGroup* GraphysX::CLSound::channelgroup = nullptr;


namespace GraphysX
{
	CLSound::CLSound()
	{	
		if (system == nullptr)//Create a System object and initialize
		{
			unsigned int      version;
			void             *extradriverdata = 0;
			result = FMOD::System_Create(&system);	ERRCHECK(result);
			result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);  ERRCHECK(result);
			result = system->set3DSettings(1.0f,1.0f, 1.0f);  ERRCHECK(result);
			//FMOD::ChannelGroup* channelgroup;
			//result = system->getMasterChannelGroup(&channelgroup); ERRCHECK(result);

			result = system->getVersion(&version);	ERRCHECK(result);
			if (version < FMOD_VERSION)
			{
				MSGBOX("FMOD lib version " << version << "doesn't match header version " << FMOD_VERSION, "Sound Initialization");
			}
			
			std::cout << "FMOD version " << version << " initialized." << std::endl;
		}
	}
	CLSound::~CLSound()
	{
	 //  AVANT - on clearait tous les son loader ici....  mais maintenant dans resource manager... :S   still safe?
		result = system->close();
		ERRCHECK(result);
		result = system->release();
		ERRCHECK(result);
	}
	void CLSound::Update()
	{
		result = system->update();
		ERRCHECK(result);
	}
	FMOD::Sound* CLSound::LoadSound(const char * filename, bool bLoop)
	{
		FMOD::Sound      *sound;

		result = system->createStream(filename, FMOD_3D_LINEARROLLOFF, 0, &sound);
		ERRCHECK(result);

		sound->set3DMinMaxDistance(0.1f, 50.0f);

		//vecSounds.push_back(sound);
		soundSetLoop(sound, bLoop);
		//return (vecSounds.size() - 1);
		return sound;
	}
	void CLSound::release(FMOD::Sound* sound)
	{
		result = sound->release();
		ERRCHECK(result);
	}
	void CLSound::soundSetLoop(FMOD::Sound *sound, bool bLoop)
	{
		if (bLoop)
		{
			result = sound->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		else{
			result = sound->setMode(FMOD_LOOP_OFF);
			ERRCHECK(result);
		}
	}
	void CLSound::PlaySound(FMOD::Sound* sound)
	{
		result = system->playSound(sound, 0, false, &channel);
		ERRCHECK(result);
	}

	void CLSound::updateListener(cTV_3DVECTOR &position, cTV_3DVECTOR &velocity, cTV_3DVECTOR & forward, cTV_3DVECTOR & up)
	{
		FMOD_VECTOR listenerVelocity;
		listenerVelocity.x = velocity.x;
		listenerVelocity.y = velocity.y;
		listenerVelocity.z = velocity.z;

		FMOD_VECTOR listenerPos;
		listenerPos.x = position.x;
		listenerPos.y = position.y;
		listenerPos.z = position.z;

		FMOD_VECTOR listenerForward;
		listenerForward.x = forward.x;
		listenerForward.y = forward.y;
		listenerForward.z = forward.z;

		FMOD_VECTOR listenerUp;
		listenerUp.x = up.x;
		listenerUp.y = up.y;
		listenerUp.z = up.z;

		result = system->set3DListenerAttributes(0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp);
		ERRCHECK(result);
	}

	/*
	void Sound3D::update3DSound( ofVec3f position, ofVec3f velocity )
{
	soundPosition.x = position.x;
    soundPosition.y = position.y;
    soundPosition.z = position.z;
	
    soundVelocity.x = velocity.x;
    soundVelocity.y = velocity.y;
    soundVelocity.z = velocity.z;
	
	FMOD_Channel_Set3DAttributes(channel, &soundPosition, &soundVelocity);
}
*/
	// Available functions to implement if needed
	/*
	{
	unsigned int ms = 0;
	unsigned int lenms = 0;
	bool         playing = 0;
	bool         paused = 0;
	int          channelsplaying = 0;

	if (channel)
	{
	FMOD::Sound *currentsound = 0;

	result = channel->isPlaying(&playing);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
	{
	ERRCHECK(result);
	}

	result = channel->getPaused(&paused);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
	{
	ERRCHECK(result);
	}

	result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
	{
	ERRCHECK(result);
	}

	channel->getCurrentSound(&currentsound);
	if (currentsound)
	{
	result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
	{
	ERRCHECK(result);
	}
	}
	}

	system->getChannelsPlaying(&channelsplaying);
	*/

}