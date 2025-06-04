#pragma once

#include <SFML/Audio.hpp>

class SoundManager 
{
	public:
		SoundManager() = delete;
		SoundManager(sf::SoundBuffer Sound);

	private:
		sf::Sound SoundPlayer;
};
