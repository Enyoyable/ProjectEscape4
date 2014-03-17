#include "SoundManager.h"
#include "SFML\Audio\Sound.hpp"
#include "SFML\Audio\SoundBuffer.hpp"
#include "SFML\Audio\Music.hpp"
#include <iostream>

namespace esc
{


	SoundManager::SoundManager(std::string _dir)
	{
		m_directory = _dir;
	}

	void SoundManager::SoundBuffer(const std::string& _filename)
	{
		std::string path = m_directory + _filename;
		sf::SoundBuffer Buffer;
		Buffer.loadFromFile(path);


		std::map<std::string, sf::SoundBuffer>::iterator it = m_soundBuffer.find(_filename);
		if (it == m_soundBuffer.end())
		{
			m_soundBuffer.insert(std::pair<std::string, sf::SoundBuffer>(_filename, Buffer));
		}
	}

	sf::Sound* SoundManager::getSound(const std::string& _filename)
	{
		sf::Sound* sound = new sf::Sound();

		std::map<std::string, sf::SoundBuffer>::iterator it = m_soundBuffer.find(_filename);
		if (it == m_soundBuffer.end())
		{
			return nullptr;
		}

		sound->setBuffer(it->second);

		return sound;
	}

	sf::Music* SoundManager::getMusic(const std::string& _filename)
	{
		sf::Music* music = new sf::Music();

		if (!music->openFromFile(m_directory + _filename))
		{
			return nullptr;
		}

		return music;
	}

}