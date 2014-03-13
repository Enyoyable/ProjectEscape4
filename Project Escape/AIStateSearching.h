#pragma once

#include "stdafx.h"

#include "AIState.h"

namespace esc
{

	class AIStateSearching : public AIState
	{
	public:
		AIStateSearching(AIManager *p_xAIManager, GameObject *p_xGameObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();
	};

}