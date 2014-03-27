#include "StateManager.h"
#include "State.h"

namespace esc
{

	StateManager::StateManager()
	{
		//m_eCurrentState = EStates::MENU;
	}

	void StateManager::setCurrentState(EStates p_eNewState)
	{
		m_eCurrentState = p_eNewState;
		m_mStates.find(m_eCurrentState)->second->init();
	}

	StateManager::EStates StateManager::getCurrentState()
	{
		return m_eCurrentState;
	}

	void StateManager::attachState(EStates p_eState, State *p_xState)
	{
		if (m_mStates.find(p_eState) == m_mStates.end())
		{
			m_mStates.insert(std::pair<EStates, State*>(p_eState, p_xState));
		}
	}

	void StateManager::detachState(EStates p_eChosenState)
	{
		if (m_mStates.find(p_eChosenState) != m_mStates.end())
		{
			m_mStates.erase(m_mStates.find(p_eChosenState));
		}
	}

	void StateManager::updateCurrentState(float p_fDeltaTime)
	{
		m_mStates.find(m_eCurrentState)->second->update(p_fDeltaTime);
	}

	void StateManager::drawCurrentState()
	{
		m_mStates.find(m_eCurrentState)->second->draw();
	}

}