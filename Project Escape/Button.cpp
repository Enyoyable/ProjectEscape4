#include "Button.h"
#include "AnimatedSprite.h"

namespace esc
{
	Button::Button(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, int p_iObjectId, sf::Sprite *p_xSprite) : GameObject(p_v2fPosition, p_v2fSize, p_bInteractable, p_iObjectId, EObjectType::BUTTON, p_xSprite)
	{
		m_xHoverSprite = nullptr;
		m_xOnClickSprite = nullptr;

		m_bIsHovering = false;
		m_bIsclicked = false;
	}

	void Button::update(float p_fDeltaTime)
	{
		m_bIsHovering = false;
		m_bIsclicked = false;

		if (m_xHoverSprite != nullptr)
			static_cast<AnimatedSprite*>(m_xHoverSprite)->update(p_fDeltaTime);
	}

	void Button::setFunction(std::function<void ()> p_xFunction)
	{
		m_xFunction = p_xFunction;
	}

	void Button::HandleCollision()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			m_xFunction();
		else
			m_bIsHovering = true;
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		if (m_bIsclicked)
		{
			target.draw(*m_xOnClickSprite, states);
		}
		else if(m_bIsHovering)
		{
			target.draw(*m_xHoverSprite, states);
		}
		else
		{
			target.draw(*m_xSprite, states);
		}
	}

	void Button::setHoverSprite(sf::Sprite *p_xSprite)
	{
		m_xHoverSprite = p_xSprite;
	}

	void Button::setClickSprite(sf::Sprite *p_xSprite)
	{
		m_xOnClickSprite = p_xSprite;
	}
}