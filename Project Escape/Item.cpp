#include "Item.h"

#include "DrawManager.h"
#include <math.h>
#include "Level.h"

namespace esc
{
	Item::Item(sf::Vector2f p_v2Pos, sf::Sprite *p_sSprite, bool p_bIsFlying, int p_iCharges, sf::Vector2f p_v2target, int p_iObjectId, EObjectType p_eType)
		:GameObject(p_v2Pos, sf::Vector2f(p_sSprite->getGlobalBounds().width - 20, p_sSprite->getGlobalBounds().height - 20), true, p_iObjectId, p_eType, p_sSprite)
	{
		setOrigin(32, 32);

		m_v2fTarget = p_v2target - p_v2Pos;

		float xDiff = -p_v2Pos.x + p_v2target.x;
		float yDiff = -p_v2Pos.y + p_v2target.y;

		float rotation = atan2f(yDiff, xDiff) / 0.017453292519943 + 180;
		setRotation(rotation);

		m_v2fTarget /= sqrtf(xDiff * xDiff + yDiff * yDiff);

		m_v2fTarget *= 1000.f;

		m_bIsRemoved = false;

		m_bIsFlying = p_bIsFlying;
		//m_v2fTarget = p_v2target;

		m_icharges = p_iCharges;
	}

	void Item::update(float p_fDeltaTime)
	{
		if (m_bIsRemoved)
			return;

		if (m_bIsFlying)
		{
			setPosition(getPosition() + m_v2fTarget * p_fDeltaTime);
			setRotation(getRotation() + 50.0f);
		}
	}


	void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_bIsRemoved)
			return;

		states.transform *= getTransform();
		target.draw(*m_xSprite, states);
	}

	void Item::HandleCollision(GameObject *p_xGameObject, std::vector<GameObject*> *p_vRemoveVector)
	{
		if (p_xGameObject->getType() == PATROLLINGGUARD || p_xGameObject->getType() == STATIONARYGUARD)
		{
			if (m_bIsFlying == true && m_bIsRemoved == false)
			{
				p_vRemoveVector->push_back(p_xGameObject);
				//p_vRemoveVector->push_back(this);

				m_bIsFlying = false;
				m_v2fTarget = sf::Vector2f(0, 0);
			}

		}
		else if (p_xGameObject->getType() == WALL)
		{
			//p_vRemoveVector->push_back(this);

			m_bIsFlying = false;
			m_v2fTarget = sf::Vector2f(0, 0);
		}
	}

	void Item::setDir(sf::Vector2f p_v2fInpSpeed)
	{
		m_v2fTarget = p_v2fInpSpeed;
	}
	sf::Vector2f Item::getDir()
	{
		return m_v2fTarget;
	}

	void Item::setIsFlying(bool p_bflying)
	{
		m_bIsFlying = p_bflying;
	}
	bool Item::getIsflying()
	{
		return m_bIsFlying;
	}

	int Item::getCharges()
	{
		return m_icharges;
	}

}