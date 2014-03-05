#pragma once

#include "stdafx.h"

namespace esc
{

	class SpriteManager;
	class PlayerObject;

	class HUD : public sf::Transformable, public sf::Drawable
	{
	public:
		HUD(sf::View *p_xView, SpriteManager *p_xSpriteManager, PlayerObject *p_xpObj);

		virtual void update(float p_fDeltaTime, sf::Time p_xTimer);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::View *m_xView;

		SpriteManager *m_xSpriteManager;
		PlayerObject *m_xPlayerObject;

		sf::Sprite *m_xLeftSmallCircle;
		sf::Sprite *m_xLeftSmallCircleItem;

		sf::Sprite *m_xRightLargeCircle;
		sf::Sprite *m_xRightLargeCircleItem;

		sf::Sprite *m_xLeftLargeCircle;
		sf::Sprite *m_xLeftLargeCircleItem;

		sf::Font *m_xFont;
		sf::Text *m_xText;
	};

}