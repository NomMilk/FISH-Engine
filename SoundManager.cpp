#include "SoundManager.h"

#include <utility>

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