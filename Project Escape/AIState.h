#pragma once

#include "stdafx.h"

namespace esc
{

	class AIManager;
	class GameObject;
	class PathFind;
	class PlayerObject;

	class AIState
	{
	public:
		AIState(AIManager *p_xAIManager, GameObject *p_xGameObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

	protected:
		AIManager *m_xAiManager;
		GameObject *m_xGameObject;
		PathFind *m_xPathFind;
		PlayerObject *m_xPlayer;
	};

}