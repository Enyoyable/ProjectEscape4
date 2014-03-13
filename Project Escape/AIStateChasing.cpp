#include "AIStateChasing.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"

namespace esc
{
	AIStateChasing::AIStateChasing(AIManager *p_xAIManager, GameObject *p_xGameObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGameObject, p_xPathFind, p_xPlayer)
	{
		m_fPathTimer = 1;
	}

	void AIStateChasing::update(float p_fDeltaTime)
	{
		m_fPathTimer += p_fDeltaTime;

		std::vector<sf::Vector2f*> vPathToPlayer;

		//if (m_fPathTimer > 0.5f)
		//{
			vPathToPlayer = m_xPathFind->pathToObject(m_xGameObject, m_xPlayer);
			m_fPathTimer = 0;
			static_cast<Guard*>(m_xGameObject)->addDrawPath(vPathToPlayer);
		//}
		
		

		sf::Vector2f v2fCurrentPos = m_xGameObject->getPosition();

		if (vPathToPlayer.size() == 0)
		{
			sf::Vector2f v2fTargetPosition = m_xPlayer->getPosition();

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGameObject->move(v2fMovement * p_fDeltaTime * 60.0f);
			static_cast<Guard*>(m_xGameObject)->setDirection(v2fTargetPosition);
		}
		else
		{
			sf::Vector2f v2fTargetPosition = *vPathToPlayer[vPathToPlayer.size() - 1];

			sf::Vector2f v2fMovement = v2fTargetPosition - v2fCurrentPos;

			float fMovementLength = sqrtf(v2fMovement.x * v2fMovement.x + v2fMovement.y * v2fMovement.y);

			v2fMovement = v2fMovement / fMovementLength;

			m_xGameObject->move(v2fMovement * p_fDeltaTime * 60.0f);
			static_cast<Guard*>(m_xGameObject)->setDirection(m_xPlayer->getPosition());
		}

		if (vPathToPlayer.size() > 0)
		{
			for (auto path : vPathToPlayer)
			{
				delete path;
				path = nullptr;
			}

			vPathToPlayer.clear();
		}
	}

	void AIStateChasing::takeDecision()
	{

	}
}