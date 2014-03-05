#pragma once

#include "stdafx.h"
#include "Weapon.h"

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

	private:
		Level *m_xLevel;
	};
}