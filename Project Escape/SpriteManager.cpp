#include "SpriteManager.h"
#include <fstream>
#include "AnimatedSprite.h"

namespace esc
{

	SpriteManager::SpriteManager(std::string directory)
	{
		m_sDirectoryPath = directory;
	}

	sf::Sprite *SpriteManager::loadSprite(std::string fileName, int x, int y, int w, int h)
	{
		auto it = m_mTextures.find(fileName);

		if (it == m_mTextures.end())
		{
			sf::Texture *texture = new sf::Texture;

			texture->loadFromFile(m_sDirectoryPath + fileName, sf::IntRect(x, y, w, h));

			m_mTextures.insert(std::pair<std::string, sf::Texture*>(fileName, texture));

			it = m_mTextures.find(fileName);
		}

		return new sf::Sprite(*it->second);
	}

	sf::Sprite *SpriteManager::loadSprite(std::string p_sFilePath)
	{
		std::fstream stream(m_sDirectoryPath + p_sFilePath);

		std::string sTexturePath;

		stream >> sTexturePath;

		auto it = m_mTextures.find(sTexturePath);

		if (it == m_mTextures.end())
		{
			sf::Texture *texture = new sf::Texture;

			texture->loadFromFile(m_sDirectoryPath + sTexturePath);

			m_mTextures.insert(std::pair<std::string, sf::Texture*>(sTexturePath, texture));

			it = m_mTextures.find(sTexturePath);
		}

		int x, y, w, h;

		stream >> x >> y >> w >> h;

		return new sf::Sprite(*it->second, sf::IntRect(x, y, w, h));
	}

	AnimatedSprite *SpriteManager::loadAnimatedSprite(std::string p_sFileName)
	{
		std::fstream stream(m_sDirectoryPath + p_sFileName);

		std::string sTexturePath;

		stream >> sTexturePath;

		auto it = m_mTextures.find(sTexturePath);

		if (it == m_mTextures.end())
		{
			sf::Texture *texture = new sf::Texture;

			texture->loadFromFile(m_sDirectoryPath + sTexturePath);

			m_mTextures.insert(std::pair<std::string, sf::Texture*>(sTexturePath, texture));

			it = m_mTextures.find(sTexturePath);
		}

		AnimatedSprite *xSprite = new AnimatedSprite;

		std::string sRow;

		while (!stream.eof())
		{
			int x, y, w, h;

			AnimatedSprite::Frame *frame = new AnimatedSprite::Frame;

			stream >> frame->m_fDuration;

			stream >> x >> y >> w >> h;

			frame->m_placement = sf::IntRect(x, y, w, h);

			frame->m_texture = it->second;

			xSprite->addFrame(frame);
		}

		return xSprite;
	}

}