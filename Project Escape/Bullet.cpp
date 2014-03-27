#include "Bullet.h"
#include "GameObject.h"
#include "Guard.h"
#include "PlayerObject.h"

namespace esc
{
	Bullet::Bullet(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, sf::Vector2f p_v2fDirection, GameObject *p_xShootObject, sf::Sprite *p_xSprite) : GameObject(p_v2fPosition, p_v2fSize, false, 99999, BULLET, p_xSprite)
	{
		setOrigin(22, 2.5);

		m_v2fMovementVector = p_v2fDirection - p_v2fPosition;

		float xDiff = -p_v2fPosition.x + p_v2fDirection.x;
		float yDiff = -p_v2fPosition.y + p_v2fDirection.y;
		
		float rotation = atan2f(yDiff, xDiff) / 0.017453292519943 + 180;
		setRotation(rotation);

		m_v2fMovementVector /= sqrtf(xDiff * xDiff + yDiff * yDiff);

		setPosition(getPosition() + (m_v2fMovementVector * 60.f));

		m_v2fMovementVector *= 2000.f;

		m_xShootObject = p_xShootObject;
	}

	void Bullet::update(float fDeltaTime)
	{
		setPosition(getPosition() + m_v2fMovementVector * fDeltaTime);
	}

	bool Bullet::HandleCollision(GameObject *p_xGameObject, std::vector<GameObject*> *p_vRemoveVector)
	{
		if (p_xGameObject->getType() == PATROLLINGGUARD || p_xGameObject->getType() == STATIONARYGUARD || p_xGameObject->getType() == PLAYER)
		{
			if (p_xGameObject == m_xShootObject)
				return false;

			p_vRemoveVector->push_back(p_xGameObject);
			p_vRemoveVector->push_back(this);

			m_v2fMovementVector = sf::Vector2f(0, 0);

			PlayerObject *player = static_cast<PlayerObject*>(m_xShootObject);

			player->m_bHasCard = true;

			return true;
		}
		else if (p_xGameObject->getType() == WALL || p_xGameObject->getType() == DOOR)
		{
			p_vRemoveVector->push_back(this);

			m_v2fMovementVector = sf::Vector2f(0, 0);

			return true;
		}

		return false;
	}
}