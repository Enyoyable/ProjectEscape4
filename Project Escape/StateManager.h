#pragma once

#include "stdafx.h"

namespace esc
{

	class State;

	

	class StateManager
	{
	public:

		enum EStates
		{
			MENU,
			GAME,
			PAUSE,
			OPTION,
			DIALOGUE,
			COUNT
		};

	public:
		StateManager();

		void setCurrentState(EStates p_eNewState);
		EStates getCurrentState();

		void attachState(EStates p_eState, State *p_xState);
		void detachState(EStates p_eChosenState);

		void updateCurrentState(float p_fDeltaTime);

		void drawCurrentState();

	private:
		EStates m_eCurrentState;

		std::map<EStates, State*> m_mStates;
	};

}