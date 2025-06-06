#include "SoundManager.h"

#include <utility>

SoundManager::SoundManager(const char* soundLocation)
	:Sound(SoundBuffer)
{
	SoundBuffer.loadFromFile(soundLocation);
	Sound.setBuffer(SoundBuffer);
}

void SoundManager::Play()
{
	Sound.play();
}

void SoundManager::Switch(sf::SoundBuffer& sound)
{
	Sound.setBuffer(sound);
}

void SoundManager::Stop()
{
	Sound.stop();
}

void SoundManager::Loop()
{
	Sound.setLooping(true);
}