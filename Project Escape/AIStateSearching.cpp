#include "AIStateSearching.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Level.h"
#include "AIManager.h"
#include "Guard.h"
#include "Animator.h"

namespace esc
{

	AIStateSearching::AIStateSearching(AIManager *p_xAIManager, Guard *p_xGuard, Level *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGuard, p_xPathFind, p_xPlayer)
	{
		m_iCurrentSearchIndex = 0;
	}

	void AIStateSearching::update(float p_fDeltaTime)
	{
		m_fSearchTimer += p_fDeltaTime;

		if (m_fSearchTimer > m_vSearchPeriods[m_iCurrentSearchIndex])
		{
			m_fSearchTimer = 0;
			m_iCurrentSearchIndex++;

			if (m_iCurrentSearchIndex == m_vSearchPeriods.size())
			{
				if (m_xGuard->getIsPatrolling())
				{
					m_xAiManager->setCurrentState(AIManager::PATROLLING);
				}
				else
				{
					m_xAiManager->setCurrentState(AIManager::STATIONARY);
				}

				return;
			}

			m_xGuard->setAngle(m_vSearchAngles[m_iCurrentSearchIndex]);
		}
	}

	void AIStateSearching::takeDecision()
	{
		if (m_xGuard->searchForPlayer(m_xPlayer) && !m_xPlayer->m_hiding)
		{
			m_xAiManager->setCurrentState(AIManager::CHASING);
		}
	}

	void AIStateSearching::enter()
	{
		m_xGuard->setWatchSize(120);
		m_xGuard->setVisionRange(250);

		m_iCurrentSearchIndex = 0;

		m_fEnterAngle = m_xGuard->getRotation();

		printf("Current angle of the guard: %f\n", m_fEnterAngle);

		int searchPerionds = rand() % 5 + 3;

		m_vSearchPeriods.clear();

		for (int i = 0; i < searchPerionds; i++)
		{
			m_vSearchPeriods.push_back(1.f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}

		printf("Now Searching for %i periods!\n", searchPerionds);
		m_fSearchTimer = 0;

		setPeriodAngles();

		m_xGuard->getAnimator()->setCurrentAnimation("guard_idle.txt");
	}

	void AIStateSearching::exit()
	{
		m_xGuard->resetVision();
	}

	void AIStateSearching::setPeriodAngles()
	{
		m_vSearchAngles.push_back(m_fEnterAngle);

		int maxTrigger = 0;

		for (int i = 1; i < m_vSearchPeriods.size(); i++)
		{
			float tempangle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 360);

			while (checkAngle(tempangle))
			{
				if (++maxTrigger > 1000)
					break;

				tempangle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 360);
			}
				

			m_vSearchAngles.push_back(tempangle);
		}
	}

	bool AIStateSearching::checkAngle(float p_fAngle)
	{
		float offset = 20.f;

		bool illegal = false;

		

		for (auto angle : m_vSearchAngles)
		{

			if (angle + offset > 360)
			{
				if (p_fAngle < angle - offset || p_fAngle < angle + offset - 360)
					illegal = true;
			}
			else if (angle - offset < 0)
			{
				if (p_fAngle > angle - offset + 360 || p_fAngle < angle + offset)
					illegal = true;
			}
			else
			{
				if (p_fAngle < angle + offset && p_fAngle > angle - offset)
					illegal = true;
			}

		}

		return illegal;
	}

}