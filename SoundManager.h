#pragma once

#include <SFML/Audio.hpp>

#include <string>
#include <filesystem>

class SoundManager 
{
	public:
		SoundManager() = delete;

		SoundManager(const char* soundLocation);

		void Play();
		void Switch(sf::SoundBuffer& sound);
		void Stop();
		void Loop();

	private:
		sf::SoundBuffer SoundBuffer;
		sf::Sound Sound;
		
};
