#pragma once

#include "stdafx.h"
#include "State.h"

namespace esc
{

	class GameObjectManager;
	class StateManager;
	class SpriteManager;
	class Engine;
	class CollisionManager;
	class Level;
	class PlayerObject;

	class GameState : public State
	{
	public:
		GameState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager, Level *p_xLevel);

		virtual void init();

		virtual void update(float p_fDeltaTime);

		virtual void draw();

		virtual void exit();

		virtual void pause();

	protected:
		Level *m_xLevel;

		PlayerObject *m_xPlayer;

		sf::View *m_xView;

		float m_fTimer;

		bool m_bHasInitialized = false;
	};

}