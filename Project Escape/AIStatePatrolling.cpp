#include "AIStatePatrolling.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "AIManager.h"

namespace esc
{

	AIStatePatrolling::AIStatePatrolling(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
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
		m_xGuard->followPathCount = 0;

		m_xGuard->setFollowPath(m_xPathFind->pathToPosition(m_xGuard, &m_xGuard->m_v2fStartPosition));
	}

	void AIStatePatrolling::exit()
	{
		m_bHasReturned = false;
	}

}