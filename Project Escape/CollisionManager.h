#pragma once

#include "stdafx.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Guard.h"

namespace esc
{
	class CollisionManager
	{
	public:
		CollisionManager();

		void getCollision(std::vector<GameObject*> *p_collisionObjects); 

		void getCollisionWithPoint(std::vector<GameObject*> *p_collisionObjects, sf::Vector2i p_v2fPoint);
	private:
		std::vector<GameObject*> *m_vRemoveVector;
	};


}