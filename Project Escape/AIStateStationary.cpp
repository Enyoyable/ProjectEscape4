#include "AIStateStationary.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "AIManager.h"

namespace esc
{

	AIStateStationary::AIStateStationary(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{
		m_bHasReturned = true;
	}

	void AIStateStationary::update(float p_fDeltaTime)
	{
		
		if (!m_bHasReturned)
		{
			m_bHasReturned = m_xGuard->followPath(p_fDeltaTime);
		}
		else if(m_xGuard->getIsRotating())
		{
			m_xGuard->updateRotation(p_fDeltaTime);
		}
		
	}

	void AIStateStationary::takeDecision()
	{
		if (m_xGuard->searchForPlayer(m_xPlayer) && !m_xPlayer->m_hiding)
		{
			m_xAiManager->setCurrentState(AIManager::CHASING);
		}
	}

	void AIStateStationary::enter()
	{
		m_xGuard->followPathCount = 0;

		std::vector<sf::Vector2f*> path(m_xPathFind->pathToPosition(m_xGuard, &m_xGuard->m_v2fStartPosition));

		m_xGuard->setFollowPath(path);
	}

	void AIStateStationary::exit()
	{
		m_bHasReturned = false;
	}

}