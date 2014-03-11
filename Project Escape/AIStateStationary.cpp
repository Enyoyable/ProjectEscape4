#include "AIStateStationary.h"
#include "PathFind.h"
#include "Guard.h"
#include "GameObject.h"
#include "PlayerObject.h"

namespace esc
{

	AIStateStationary::AIStateStationary(AIManager *p_xAIManager, GameObject *p_xGameObject, PathFind *p_xPathFind, PlayerObject *p_xPlayer) : AIState(p_xAIManager, p_xGameObject, p_xPathFind, p_xPlayer)
	{

	}

	void AIStateStationary::update(float p_fDeltaTime)
	{

	}

	void AIStateStationary::takeDecision()
	{

	}

}