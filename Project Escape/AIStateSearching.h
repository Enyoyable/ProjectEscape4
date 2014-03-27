#pragma once

#include "stdafx.h"

#include "AIState.h"

namespace esc
{

	class Guard;
	class Level;

	class AIStateSearching : public AIState
	{
	public:
		AIStateSearching(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer);

		virtual void update(float p_fDeltaTime);

		virtual void takeDecision();

		virtual void enter();

		virtual void exit();

		void setPeriodAngles();

		bool checkAngle(float p_fAngle);

	private:
		float m_fSearchTimer;
		std::vector<float> m_vSearchPeriods;
		std::vector<float> m_vSearchAngles;
		float m_fEnterAngle;

		int m_iCurrentSearchIndex;
	};

}