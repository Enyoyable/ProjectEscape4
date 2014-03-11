#pragma once

#include "stdafx.h"
#include "AIState.h"

namespace esc
{

	class AIManager;
	class GameObject;
	class PathFind;
	class PlayerObject;

	class AIStatePatrolling : public AIState
	{
	public:
		AIStatePatrolling(AIManager *p_xAIManager, GameObject *p_xGameObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();
	};

}