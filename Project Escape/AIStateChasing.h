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
	class Level;

	class AIStateChasing : public AIState
	{
	public:
		AIStateChasing(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

		virtual void enter();

		virtual void exit();

	private:
		std::vector<sf::Vector2f*> m_vCurrentPath;

		float m_fPathTimer;


	};

}