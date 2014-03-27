#include "AIStateChasing.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "AIManager.h"
#include "Level.h"
#include "Animator.h"

namespace esc
{
	AIStateChasing::AIStateChasing(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{
		m_fPathTimer = 1;
	}

	void AIStateChasing::update(float p_fDeltaTime)
	{
		m_fPathTimer += p_fDeltaTime;

		std::vector<sf::Vector2f*> vPathToPlayer;

		PathFind xPathFind(m_xPathFind);

		if (fabs(m_xGuard->getPosition().x - m_xPlayer->getPosition().x) > 64 || fabs(m_xGuard->getPosition().y - m_xPlayer->getPosition().y) > 64)
			vPathToPlayer = xPathFind.pathToObject(m_xGuard, m_xPlayer);
		else
		{
			sf::Vector2f v2fCurrentPos = m_xGuard->getPosition();

			sf::Vector2f v2fTargetPosition = m_xPlayer->getPosition();

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGuard->move(v2fMovement * p_fDeltaTime * m_xGuard->getMovementSpeed());
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

			m_xGuard->move(v2fMovement * p_fDeltaTime * m_xGuard->getMovementSpeed());
			m_xGuard->setDirection(v2fTargetPosition);
			m_xGuard->setRotation(m_xGuard->m_fWatchAngle);
		}
		else
		{
			sf::Vector2f v2fTargetPosition = *vPathToPlayer[vPathToPlayer.size() - 1];

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGuard->move(v2fMovement * p_fDeltaTime * m_xGuard->getMovementSpeed());
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
			m_xAiManager->setIsLocked(false);
			if (m_xGuard->getIsPatrolling())
			{
				m_xAiManager->setCurrentState(AIManager::PATROLLING);
			}
			else if (m_xGuard->getIsRotating())
			{
				m_xAiManager->setCurrentState(AIManager::STATIONARY);
			}
		}
		else
		{
			m_xAiManager->setIsLocked(true);
		}
	}

	void AIStateChasing::enter()
	{
		printf("Now Chasing!\n");

		m_xGuard->setMovementSpeed(200.f);

		m_xGuard->setWatchSize(120);
		m_xGuard->setVisionRange(250);

		m_xGuard->getAnimator()->setCurrentAnimation("guard_run.txt");
	}

	void AIStateChasing::exit()
	{
		m_xGuard->resetVision();
	}
}