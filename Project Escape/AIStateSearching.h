#pragma once

#include "stdafx.h"

#include "AIState.h"

namespace esc
{

	class Guard;

	class AIStateSearching : public AIState
	{
	public:
		AIStateSearching(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

		virtual void enter();

		virtual void exit();
	};

}