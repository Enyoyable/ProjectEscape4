#include "Gun.h"
#include "Bullet.h"
#include "SpriteManager.h"

namespace esc
{
	Gun::Gun(bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects, GameObjectManager *p_xGameObjectManager, SpriteManager *p_xSpriteManager) : Weapon(EWeaponType::WEAPONGUN, p_bHasCharges, p_iAmountOfCharges, p_fDelay, p_fDuration, p_vObjects)
	{
		m_xGameObjectManager = p_xGameObjectManager;
		m_xSpriteManager = p_xSpriteManager;
		m_bHasShot = false;
	}

	void Gun::update(float fDeltaTime)
	{
		if (m_bHasFired)
		{
			float fLastTimer = m_fAttackTimer;

			m_fAttackTimer += fDeltaTime;

			if (m_bHasShot)
			{
				if (m_fAttackDelay + m_fAttackDuration < m_fAttackTimer)
				{
					m_bHasFired = false;
					m_bHasShot = false;
				}
			}
			else
			{
				attack();
				m_bHasShot = true;
			}
		}
	}

	void Gun::attack()
	{
		Bullet *bullet = new Bullet(m_xAttachedObject->getPosition(), sf::Vector2f(44, 5), m_v2fTarget, m_xAttachedObject, m_xSpriteManager->loadSprite("bullet.png", 0, 0, 44, 5));
		m_vObjects->push_back(bullet);
	}

	void Gun::trigger()
	{
		if (m_bHasCharges && m_bHasFired == false)
		{
			if (m_iCurrentCharges > 0)
			{
				m_iCurrentCharges--;
				m_bHasFired = true;
				m_fAttackTimer = 0;
			}
		}
		else
		{
			m_bHasFired = true;
			m_fAttackTimer = 0;
		}
	}

	void Gun::Throw()
	{
		Item *Gun = new Item(m_xAttachedObject->getPosition(), m_xSpriteManager->loadSprite("PDA_pu.png", 0, 0, 64, 64), true, m_v2fTarget, 9999, BATON);
		m_vObjects->push_back(Gun);
	}

	void Gun::setTarget(sf::Vector2f p_v2fTarget)
	{
		m_v2fTarget = p_v2fTarget;
	}
}