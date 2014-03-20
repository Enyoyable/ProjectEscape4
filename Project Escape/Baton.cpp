#include "Baton.h"

#include "GameObject.h"
#include "Guard.h"
#include "PlayerObject.h"
#include "Level.h"
#include "SpriteManager.h"

namespace esc
{
	Baton::Baton(bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects, Level *p_xLevel, GameObjectManager *p_xGameObjectManager, SpriteManager *p_xSpriteManager) : Weapon(EWeaponType::WEAPONBATON, p_bHasCharges, p_iAmountOfCharges, p_fDelay, p_fDuration, p_vObjects)
	{
		m_xGameObjectManager = p_xGameObjectManager;
		m_xSpriteManager = p_xSpriteManager;
		m_xLevel = p_xLevel;
	}

	void Baton::update(float fDeltaTime)
	{
		if (m_bHasFired)
		{
			float fLastTimer = m_fAttackTimer;

			m_fAttackTimer += fDeltaTime;

			if (m_fAttackTimer > m_fAttackDuration)
			{
				if (m_fAttackDelay + m_fAttackDuration < m_fAttackTimer)
				{
					m_bHasFired = false;
				}
			}
			else if (m_fAttackTimer > m_fAttackDuration * 0.8f && fLastTimer < m_fAttackDuration * 0.8f)
			{
				attack();
			}
		}
	}

	void Baton::attack()
	{
		printf("attack");

		if (m_xAttachedObject != nullptr)
		{
			if (m_xAttachedObject->getType() == EObjectType::PATROLLINGGUARD || m_xAttachedObject->getType() == EObjectType::STATIONARYGUARD)
			{
				Guard *xGuard = static_cast<Guard*>(m_xAttachedObject);

				float fMiddleAngle = xGuard->getRotation();

				PlayerObject *xPlayer = nullptr;

				for (auto object : *m_vObjects)
				{
					if (object->getType() == PLAYER)
					{
						xPlayer = static_cast<PlayerObject*>(object);
						break;
					}
				}

				if (xPlayer == nullptr)
					return;

				float fAttackWidth = 40.f;

				float fLowestAngle = fMiddleAngle - fAttackWidth * 0.5;

				float fHighestAngle = fMiddleAngle + fAttackWidth * 0.5;

				if (fLowestAngle < 0)
					fLowestAngle += 360;

				if (fHighestAngle > 360)
					fHighestAngle -= 360;

				float fAngleToPlayer;

				float xDiff = xGuard->getPosition().x - xPlayer->getPosition().x;
				float yDiff = xGuard->getPosition().y - xPlayer->getPosition().y;

				fAngleToPlayer = atan2f(yDiff, xDiff) / 0.017453292519943 + 180;

				if (fLowestAngle > fHighestAngle)
				{
					if (fAngleToPlayer > fLowestAngle || fAngleToPlayer < fHighestAngle)
					{
						float fTotDiff = sqrtf(xDiff * xDiff + yDiff * yDiff);

						if (fTotDiff < 60.f)
						{
							m_xLevel->reset();
							xPlayer->reset();
							return;
						}
					}
				}
				else
				{
					if (fAngleToPlayer > fLowestAngle && fAngleToPlayer < fHighestAngle)
					{
						float fTotDiff = sqrtf(xDiff * xDiff + yDiff * yDiff);

						if (fTotDiff < 60.f)
						{
							m_xLevel->reset();
							xPlayer->reset();
							return;
						}
					}
				}
			}
			else if (m_xAttachedObject->getType() == PLAYER)
			{
				PlayerObject *xPlayer = static_cast<PlayerObject*>(m_xAttachedObject);

				float fMiddleAngle = xPlayer->getRotation();

				for (auto object : *m_vObjects)
				{
					if (object->getType() == PATROLLINGGUARD || object->getType() == STATIONARYGUARD)
					{
						Guard *xGuard = static_cast<Guard*>(object);

						float fAttackWidth = 90.f;

						float fLowestAngle = fMiddleAngle - fAttackWidth * 0.5;

						float fHighestAngle = fMiddleAngle + fAttackWidth * 0.5;

						if (fLowestAngle < 0)
							fLowestAngle += 360;

						if (fHighestAngle > 360)
							fHighestAngle -= 360;

						float fAngleToGuard;

						float xDiff = xPlayer->getPosition().x - xGuard->getPosition().x;
						float yDiff = xPlayer->getPosition().y - xGuard->getPosition().y;

						fAngleToGuard = atan2f(yDiff, xDiff) / 0.017453292519943 + 180;

						if (fLowestAngle > fHighestAngle)
						{
							if (fAngleToGuard > fLowestAngle || fAngleToGuard < fHighestAngle)
							{
								float fTotDiff = sqrtf(xDiff * xDiff + yDiff * yDiff);

								if (fTotDiff < 100.f)
								{
									xGuard->m_isDead = true;
									xGuard->setIsRemoved(true);
									return;
								}
							}
						}
						else
						{
							if (fAngleToGuard > fLowestAngle && fAngleToGuard < fHighestAngle)
							{
								float fTotDiff = sqrtf(xDiff * xDiff + yDiff * yDiff);

								if (fTotDiff < 100.f)
								{
									xGuard->m_isDead = true;
									xGuard->setIsRemoved(true);
									return;
								}
							}
						}
					}

					
				}
			}
		}
	}

	void Baton::trigger()
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
	}

	void Baton::Throw()
	{
		Item *Baton = new Item(m_xAttachedObject->getPosition(), m_xSpriteManager->loadSprite("Baton_pu.png", 0, 0, 64, 64), true, m_iCurrentCharges, m_v2fTarget, 9999, BATON);
		m_vObjects->push_back(Baton);
	}

	void Baton::drop()
	{
		Item *Baton = new Item(m_xAttachedObject->getPosition(), m_xSpriteManager->loadSprite("baton_pu.png", 0, 0, 64, 64), false, m_iCurrentCharges, sf::Vector2f(0.0f, 0.0f), 9999, BATON);
		m_vObjects->push_back(Baton);
	}

	void Baton::setTarget(sf::Vector2f p_v2fTarget)
	{
		m_v2fTarget = p_v2fTarget;
	}
}