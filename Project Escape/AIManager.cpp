#include "AIManager.h"

#include "AIState.h"

#include "PathFind.h"
#include "GameObject.h"
#include "AIStateChasing.h"

namespace esc
{
	AIManager::AIManager(GameObject *p_xObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer)
	{
		m_xObject = p_xObject;
		m_xPlayer = p_xPlayer;

		AIStateChasing *chase = new AIStateChasing(this, p_xObject, p_xPathFind, p_xPlayer);

		m_mAIStates.insert(std::pair<EAIStates, AIState*>(EAIStates::CHASING, chase));
		m_eCurrentState = CHASING;
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