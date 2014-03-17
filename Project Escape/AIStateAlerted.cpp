#include "AIStateAlerted.h"

namespace esc
{

	AIStateAlerted::AIStateAlerted(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{

	}

	void AIStateAlerted::update(float p_fDeltaTime)
	{

	}

	void AIStateAlerted::takeDecision()
	{

	}

	void AIStateAlerted::setTarget(sf::Vector2f p_v2fTarget)
	{
		m_v2fTarget = p_v2fTarget;
	}

	void AIStateAlerted::enter()
	{

	}

	void AIStateAlerted::exit()
	{

	}

}