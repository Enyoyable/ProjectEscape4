#pragma once

#include "stdafx.h"
#include "AIState.h"

namespace esc
{

	class AIManager;
	class GameObject;
	class PathFind;
	class PlayerObject;
	class Guard;

	class AIStateStationary : public AIState
	{
	public:
		AIStateStationary(AIManager *p_xAIManager, Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

		virtual void enter();

		virtual void exit();

	private:
		bool m_bHasReturned;
	};

}