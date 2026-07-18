#include "StdAfx.h"
#include "Sound.h"

// Initialisation du singleton à NULL
CLSound *CLSound::_singleton = NULL;

void(*Common_Private_Error)(FMOD_RESULT, const char *, int);
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		MSGBOX(file << "(" << line << "): FMOD error " << result << FMOD_ErrorString(result),"Sound ERRCHECK");
	}
}

CLSound::CLSound()
{
	unsigned int      version;
	void             *extradriverdata = 0;
	/*
	Create a System object and initialize
	*/
	result = FMOD::System_Create(&system);
	ERRCHECK(result);
	result = system->getVersion(&version);
	ERRCHECK(result);
	cout << "FMOD version " << version << "loading..." << endl;
	if (version < FMOD_VERSION)
	{
	  MSGBOX("FMOD lib version " << version << "doesn't match header version " << FMOD_VERSION,  "Sound Initialization");
	}
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);


	/*
	Load Sounds
	*/
	iIntro = LoadSound("Media\\Sounds\\Intro.mp3", false);
	iPiano = LoadSound("Media\\Sounds\\Piano.mp3", true);
	iCoin = LoadSound("Media\\Sounds\\Coin.wav", false);
	iJump = LoadSound("Media\\Sounds\\Jump.wav", false);

}
CLSound::~CLSound()
{
	for each(FMOD::Sound*  snd in vecSounds)
	{
		result = snd->release();
		ERRCHECK(result);
	}
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
int CLSound::LoadSound(const char * filename, bool bLoop)
{
	FMOD::Sound      *sound;

	result = system->createStream(filename, FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result);
	vecSounds.push_back(sound);
	if (bLoop)
	{
	result = sound->setMode(FMOD_LOOP_NORMAL);    
	ERRCHECK(result);
	}
	else{
		result = sound->setMode(FMOD_LOOP_OFF);
		ERRCHECK(result);
	}
	return (vecSounds.size()-1);
}
void CLSound::PlaySound(int iID)
{
		result = system->playSound(vecSounds[iID], 0, false, &channel);
		ERRCHECK(result);
}
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