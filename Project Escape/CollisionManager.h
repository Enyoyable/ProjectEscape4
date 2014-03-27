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

		bool getCollisionWithLine(std::vector<GameObject*> *p_vCollisionObjects, sf::Vector2f p_v2iPoint1, sf::Vector2f p_v2iPoint2);

		bool lineCollision(sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2, double out);

		double dot(sf::Vector2f a, sf::Vector2f b);

		double perpDot(sf::Vector2f a, sf::Vector2f b);

	private:
		std::vector<GameObject*> *m_vRemoveVector;

		
	};


}