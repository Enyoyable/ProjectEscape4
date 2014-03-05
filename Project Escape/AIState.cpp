#include "AIState.h"

namespace esc
{

	AIState::AIState(AIManager *p_xAIManager, GameObject *p_xGameObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer)
	{
		m_xAiManager = p_xAIManager;
		m_xGameObject = p_xGameObject;
		m_xPathFind = p_xPathFind;
		m_xPlayer = p_xPlayer;
	}

	void AIState::update(float p_fDeltaTime)
	{

	}

	void AIState::takeDecision()
	{

	}

}