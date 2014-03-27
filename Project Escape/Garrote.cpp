#include "Garrote.h"
#include "PlayerObject.h"
#include "Guard.h"
#include "SoundRipple.h"
#include "Animator.h"

namespace esc
{

	Garrote::Garrote(float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects) : Weapon(WEAPONGARROTE, false, 0, p_fDelay, p_fDuration, p_vObjects)
	{
		m_bHasStarted = false;
		m_bHasAttacked = false;
		m_fSoundTimer = 0;

		m_bAnimationSet = false;
	}

	void Garrote::update(float fDeltaTime)
	{
		if (m_bHasFired)
		{
			float fLastTimer = m_fAttackTimer;

			m_fAttackTimer += fDeltaTime;

			if (m_fAttackTimer < m_fAttackDuration || fLastTimer < m_fAttackDuration)
			{
				if (m_fAttackTimer > m_fAttackDuration && fLastTimer < m_fAttackDuration)
				{
					attack(true);
				}
				else
				{
					m_fSoundTimer += fDeltaTime;

					if (m_fSoundTimer > 0.4f)
					{
						sf::Vector2f ripplePosition = m_xAttachedObject->getPosition();

						//printf("Rads: %f", ((getRotation() - 180) * 0.0174532925));

						ripplePosition.x -= cosf((m_xAttachedObject->getRotation() - 180) * 0.0174532925) * 60;
						ripplePosition.y -= sinf((m_xAttachedObject->getRotation() - 180) * 0.0174532925) * 60;
						SoundRipple *ripple = new SoundRipple(ripplePosition, 8, 150, 0.5f);
						m_vObjects->insert(m_vObjects->begin(), ripple);

						m_fSoundTimer = 0;
					}

					attack(false);
				}
			}
			else if (m_fAttackTimer > m_fAttackDelay + m_fAttackDuration)
			{
				m_bHasFired = false;
			}

			if (!m_bHasAttacked)
				m_bHasFired = false;
		}
	}

	void Garrote::attack(bool p_bIsFinished)
	{
		

		PlayerObject *xPlayer = static_cast<PlayerObject*>(m_xAttachedObject);

		float fMiddleAngle = xPlayer->getRotation();

		for (auto object : *m_vObjects)
		{
			if (object->getType() == PATROLLINGGUARD || object->getType() == STATIONARYGUARD)
			{
				Guard *xGuard = static_cast<Guard*>(object);

				if (xGuard->m_isDead)
					continue;

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

				if (m_bHasStarted)
				{
					xPlayer->setRotation(xGuard->m_fWatchAngle);
				}

				if (fLowestAngle > fHighestAngle)
				{
					if (fAngleToGuard > fLowestAngle || fAngleToGuard < fHighestAngle)
					{
						float fTotDiff = sqrtf(xDiff * xDiff + yDiff * yDiff);

						if (fTotDiff < 100.f)
						{
							float fGuardRotation = xGuard->m_fWatchAngle;

							

							if (fabs(fGuardRotation - fMiddleAngle) < fAttackWidth)
							{
								

								m_bHasStarted = true;
								fGuardRotation += 180;
								m_bHasAttacked = true;
								if (fGuardRotation > 360)
								{
									fGuardRotation -= 360;
								}

								sf::Vector2f guardDirection(cosf(fGuardRotation * 0.0174532925), sinf(fGuardRotation * 0.0174532925));
								guardDirection *= 40.f;

								xPlayer->setPosition(xGuard->getPosition() + guardDirection);
								xPlayer->m_bAngleLocked = true;
								xGuard->m_bAngleLocked = true;

								if (!m_bAnimationSet)
								{
									PlayerObject *xPlayer = static_cast<PlayerObject*>(m_xAttachedObject);

									xPlayer->m_xAnimator->setForcedAnimation("spy_strangle.txt");
									m_bAnimationSet = true;
								}

								if (p_bIsFinished)
								{
									xGuard->m_isDead = true;
									xGuard->setIsRemoved(true);
									m_bHasStarted = false;
									m_bHasAttacked = false;
									xPlayer->m_bAngleLocked = false;
									xGuard->m_bAngleLocked = false;
									m_fAttackTimer = 0;
									m_bAnimationSet = false;
								}
								return;
							}

							
						}
					}
					else
					{
						m_bHasFired = false;
					}
				}
				else
				{
					if (fAngleToGuard > fLowestAngle && fAngleToGuard < fHighestAngle)
					{
						float fTotDiff = sqrtf(xDiff * xDiff + yDiff * yDiff);

						if (fTotDiff < 100.f)
						{
							float fGuardRotation = xGuard->m_fWatchAngle;

							

							if (fabs(fGuardRotation - fMiddleAngle) < fAttackWidth)
							{
								m_bHasStarted = true;
								fGuardRotation += 180;
								m_bHasAttacked = true;
								if (fGuardRotation > 360)
								{
									fGuardRotation -= 360;
								}

								sf::Vector2f guardDirection(cosf(fGuardRotation * 0.0174532925), sinf(fGuardRotation * 0.0174532925));
								guardDirection *= 40.f;

								xPlayer->setPosition(xGuard->getPosition() + guardDirection);
								xPlayer->m_bAngleLocked = true;
								xGuard->m_bAngleLocked = true;

								if (!m_bAnimationSet)
								{
									PlayerObject *xPlayer = static_cast<PlayerObject*>(m_xAttachedObject);

									xPlayer->m_xAnimator->setForcedAnimation("spy_strangle.txt");
									m_bAnimationSet = true;
								}
								
								if (p_bIsFinished)
								{
									m_bHasAttacked = false;
									xGuard->m_isDead = true;
									xGuard->setIsRemoved(true);
									m_bHasStarted = false;
									xPlayer->m_bAngleLocked = false;
									xGuard->m_bAngleLocked = false;
									m_fAttackTimer = 0;
									m_bAnimationSet = false;
								}
								
								return;
							}

							
						}
					}
					else
					{
						m_bHasFired = false;
					}
				}
			}


		}

		
	}

	void Garrote::trigger()
	{
		if (m_bHasFired == false)
		{
			m_bHasFired = true;
			m_fAttackTimer = 0;
		}
	}

}