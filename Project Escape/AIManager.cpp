#include "AIManager.h"

#include "AIState.h"

#include "PathFind.h"
#include "GameObject.h"
#include "AIStateChasing.h"
#include "AIStatePatrolling.h"
#include "AIStateSearching.h"
#include "AIStateStationary.h"
#include "AIStateAlerted.h"

#include "Level.h"

#include "PlayerObject.h"

#include "Guard.h"

namespace esc
{
	AIManager::AIManager(Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer)
	{
		m_xGuard = p_xGuard;
		m_xPlayer = p_xPlayer;

		AIStateChasing *chase = new AIStateChasing(this, m_xGuard, p_xPathFind, p_xPlayer);
		AIStatePatrolling *patrol = new AIStatePatrolling(this, m_xGuard, p_xPathFind, p_xPlayer);
		AIStateSearching *search = new AIStateSearching(this, m_xGuard, p_xPathFind, p_xPlayer);
		AIStateStationary *stationary = new AIStateStationary(this, m_xGuard, p_xPathFind, p_xPlayer);
		AIStateAlerted *alerted = new AIStateAlerted(this, m_xGuard, p_xPathFind, p_xPlayer);

		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::CHASING, chase));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::PATROLLING, patrol));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::SEARCHING, search));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::STATIONARY, stationary));
		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::ALERTED, alerted));


		if (m_xGuard->getIsPatrolling())
		{
			m_eCurrentState = PATROLLING;
		}
		else 
		{
			m_eCurrentState = STATIONARY;
		}

		m_bLockedState = false;
	}

	void AIManager::update(float p_fDeltaTime)
	{

		m_mAIStates.find(m_eCurrentState)->second->update(p_fDeltaTime);
		m_mAIStates.find(m_eCurrentState)->second->takeDecision();
	}

	void AIManager::setCurrentState(EAIStates p_eNewState)
	{
		if (m_bLockedState)
			return;

		auto it = m_mAIStates.find(m_eCurrentState);

		if (it == m_mAIStates.end())
			return;
		else
			it->second->exit();

		it = m_mAIStates.find(p_eNewState);

		if (it == m_mAIStates.end())
			return;
		else
			m_eCurrentState = p_eNewState;
		
		it = m_mAIStates.find(m_eCurrentState);

		if (it == m_mAIStates.end())
			return;
		else
			m_mAIStates.find(m_eCurrentState)->second->enter();
		
	}

	void AIManager::forceCurrentState(EAIStates p_eNewState)
	{
		m_eCurrentState = p_eNewState;
	}

	AIManager::EAIStates AIManager::getCurrentStateID()
	{
		return m_eCurrentState;
	}

	AIState *AIManager::getCurrentState()
	{
		return m_mAIStates.find(m_eCurrentState)->second;
	}

	void AIManager::setIsLocked(bool p_bIsLocked)
	{
		m_bLockedState = p_bIsLocked;
	}

	bool AIManager::getIsLocked()
	{
		return m_bLockedState;
	}
}