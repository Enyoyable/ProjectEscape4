#pragma once
#include <string>
#include <vector>
#include <map>
#include "SFML\Audio\Sound.hpp"
#include "SFML\Audio\SoundBuffer.hpp"
#include "SFML\Audio\Music.hpp"

namespace esc
{
	class Sound;
	class Music;
}


class SoundManager
{
public:
	SoundManager(std::string _dir);

	void SoundBuffer(const std::string& _filename);
	sf::Sound *getSound(const std::string& _filename);
	sf::Music *getMusic(const std::string& _filename);
	sf::SoundBuffer _soundBuffer;
	void Cleanup();
	sf::Sound _sound;

private:
	std::string m_directory;
	std::map<std::string, sf::SoundBuffer> m_soundBuffer;
	std::map<std::string, sf::Music> m_music;
	std::map<std::string, sf::Sound> m_Sound;

};
