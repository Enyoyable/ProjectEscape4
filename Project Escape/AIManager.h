#pragma once

#include "stdafx.h"

namespace esc
{

	class GameObject;
	class AIState;
	class PathFind;
	class PlayerObject;
	class Guard;
	class AIManager
	{
	public:
		enum EAIStates
		{
			PATROLLING,
			STATIONARY,
			CHASING,
			SEARCHING,
			ALERTED
		};

	public:
		AIManager(Guard *p_xGuard, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		void update(float p_fDeltaTime);

		void updateCurrentState(float p_fDeltaTime);

		void setCurrentState(EAIStates p_eNewState);

		void forceCurrentState(EAIStates p_eNewState);

		EAIStates getCurrentStateID();
		AIState *getCurrentState();

	private:
		Guard *m_xGuard;

		std::map<EAIStates, AIState*> m_mAIStates;

		EAIStates m_eCurrentState;

		PlayerObject *m_xPlayer;
	};

}