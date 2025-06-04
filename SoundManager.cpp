#include "SoundManager.h"

#include <utility>

SoundManager::SoundManager(std::filesystem::path&& soundLocation)
	:SoundBuffer(std::move(soundLocation)), Sound(SoundBuffer)
{}

SoundManager::SoundManager(const sf::SoundBuffer& sound)
	:SoundBuffer(sound), Sound(SoundBuffer)
{}

SoundManager::SoundManager(sf::SoundBuffer&& sound)
	:SoundBuffer(std::move(sound)), Sound(SoundBuffer)
{}

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