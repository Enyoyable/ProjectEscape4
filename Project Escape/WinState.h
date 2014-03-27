#pragma once

#include "stdafx.h"
#include "State.h"

namespace esc
{

	class GameObjectManager;
	class StateManager;
	class Button;
	class SpriteManager;
	class Engine;
	class CollisionManager;
	class SoundManager;
	class GameObject;

	class WinState : public State
	{
	public:
		WinState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager);

		virtual void init();

		virtual void update(float p_fDeltaTime);

		virtual void draw();

		virtual void exit();

		virtual void pause();

	private:



		std::vector<sf::Text*> m_vTextObjects;

		sf::Music *m_sWin;

		GameObject *m_xWinBack;

		Button *m_xRestartButton;
		Button *m_xMenuButton;
		Button *m_xExitButton;


		SoundManager *m_xSoundManager;
	};

}