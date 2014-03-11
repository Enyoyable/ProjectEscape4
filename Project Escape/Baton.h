#pragma once

#include "stdafx.h"
#include "Weapon.h"

namespace esc
{

	class GameObjectManager;
	class SpriteManager;
	class Level;

	class Baton : public Weapon
	{
	public:
		Baton(bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects, Level *p_xLevel, GameObjectManager *p_xGameObjectManager, SpriteManager *p_xSpriteManager);

		virtual void update(float fDeltaTime);

		virtual void attack();

		virtual void trigger();

		virtual void drop();

		virtual void Throw();

		virtual void setTarget(sf::Vector2f p_v2fTarget);

	private:
		Level *m_xLevel;

		GameObjectManager *m_xGameObjectManager;

		sf::Vector2f m_v2fTarget;

		SpriteManager *m_xSpriteManager;
	};
}