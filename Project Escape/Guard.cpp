#include "Guard.h"
#include "PlayerObject.h"
#include "AIManager.h"
#include "PathFind.h"
#include "AIStateAlerted.h"
#include "SoundRipple.h"
#include "Level.h"
#include "CollisionManager.h"
#include "Animator.h"
#include "AnimatedSprite.h"

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

		m_xLastRipple = nullptr;
		m_fMovementSpeed = 60.0f;

		m_bOriginalVisionRangeSet = false;

		m_bOriginalVisionSizeSet = false;
		m_xAnimator = nullptr;
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

		

		if (!m_bAngleLocked)
			target.draw(&vertices[0], vertices.size(), sf::TrianglesFan);

		states.transform *= getTransform();

		if (!m_bAngleLocked)
			target.draw(*m_xAnimator->getCurrentAnimation(), states);
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

		m_xAnimator->update(p_fDeltaTime);

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
		*/
	}

	bool Guard::searchForPlayer(PlayerObject *p_xObject)
	{
		sf::Vector2f v2fPlayerPos = p_xObject->getPosition();


		float xDiff = getPosition().x - v2fPlayerPos.x;
		float yDiff = getPosition().y - v2fPlayerPos.y;

		float fDistance = sqrtf(xDiff * xDiff + yDiff * yDiff);

		float fAngle = atan2f(yDiff, xDiff) / 0.017453292519943 + 180;

		

		if (fDistance > m_fViewDistance)
			return false;

		if (m_fWatchAngle + m_fWatchArea / 2 > 360)
		{
			if (fAngle > m_fWatchAngle - m_fWatchArea/2 || fAngle < m_fWatchAngle + m_fWatchArea/2 - 360)
			{
				return checkWalls(fAngle, fDistance);
			}
		}
		else if (m_fWatchAngle - m_fWatchArea / 2 < 0)
		{
			if (fAngle > m_fWatchAngle - m_fWatchArea / 2 + 360 || fAngle < m_fWatchAngle + m_fWatchArea / 2)
			{
				return checkWalls(fAngle, fDistance);
			}
		}
		else
		{
			if (fAngle > m_fWatchAngle - m_fWatchArea / 2 && fAngle < m_fWatchAngle + m_fWatchArea / 2)
			{
				return checkWalls(fAngle, fDistance);
			}
		}

		return false;
	}
	
	bool Guard::checkWalls(float p_fAngleToPlayer, float p_fDistanceToPlayer)
	{
		int iTileDistance = m_fWatchArea / 64;

		int iPlayerDistance = p_fDistanceToPlayer / 64;

		if (iPlayerDistance > iTileDistance + 2)
			return true;

		sf::Vector2i v2iCurrentTilePosition((getPosition().x) / 64, (getPosition().y) / 64);

		sf::Vector2i v2iCurrentPlayerTilePosition((m_xPlayerObject->getPosition().x) / 64, (m_xPlayerObject->getPosition().y ) / 64);

		GameObject *aWalls[200][200];

		m_xLevel->getWalls(aWalls);

		std::vector<GameObject*> vWalls;

		for (int y = v2iCurrentPlayerTilePosition.y - 12; y <= v2iCurrentPlayerTilePosition.y + 12; y++)
		{

			if (y < 0)
				y = 0;

			if (y == 200)
				break;

			for (int x = v2iCurrentPlayerTilePosition.x - 12; x <= v2iCurrentPlayerTilePosition.x + 12; x++)
			{
				if (x < 0)
					x = 0;

				if (x == 200)
					break;

				if (aWalls[x][y] != nullptr)
					vWalls.push_back(aWalls[x][y]);
			}
		}

		float playerRotation = m_xPlayerObject->getRotation();

		std::vector<float> vRotations;

		vRotations.push_back(playerRotation);

		for (int i = 1; i < 8; i++)
		{
			float newRotation = playerRotation + i * 45;

			if (newRotation > 360)
				newRotation -= 360;

			vRotations.push_back(newRotation);
		}

		CollisionManager manager;

		int collcount = 0;

		for (auto rotation : vRotations)
		{
			float x = cosf((rotation - 180) * 0.017453292519943) * 32 + m_xPlayerObject->getPosition().x;
			float y = sinf((rotation - 180) * 0.017453292519943) * 32 + m_xPlayerObject->getPosition().y;

			if (!manager.getCollisionWithLine(&vWalls, sf::Vector2f(x, y), getPosition()))
				return true;
		}

		return false;
	}

	void Guard::setDirection(sf::Vector2f p_v2fDirection)
	{
		float fxDiff = getPosition().x - p_v2fDirection.x;
		float fyDiff = getPosition().y - p_v2fDirection.y;

		m_fWatchAngle = atan2f(fyDiff, fxDiff) / 0.017453292519943 + 180;
	}

	void Guard::setWatchSize(float p_fAngle)
	{
		if (!m_bOriginalVisionSizeSet)
		{
			m_fOriginalVisionSize = p_fAngle;
			m_bOriginalVisionSizeSet = true;
		}

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
		if (!m_bOriginalVisionRangeSet)
		{
			m_fOriginalVisionRange = p_fRange;
			m_bOriginalVisionRangeSet = true;
		}

		m_fViewDistance = p_fRange;
	}

	void Guard::resetVision()
	{
		m_fViewDistance = m_fOriginalVisionRange;
		m_fWatchArea = m_fOriginalVisionSize;
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
		setRotation(p_fAngle);
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

	void Guard::alert(sf::Vector2f p_v2fPosition)
	{
		
	}

	void Guard::updateRotation(float p_fDeltaTime)
	{
		float fTurnRate = 45.f;

		if (m_bHasConstantRotation)
		{
			if (m_bIsRotatingClockwise)
			{
				m_fWatchAngle += fTurnRate * p_fDeltaTime;
				if (m_fWatchAngle > 360)
				{
					m_fWatchAngle -= 360;
				}
			}
			else
			{
				m_fWatchAngle -= fTurnRate * p_fDeltaTime;
				if (m_fWatchAngle < 0)
				{
					m_fWatchAngle += 360;
				}
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
						m_fWatchAngle += fTurnRate * p_fDeltaTime;
						if (m_fWatchAngle > m_fMaxRotationAngle)
						{
							m_bIsRotatingClockwise = false;
						}
					}
					else
					{
						m_fWatchAngle -= fTurnRate * p_fDeltaTime;
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
						m_fWatchAngle += fTurnRate * p_fDeltaTime;
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
						m_fWatchAngle -= fTurnRate * p_fDeltaTime;
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
		}

		setRotation(m_fWatchAngle);
	}

	void Guard::updatePatrolling(float p_fDeltaTime)
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
		}

		setRotation(m_fWatchAngle);
	}

	sf::Vector2f *Guard::getCurrentPatrolPoint()
	{
		return m_vPatrolPoints[m_iCurrentPatrolPointIndex];
	}

	bool Guard::followPath(float p_fDeltaTime)
	{
		if (m_vFollowPath.size() == 0 || followPathCount == m_vFollowPath.size())
		{
			return true;
		}

		sf::Vector2f v2fCurrentTarget = *m_vFollowPath[m_vFollowPath.size() - 1 - followPathCount];

		float fTargetxDiff = v2fCurrentTarget.x - getPosition().x;
		float fTargetyDiff = v2fCurrentTarget.y - getPosition().y;

		float fTargetDistance = sqrt(fTargetxDiff * fTargetxDiff + fTargetyDiff * fTargetyDiff);

		sf::Vector2f v2fMovementVector(fTargetxDiff / fTargetDistance * m_fMovementSpeed * p_fDeltaTime, fTargetyDiff / fTargetDistance * m_fMovementSpeed * p_fDeltaTime);

		float fMovementDistance = sqrt(v2fMovementVector.x * v2fMovementVector.x + v2fMovementVector.y * v2fMovementVector.y);

		if (fMovementDistance > fTargetDistance)
		{
			setPosition(v2fCurrentTarget);

			followPathCount++;

			if (followPathCount == m_vFollowPath.size())
			{
				if (m_xAIManager->getCurrentStateID() == AIManager::ALERTED)
					return true;

				if (getIsPatrolling())
				{
					setPosition(m_v2fStartPosition);
					m_iCurrentPatrolPointIndex = 0;
				}
				else
				{
					setPosition(m_v2fStartPosition);
				}
				setRotation(m_fWatchAngle);
				return true;
			}
				
		}
		else
		{
			setDirection(v2fCurrentTarget);

			setPosition(getPosition() + v2fMovementVector);
		}

		setRotation(m_fWatchAngle);

		return false;
	}

	void Guard::setFollowPath(std::vector<sf::Vector2f*> p_v2fFollowPath)
	{
		printf("followpath set!\n");

		if (p_v2fFollowPath.size() == 0)
			return;

		if (m_vFollowPath.size() != 0)
		{
			for (auto path : m_vFollowPath)
			{
				delete path;
				path = nullptr;
			}

			m_vFollowPath.clear();
		}
		followPathCount = 0;

		m_vFollowPath = p_v2fFollowPath;

		if (m_xTarget != nullptr)
		{
			delete m_xTarget;
			m_xTarget = nullptr;
		}

		

		m_xTarget = new sf::CircleShape(20, 40);

		m_xTarget->setPosition(*m_vFollowPath[0] -sf::Vector2f(16, 16));
		m_xTarget->setFillColor(sf::Color::Green);
	}

	void Guard::HandleCollision(GameObject *p_xOtherObject)
	{
		if (m_xAIManager->getIsLocked())
			return;
		
		SoundRipple *ripple = static_cast<SoundRipple*>(p_xOtherObject);

		if (m_xLastRipple == ripple)
			return;

		if (m_xAIManager->getCurrentStateID() == AIManager::ALERTED)
		{
			AIStateAlerted *alerted = static_cast<AIStateAlerted*>(m_xAIManager->getCurrentState());
			alerted->setNewTarget(ripple->getRipplePosition() + sf::Vector2f(32, 32));
		}
		else
		{
			m_xLastRipple = ripple;
			m_xAIManager->setCurrentState(AIManager::ALERTED);
			AIStateAlerted *alerted = static_cast<AIStateAlerted*>(m_xAIManager->getCurrentState());
			alerted->setTarget(ripple->getRipplePosition() + sf::Vector2f(32, 32));
		}
		
		
	}

	void Guard::setMovementSpeed(float p_fSpeed)
	{
		m_fMovementSpeed = p_fSpeed;
	}

	float Guard::getMovementSpeed()
	{
		return m_fMovementSpeed;
	}

	void Guard::attachAnimator(Animator *p_xAnimator)
	{
		m_xAnimator = p_xAnimator;

		m_xAnimator->loadAnimations("guardAnims.txt");

		m_xAnimator->setCurrentAnimation("guard_idle.txt");
	}

	Animator *Guard::getAnimator()
	{
		return m_xAnimator;
	}
}