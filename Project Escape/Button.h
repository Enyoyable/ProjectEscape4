#pragma once

#include "stdafx.h"
#include "GameObject.h"
#include <functional>

namespace esc
{

	class Button : public GameObject
	{
	public:
		Button(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, int p_iObjectId, sf::Sprite *p_xSprite = nullptr);

		virtual void update(float p_fDeltaTime);

		void HandleCollision();

		void setFunction(std::function<void ()> p_xFunction);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setHoverSprite(sf::Sprite *p_xSprite);

		void setClickSprite(sf::Sprite *p_xSprite);

	private:
		sf::Sprite *m_xHoverSprite;
		sf::Sprite *m_xOnClickSprite;

		bool m_bIsHovering;
		bool m_bIsclicked;

		std::function<void()> m_xFunction;
	};

}