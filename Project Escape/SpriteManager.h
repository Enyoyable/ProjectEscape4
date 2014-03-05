#pragma once

#include "stdafx.h"

namespace esc
{
	class AnimatedSprite;

	class SpriteManager
	{
	public:
		SpriteManager(std::string p_sdirectory);

		sf::Sprite *loadSprite(std::string p_sFileName, int p_iX, int p_iY, int p_iW, int p_iH);

		sf::Sprite *loadSprite(std::string p_sFileName);

		AnimatedSprite *loadAnimatedSprite(std::string p_sFileName);

	private:
		std::string m_sDirectoryPath;

		std::map<std::string, sf::Texture*> m_mTextures;
	};

}