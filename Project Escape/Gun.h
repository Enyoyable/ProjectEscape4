#pragma once

#include "stdafx.h"
#include "Weapon.h"

namespace esc
{

	class GameObjectManager;
	class SpriteManager;

	class Gun : public Weapon
	{
	public:
		Gun(bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects, GameObjectManager *p_xGameObjectManager, SpriteManager *p_xSpriteManager);

		virtual void update(float fDeltaTime);

		virtual void attack();

		virtual void trigger();

		virtual void Throw();

		virtual void drop();

		virtual void setTarget(sf::Vector2f p_v2fTarget);

	private:
		GameObjectManager *m_xGameObjectManager;

		sf::Vector2f m_v2fTarget;

		SpriteManager *m_xSpriteManager;

		sf::Music *m_sGunshot;

		bool m_bHasShot;
	};

}