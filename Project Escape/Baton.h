#pragma once

#include "stdafx.h"
#include "Weapon.h"
#include "Level.h"

namespace esc
{

	class Level;

	class Baton : public Weapon
	{
	public:
		Baton(bool p_bHasCharges, int p_iAmountOfCharges, float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects, Level *p_xLevel);

		virtual void update(float fDeltaTime);

		virtual void attack();

		virtual void trigger();

		virtual void Throw();

		void setTarget(sf::Vector2f p_v2fTarget);
	private:
		Level *m_xLevel;

		sf::Sprite *m_xSprite;

		sf::Vector2f m_v2fTarget;
	};
}