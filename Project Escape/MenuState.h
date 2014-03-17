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

	class MenuState : public State
	{
	public:
		MenuState(GameObjectManager *p_xGameObjectManager, StateManager *p_xStateManager, SpriteManager *p_xSpriteManager, Engine *p_xEngine, CollisionManager *p_xCollisionManager);

		virtual void init();

		virtual void update(float p_fDeltaTime);

		virtual void draw();

		virtual void exit();

		virtual void pause();

	private:
		std::vector<sf::Text*> m_vTextObjects;

		sf::Music *m_sTitle;

		GameObject *m_xMenuBack;

		Button *m_xStartButton;
		Button *m_xExitButton;
		Button *m_xOptionsButton;
		Button *m_xCreditsButton;


		SoundManager *m_xSoundManager;
	};

}