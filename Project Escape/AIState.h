#pragma once

#include "stdafx.h"

namespace esc
{

	class AIManager;
	class GameObject;
	class PathFind;
	class PlayerObject;
	class Guard;
	class Level;

	class AIState
	{
	public:
		AIState(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

		virtual void enter();

		virtual void exit();

	protected:
		AIManager *m_xAiManager;
		Guard *m_xGuard;
		Level *m_xPathFind;
		PlayerObject *m_xPlayer;
	};

}