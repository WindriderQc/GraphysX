#pragma once
#include "Sound.h"
#include <map>

class SoundCache
{
public:
	SoundCache::SoundCache();
	FMOD::Sound* getSound(std::string soundFilename, bool loop);
	int getNbrSound();
	void releaseSounds();

private:
	std::map<std::string, FMOD::Sound*> m_soundMap;

	const std::string soundFolder = "..\\StockRoom\\Sounds\\";
};

