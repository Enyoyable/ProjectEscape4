#pragma once

#include "stdafx.h"

#include "Weapon.h"

namespace esc
{

	class Garrote : public Weapon
	{
	public:
		Garrote(float p_fDelay, float p_fDuration, std::vector<GameObject*> *p_vObjects);

		virtual void update(float fDeltaTime);

		virtual void attack(bool p_bIsFinished);

		virtual void trigger(); 

	private:
		bool m_bHasStarted;
	};

}