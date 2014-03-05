#pragma once

#include <string.h>
#include <SFML/Audio.hpp>

/*class IAudioProvider
{
public:
  virtual ~IAudioProvider() {}
  virtual void PlaySound(std::string filename) = 0;
  virtual void PlaySong(std::string filename, bool looping) = 0;
  virtual void StopAllSounds() = 0;
  
  virtual bool IsSoundPlaying() = 0;
  virtual bool IsSongPlaying() = 0;
};
*/
class SFMLSoundProvider //: public IAudioProvider
{
public:

	SFMLSoundProvider();

	void PlaySound(std::string filename);
	void PlaySong(std::string filename, bool looping = false);
	void StopAllSounds();
	
	bool IsSoundPlaying();
	bool IsSongPlaying();

private:
	sf::SoundBuffer _soundBuffer;
	sf::Sound _sound;
	sf::Music _music;
};