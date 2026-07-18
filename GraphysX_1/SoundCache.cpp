#include "stdafx.h"
#include "SoundCache.h"
#include "Sound.h"

namespace GraphysX
{

	SoundCache::SoundCache()
	{
	}

	FMOD::Sound* SoundCache::getSound(std::string soundFilename, bool loop)
	{
		FMOD::Sound* sound;
		auto mit = m_soundMap.find(soundFilename);
		if (mit == m_soundMap.end())
		{
			std::string filePath = soundFolder + soundFilename;
			sound = GraphysX::CLSound::LoadSound(filePath.c_str(), loop);
			m_soundMap.insert(make_pair(soundFilename, sound));
			return sound;
		}
		GraphysX::CLSound::soundSetLoop(mit->second, loop);
		return mit->second;
	}

	int SoundCache::getNbrSound()
	{
		return m_soundMap.max_size();
	}

	void SoundCache::releaseSounds()
	{
		for (auto it : m_soundMap)
		{
			GraphysX::CLSound::release(it.second);
		}
	}


}