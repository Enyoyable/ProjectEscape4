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
	class HUD;

	class GameState : public State
	{
	public:
		GameState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager);

		virtual void init();

		virtual void update(float p_fDeltaTime);

		virtual void draw();

		virtual void exit();

		virtual void pause();

	protected:
		Level *m_xLevel;

		PlayerObject *m_xPlayer;

		sf::View *m_xView;

		sf::Music *m_sIngame;

		bool m_bHasInitialized = false;

		float m_fRippleTimer;

		HUD *hud;

		sf::Time m_tTimer;

		sf::Clock m_cClock;

	};

}