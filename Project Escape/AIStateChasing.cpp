#include "AIStateChasing.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "AIManager.h"

namespace esc
{
	AIStateChasing::AIStateChasing(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{
		m_fPathTimer = 1;
	}

	void AIStateChasing::update(float p_fDeltaTime)
	{
		m_fPathTimer += p_fDeltaTime;

		std::vector<sf::Vector2f*> vPathToPlayer;

		if (fabs(m_xGuard->getPosition().x - m_xPlayer->getPosition().x) > 64 || fabs(m_xGuard->getPosition().y - m_xPlayer->getPosition().y) > 64)
			vPathToPlayer = m_xPathFind->pathToObject(m_xGuard, m_xPlayer);
		else
		{
			sf::Vector2f v2fCurrentPos = m_xGuard->getPosition();

			sf::Vector2f v2fTargetPosition = m_xPlayer->getPosition();

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGuard->move(v2fMovement * p_fDeltaTime * 180.0f);
			m_xGuard->setDirection(v2fTargetPosition);
			m_xGuard->setRotation(m_xGuard->m_fWatchAngle);
			return;
		}
		
		
		

			sf::Vector2f v2fCurrentPos = m_xGuard->getPosition();

		if (vPathToPlayer.size() == 0)
		{
			sf::Vector2f v2fTargetPosition = m_xPlayer->getPosition();

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGuard->move(v2fMovement * p_fDeltaTime * 180.0f);
			m_xGuard->setDirection(v2fTargetPosition);
			m_xGuard->setRotation(m_xGuard->m_fWatchAngle);
		}
		else
		{
			sf::Vector2f v2fTargetPosition = *vPathToPlayer[vPathToPlayer.size() - 1];

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGuard->move(v2fMovement * p_fDeltaTime * 180.0f);
			m_xGuard->setDirection(m_xPlayer->getPosition());
			m_xGuard->setRotation(m_xGuard->m_fWatchAngle);
		}

		if (vPathToPlayer.size() > 0)
		{
			for (auto path : vPathToPlayer)
			{
				delete path;
				path = nullptr;
			}

			vPathToPlayer.clear();
		}
	}

	void AIStateChasing::takeDecision()
	{
		if (!m_xGuard->searchForPlayer(m_xPlayer) || m_xPlayer->m_hiding)
		{
			if (m_xGuard->getIsPatrolling())
			{
				m_xAiManager->setCurrentState(AIManager::PATROLLING);
			}
			else if (m_xGuard->getIsRotating())
			{
				m_xAiManager->setCurrentState(AIManager::STATIONARY);
			}
			
		}
	}

	void AIStateChasing::enter()
	{

	}

	void AIStateChasing::exit()
	{

	}
}