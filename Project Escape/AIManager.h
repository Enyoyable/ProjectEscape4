#pragma once

#include "stdafx.h"

namespace esc
{

	class GameObject;
	class AIState;
	class PathFind;
	class PlayerObject;

	class AIManager
	{

		enum EAIStates
		{
			PATROLLING,
			STATIONARY,
			ROTATING,
			CHASING
		};

	public:
		AIManager(GameObject *p_xObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer);

		void update(float p_fDeltaTime);

		void updateCurrentState(float p_fDeltaTime);

		void setCurrentState(EAIStates p_eNewState);

		EAIStates getCurrentState();

	private:
		GameObject *m_xObject;

		std::map<EAIStates, AIState*> m_mAIStates;

		EAIStates m_eCurrentState;

		PlayerObject *m_xPlayer;
	};

}