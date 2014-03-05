#include "Item.h"

#include "DrawManager.h"
#include <math.h>
#include "Level.h"

namespace esc
{
	Item::Item(sf::Vector2f p_v2Pos, sf::Sprite *p_sSprite, int p_iObjectId, EObjectType p_eType)
		:GameObject(p_v2Pos, sf::Vector2f(p_sSprite->getGlobalBounds().width, p_sSprite->getGlobalBounds().height), true, p_iObjectId, p_eType, p_sSprite)
	{

		m_bIsRemoved = false;
		m_bIsFlying = false;
		m_v2Speed = sf::Vector2f(0.0f, 0.0f);
	}

	void Item::update(float p_fDeltaTime)
	{
		if (m_bIsRemoved)
			return;

		if (m_bIsFlying)
		{
			move(m_v2Speed);
			setRotation(getRotation() + 5.0f);
		}
	}

	void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_bIsRemoved)
			return;

		states.transform *= getTransform();
		target.draw(*m_xSprite, states);
	}

	void Item::setIsRemoved(bool p_bIsGone)
	{
		m_bIsRemoved = p_bIsGone;
	}
	bool Item::getIsRemoved()
	{
		return  m_bIsRemoved;
	}

	void Item::setSpeed(sf::Vector2f p_v2fInpSpeed)
	{
		m_v2Speed = p_v2fInpSpeed;
	}
	sf::Vector2f Item::getSpeed()
	{
		return m_v2Speed;
	}

	void Item::setIsFlying(bool p_bflying)
	{
		m_bIsFlying = p_bflying;
	}
	bool Item::getIsflying()
	{
		return m_bIsFlying;
	}

}