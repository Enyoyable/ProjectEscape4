#pragma once

#include "stdafx.h"
#include "AIState.h"


namespace esc
{

	class Guard;

	class AIStateAlerted : public AIState
	{
	public:
		AIStateAlerted(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer);
		
		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

		virtual void enter();

		virtual void exit();

		void setTarget(sf::Vector2f p_v2fTarget);

	private:
		sf::Vector2f m_v2fTarget;
	};

}