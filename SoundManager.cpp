#include "SoundManager.h"

#include <utility>
#include <iostream>

SoundManager::SoundManager(const char* soundLocation)
	:Sound(SoundBuffer)
{
	if (!SoundBuffer.loadFromFile(soundLocation)) {
		std::cerr << "Failed to load from path: " << soundLocation << '\n';
	}
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