#include "AIStateAlerted.h"
#include "Level.h"
#include "PathFind.h"
#include "Guard.h"
#include "Level.h"
#include "AIManager.h"
#include "PlayerObject.h"
#include "Animator.h"

namespace esc
{

	AIStateAlerted::AIStateAlerted(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{
		m_bTargetSet = false;
		m_bArrivedAtPoint = false;
	}

	void AIStateAlerted::update(float p_fDeltaTime)
	{
		
		if (m_bArrivedAtPoint = m_xGuard->followPath(p_fDeltaTime))
			m_xAiManager->setCurrentState(AIManager::SEARCHING);
		
	}

	void AIStateAlerted::takeDecision()
	{
		if (m_xGuard->searchForPlayer(m_xPlayer) && !m_xPlayer->m_hiding)
		{
			m_xAiManager->setCurrentState(AIManager::CHASING);
		}

		if (m_bArrivedAtPoint)
		{
			m_xAiManager->setCurrentState(AIManager::SEARCHING);
		}
	}

	void AIStateAlerted::setTarget(sf::Vector2f p_v2fTarget)
	{
		m_bTargetSet = false;
		m_v2fTarget = p_v2fTarget;

		pathToCurrentTarget();
		
		
	}

	void AIStateAlerted::enter()
	{
		printf("Now Alerted!\n");
		m_xGuard->setMovementSpeed(180.f);

		m_xGuard->setWatchSize(70);
		m_xGuard->setVisionRange(250);

		m_xGuard->getAnimator()->setCurrentAnimation("guard_run.txt");
	}

	void AIStateAlerted::exit()
	{
		//m_bTargetSet = false;
	}

	void AIStateAlerted::setNewTarget(sf::Vector2f p_v2fTarget)
	{
		m_v2fTarget = p_v2fTarget;

		pathToCurrentTarget();
	}

	void AIStateAlerted::pathToCurrentTarget()
	{
		if (m_xPathFinder != nullptr)
		{
			delete m_xPathFinder;
			m_xPathFinder = nullptr;
		}

		m_xPathFinder = new PathFind(m_xPathFind);

		m_bTargetSet = true;
		m_bArrivedAtPoint = false;

		m_xGuard->setFollowPath(m_xPathFinder->pathToPosition(m_xGuard, &m_v2fTarget));
	}
}