#include "AIStatePatrolling.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "AIManager.h"
#include "Level.h"
#include "Animator.h"

namespace esc
{

	AIStatePatrolling::AIStatePatrolling(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{
		m_bHasReturned = true;
	}

	void AIStatePatrolling::update(float p_fDeltaTime)
	{
		if (!m_bHasReturned)
		{
			m_bHasReturned = m_xGuard->followPath(p_fDeltaTime);
		}
		else if (m_xGuard->getIsPatrolling())
		{
			m_xGuard->updatePatrolling(p_fDeltaTime);

			m_xGuard->getAnimator()->setCurrentAnimation("guard_walking.txt");
		}
	}

	void AIStatePatrolling::takeDecision()
	{
		if (m_xGuard->searchForPlayer(m_xPlayer) && !m_xPlayer->m_hiding)
		{
			m_xAiManager->setCurrentState(AIManager::CHASING);
		}
			
	}

	void AIStatePatrolling::enter()
	{
		printf("Now Patrolling!\n");

		PathFind xPathFind(m_xPathFind);

		m_xGuard->followPathCount = 0;

		m_xGuard->setFollowPath(xPathFind.pathToPosition(m_xGuard, &m_xGuard->m_v2fStartPosition));

		m_xGuard->setMovementSpeed(120.f);

		m_xGuard->getAnimator()->setCurrentAnimation("guard_walking.txt");
	}

	void AIStatePatrolling::exit()
	{
		m_bHasReturned = false;
	}

}