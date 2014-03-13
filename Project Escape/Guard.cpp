#include "Guard.h"
#include "PlayerObject.h"
#include "AIManager.h"
#include "PathFind.h"

namespace esc
{

	Guard::Guard(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, bool p_bInteractable, int p_iObjectId, PlayerObject *p_xPlayer, GameObjectManager *p_xGameObjectManager, Level *p_xlevel, PathFind *p_xPathfind, sf::Sprite *p_xSprite)
		: GameObject(p_v2fPosition, p_v2fSize, p_bInteractable, p_iObjectId, PATROLLINGGUARD, p_xSprite)
	{
		m_fWatchAngle = 0;
		m_fWatchArea = 0;

		setOrigin(32, 32);

		m_bIsChasing = false;

		m_xPlayerObject = p_xPlayer;

		m_eObjectType = PATROLLINGGUARD;

		m_fViewDistance = 0;
		
		m_bIsRotating = false;

		m_fStartAngle = 0;

		m_v2fStartPosition = p_v2fPosition;

		m_bIsPatrolling = false;

		m_iCurrentPatrolPointIndex = 0;

		m_iPatrolPointCount = 0;

		m_fGuardChaseSpeed = 120.f;
		m_fGuardPatrolSpeed = 60.f;

		m_isDead = false;

		m_bHasAI = false;

		m_xGobjManager = p_xGameObjectManager;

		m_xLevel = p_xlevel;
	}

