#include "AIManager.h"

#include "AIState.h"

#include "PathFind.h"
#include "GameObject.h"
#include "AIStateChasing.h"
#include "AIStatePatrolling.h"
#include "AIStateSearching.h"
#include "AIStateStationary.h"

#include "Guard.h"

namespace esc
{
	AIManager::AIManager(GameObject *p_xObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer)
	{
		m_xObject = p_xObject;
		m_xPlayer = p_xPlayer;

		AIStateChasing *chase = new AIStateChasing(this, p_xObject, p_xPathFind, p_xPlayer);
		AIStatePatrolling *patrol = new AIStatePatrolling(this, p_xObject, p_xPathFind, p_xPlayer);
		AIStateSearching *search = new AIStateSearching(this, p_xObject, p_xPathFind, p_xPlayer);
		AIStateStationary *stationary = new AIStateStationary(this, p_xObject, p_xPathFind, p_xPlayer);

		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::CHASING, chase));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::PATROLLING, patrol));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::SEARCHING, search));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::STATIONARY, stationary));

		Guard *guard = static_cast<Guard*>(m_xObject);

		if (guard->getIsPatrolling())
		{
			m_eCurrentState = PATROLLING;
		}
		else 
		{
			m_eCurrentState = STATIONARY;
		}
	}

	void AIManager::update(float p_fDeltaTime)
	{
		m_mAIStates.find(m_eCurrentState)->second->update(p_fDeltaTime);
		m_mAIStates.find(m_eCurrentState)->second->takeDecision();
	}

	void AIManager::setCurrentState(EAIStates p_eNewState)
	{
		m_eCurrentState = p_eNewState;
	}

	AIManager::EAIStates AIManager::getCurrentState()
	{
		return m_eCurrentState;
	}
}