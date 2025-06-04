#pragma once

#include <SFML/Audio.hpp>
#include <string>

class SoundManager 
{
	public:
		SoundManager() = delete;

		SoundManager(const std::string soundLocation);

		//not sure if these are neccessary
		//since I don't think main has access to the audio lib
		// i'll keep em for now
		SoundManager(const sf::SoundBuffer& sound);
		SoundManager(sf::SoundBuffer&& sound);

		void Play();
		void Switch(sf::SoundBuffer& sound);
		void Switch(sf::SoundBuffer&& sound);
		void Stop();
		void Loop();

	private:
		sf::SoundBuffer SoundBuffer;
		sf::Sound Sound;
		
};