	void Guard::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_isDead)
			return;

		sf::Vertex vertex1, vertex2;

		std::vector<sf::Vertex> vertices;

		sf::Color red(sf::Color::Red);
		red.a = 122;

		float maxAngle = m_fWatchAngle + m_fWatchArea / 2;
		float minAngle = m_fWatchAngle - m_fWatchArea / 2;

		if (maxAngle > 360)
		{
			maxAngle -= 360;
		}

		if (minAngle < 0)
		{
			minAngle += 360;
		}

		vertices.push_back(sf::Vertex(getPosition(), red));

		int counter = 0;

		for (int angle =(int)minAngle; angle != (int)maxAngle; angle++)
		{
			counter++;
			if (counter > 360)
				break;
			if (angle < 0)
				angle += 359;
			else if (angle > 360)
				angle -= 361;

			if (maxAngle < minAngle)
			{
				if (angle > (int)maxAngle && angle < (int)minAngle)
					break;
			}
			vertices.push_back(sf::Vertex(sf::Vector2f(cosf(angle * 0.017453292519943) * m_fViewDistance, sinf(angle * 0.017453292519943) * m_fViewDistance) + getPosition(), red));
		}

		vertex1.position = sf::Vector2f(cosf(minAngle * 0.017453292519943) * m_fViewDistance, sinf(minAngle * 0.017453292519943) * m_fViewDistance) + getPosition();
		vertex2.position = sf::Vector2f(cosf(maxAngle * 0.017453292519943) * m_fViewDistance, sinf(maxAngle * 0.017453292519943) * m_fViewDistance) + getPosition();

		vertex1.color = red;
		vertex2.color = red;

		


		target.draw(&vertices[0], vertices.size(), sf::TrianglesFan);

		states.transform *= getTransform();
		target.draw(*m_xSprite, states);
	}

	void Guard::update(float p_fDeltaTime)
	{
		if (m_isDead)
			return;

		if (m_bIsRemoved)
			return;

		if (m_bAngleLocked)
			return;
			
		m_xAIManager->update(p_fDeltaTime);

		/*m_bIsChasing = false;

		float fTurnRate = p_fDeltaTime * 20.f;

		if (m_xPlayerObject == nullptr)
		{
			return;
		}

		float xDiff = getPosition().x - m_xPlayerObject->getPosition().x;
		float yDiff = getPosition().y - m_xPlayerObject->getPosition().y;

		float fDistance = sqrtf(xDiff * xDiff + yDiff * yDiff);

		setRotation(m_fWatchAngle);

		float fAngle = 0;

		if (fDistance < m_fViewDistance)
		{
			fAngle = atan2f(yDiff, xDiff) / 0.017453292519943 + 180;
			searchForPlayer(xDiff, yDiff, fDistance, fAngle);
		}

		if (fDistance < m_xPlayerObject->getNoise())
		{
			m_bIsChasing = true;
			setDirection(m_xPlayerObject->getPosition());
		}

		if (fabs(getPosition().x - m_xPlayerObject->getattack()->getPosition().x) < m_xPlayerObject->getattack()->getRadius() &&
			fabs(getPosition().y - m_xPlayerObject->getattack()->getPosition().y) < m_xPlayerObject->getattack()->getRadius())
		{
			m_bIsRemoved = true;
			//GameObject *card = m_xGobjManager->createObject(getPosition(), sf::Vector2f(64, 64), true, KEYCARD, m_xLevel->getspritemanager()->loadSprite("Keycard_pu.png", 0, 0, 64, 64));
			//setPosition(5000, 5000);
		}

		if (m_bIsChasing && m_xPlayerObject != nullptr && m_xPlayerObject->m_hiding == false)
		{
			if (fAngle > m_fWatchAngle)
			{
				if (fabs(fAngle - m_fWatchAngle) > 180)
				{
					m_fWatchAngle -= fTurnRate;
					if (m_fWatchAngle < 0)
					{
						m_fWatchAngle += 360;
					}
				}
				else
				{
					m_fWatchAngle += fTurnRate;
				}
				
			}
			else if (fAngle < m_fWatchAngle)
			{
				if (fabs(fAngle - m_fWatchAngle) > 180)
				{
					m_fWatchAngle += fTurnRate;
					if (m_fWatchAngle > 360)
					{
						m_fWatchAngle -= 360;
					}
				}
				else
				{
					m_fWatchAngle -= fTurnRate;
				}
				
			}

			setRotation(m_fWatchAngle);

			//move((m_xPlayerObject->getPosition() - getPosition()) / fDistance * m_fGuardChaseSpeed * p_fDeltaTime);
			//setPosition(getPosition() + ((m_xPlayerObject->getPosition() - getPosition()) / fDistance * m_fGuardChaseSpeed * p_fDeltaTime));

			m_xAIManager->update(p_fDeltaTime);
		}
		else if (m_bIsChasing == false &&
			fabs(getPosition().x - m_v2fStartPosition.x) >= 10 &&
			fabs(getPosition().y - m_v2fStartPosition.y) >= 10 && m_bIsPatrolling == false)
		{
			float angleToStart = atan2f(fabs(getPosition().x - m_v2fStartPosition.x), fabs(getPosition().y - m_v2fStartPosition.y)) / 0.017453292519943 + 180;
			m_fWatchAngle = (fabs(getPosition().x - m_v2fStartPosition.x), (fabs(getPosition().y - m_v2fStartPosition.y)), 0, angleToStart);
			float StartDistance = sqrtf(m_v2fStartPosition.x * m_v2fStartPosition.x + m_v2fStartPosition.y * m_v2fStartPosition.x);
			//move((m_v2fStartPosition - getPosition()) * 0.6f * p_fDeltaTime);
			setPosition(getPosition() + ((m_v2fStartPosition - getPosition()) * 0.6f * p_fDeltaTime));
		}
		else if (m_bHasConstantRotation)
		{
			if (m_bIsRotatingClockwise)
			{
				m_fWatchAngle += fTurnRate;
				if (m_fWatchAngle > 360)
				{
					m_fWatchAngle -= 360;
				}
			}
			else
			{
				m_fWatchAngle -= fTurnRate;
				if (m_fWatchAngle < 0)
				{
					m_fWatchAngle += 360;
				}
			}
		}
		else if (m_bIsPatrolling)
		{
			if (m_iPatrolPointCount == 0)
			{
				return;
			}

			sf::Vector2f v2fCurrentTarget = *m_vPatrolPoints[m_iCurrentPatrolPointIndex];

			float fTargetxDiff = v2fCurrentTarget.x - getPosition().x;
			float fTargetyDiff = v2fCurrentTarget.y - getPosition().y;

			float fTargetDistance = sqrt(fTargetxDiff * fTargetxDiff + fTargetyDiff * fTargetyDiff);

			sf::Vector2f v2fMovementVector(fTargetxDiff / fTargetDistance * m_fGuardPatrolSpeed * p_fDeltaTime, fTargetyDiff / fTargetDistance * m_fGuardPatrolSpeed * p_fDeltaTime);

			float fMovementDistance = sqrt(v2fMovementVector.x * v2fMovementVector.x + v2fMovementVector.y * v2fMovementVector.y);

			if (fMovementDistance > fTargetDistance)
			{
				setPosition(v2fCurrentTarget);

				m_iCurrentPatrolPointIndex++;

				if (m_iCurrentPatrolPointIndex >= m_iPatrolPointCount)
				{
					m_iCurrentPatrolPointIndex = 0;
				}
			}
			else
			{
				setDirection(v2fCurrentTarget);

				setPosition(getPosition() + v2fMovementVector);

				//move(v2fMovementVector);
			}
		}
		else
		{
			if (m_bIsRotating == true)
			{
				if (m_fMaxRotationAngle > m_fMinRotationAngle)
				{
					if (m_bIsRotatingClockwise == true)
					{
						m_fWatchAngle += fTurnRate;
						if (m_fWatchAngle > m_fMaxRotationAngle)
						{
							m_bIsRotatingClockwise = false;
						}
					}
					else
					{
						m_fWatchAngle -= fTurnRate;
						if (m_fWatchAngle < m_fMinRotationAngle)
						{
							m_bIsRotatingClockwise = true;
						}
					}
				}
				else
				{
					if (m_bIsRotatingClockwise == true)
					{
						m_fWatchAngle += fTurnRate;
						if (m_fWatchAngle > 360)
						{
							m_fWatchAngle -= 360;
							m_bHasPassedZero = true;
						}

						if (m_fWatchAngle > m_fMaxRotationAngle && m_bHasPassedZero)
						{
							m_bHasPassedZero = false;
							m_bIsRotatingClockwise = false;
						}
					}
					else
					{
						m_fWatchAngle -= fTurnRate;
						if (m_fWatchAngle < 0)
						{
							m_fWatchAngle += 360;
							m_bHasPassedZero = true;;
						}

						if (m_fWatchAngle < m_fMinRotationAngle && m_bHasPassedZero)
						{
							m_bHasPassedZero = false;
							m_bIsRotatingClockwise = true;
						}
					}
				}
			}

			setRotation(m_fWatchAngle);
		}*/
	}

	void Guard::searchForPlayer(float p_fxDiff, float p_fyDiff, float p_fDistance, float p_fAngle)
	{
		sf::Vector2f v2fPlayerPos = m_xPlayerObject->getPosition();

		if (m_fWatchAngle + m_fWatchArea / 2 > 360)
		{
			if (p_fAngle > m_fWatchAngle - m_fWatchArea/2 || p_fAngle < m_fWatchAngle + m_fWatchArea/2 - 360)
			{
				m_bIsChasing = true;
			}
		}
		else if (m_fWatchAngle - m_fWatchArea / 2 < 0)
		{
			if (p_fAngle > m_fWatchAngle - m_fWatchArea / 2 + 360 || p_fAngle < m_fWatchAngle + m_fWatchArea / 2)
			{
				m_bIsChasing = true;
			}
		}
		else
		{
			if (p_fAngle > m_fWatchAngle - m_fWatchArea / 2 && p_fAngle < m_fWatchAngle + m_fWatchArea / 2)
			{
				m_bIsChasing = true;
			}
		}
	}

	void Guard::setDirection(sf::Vector2f p_v2fDirection)
	{
		float fxDiff = getPosition().x - p_v2fDirection.x;
		float fyDiff = getPosition().y - p_v2fDirection.y;

		m_fWatchAngle = atan2f(fyDiff, fxDiff) / 0.017453292519943 + 180;
	}

	void Guard::setWatchSize(float p_fAngle)
	{
		m_fWatchArea = p_fAngle;
	}

	void Guard::setRotationPoints(float p_fMaxAngle, float p_fMinAngle, bool p_bClockwise)
	{
		m_bIsRotating = true;

		m_fMaxRotationAngle = p_fMaxAngle;
		m_fMinRotationAngle = p_fMinAngle;

		m_bIsRotatingClockwise = p_bClockwise;

		m_bHasPassedZero = false;

		m_bHasConstantRotation = false;
	}

	bool Guard::getIsRotating()
	{
		return m_bIsRotating;
	}

	void Guard::setVisionRange(float p_fRange)
	{
		m_fViewDistance = p_fRange;
	}

	void Guard::setConstantRotation(bool p_fClockwise)
	{
		m_bIsRotating = true;
		m_bIsRotatingClockwise = p_fClockwise;
		m_bHasConstantRotation = true;
	}

	void Guard::setAngle(float p_fAngle)
	{
		m_fWatchAngle = p_fAngle;
		m_fStartAngle = p_fAngle;
	}

	void Guard::reset()
	{
		m_isDead = false;
		m_bIsRemoved = false;
		m_fWatchAngle = m_fStartAngle;
		setPosition(m_v2fStartPosition);
		m_iCurrentPatrolPointIndex = 0;
	}

	void Guard::addPatrolPoints(sf::Vector2f *p_v2fPatrolPoint)
	{
		m_vPatrolPoints.push_back(p_v2fPatrolPoint);
		m_iPatrolPointCount++;
	}

	void Guard::removePatrolPoint(int m_iIndex)
	{
		delete m_vPatrolPoints[m_iIndex];

		m_vPatrolPoints[m_iIndex] = nullptr;

		for (auto iter = m_vPatrolPoints.begin(); iter != m_vPatrolPoints.end(); ++iter)
		{
			if (*iter == m_vPatrolPoints[m_iIndex])
			{
				m_vPatrolPoints.erase(iter);
				m_iPatrolPointCount--;
				return;
			}
		}
	}

	void Guard::clearPatrolPoints()
	{
		for (auto point : m_vPatrolPoints)
		{
			delete point;
			point = nullptr;
		}

		m_vPatrolPoints.clear();

		m_iPatrolPointCount = 0;
	}

	std::vector<sf::Vector2f*> Guard::getPatrolPoints()
	{
		return m_vPatrolPoints;
	}

	void Guard::setIsPatrolling(bool p_bIsPatrolling)
	{
		m_bIsPatrolling = p_bIsPatrolling;
	}

	bool Guard::getIsPatrolling()
	{
		return m_bIsPatrolling;
	}

	void Guard::setPatrolPath(std::vector<sf::Vector2f*> p_vPatrolPath)
	{
		m_vPatrolPoints = p_vPatrolPath;
		m_iPatrolPointCount = m_vPatrolPoints.size();
	}

	void Guard::attachAi(AIManager *p_xAIManager)
	{
		m_xAIManager = p_xAIManager;
		m_bHasAI = true;
	}

	void Guard::addDrawPath(std::vector<sf::Vector2f*> p_vPath)
	{
		m_vPath = p_vPath;
	}
}