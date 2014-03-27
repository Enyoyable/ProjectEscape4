#include "AIState.h"
#include "Level.h"
#include "Guard.h"
#include "PlayerObject.h"
#include "AIManager.h"

namespace esc
{

	AIState::AIState(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer)
	{
		m_xAiManager = p_xAIManager;
		m_xGuard = p_xGuard;
		m_xPathFind = p_xPathFind;
		m_xPlayer = p_xPlayer;
	}

	void AIState::update(float p_fDeltaTime)
	{

	}

	void AIState::takeDecision()
	{

	}

	void AIState::enter()
	{

	}

	void AIState::exit()
	{

	}

}