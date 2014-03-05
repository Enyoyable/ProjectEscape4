#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace esc
{

	class GameObject;

	class Bullet : public GameObject
	{
	public:
		Bullet(sf::Vector2f p_v2fPosition, sf::Vector2f p_v2fSize, sf::Vector2f p_v2fDirection, GameObject *p_xShootObject, sf::Sprite *p_xSprite = nullptr);

		virtual void update(float fDeltaTime);

		bool HandleCollision(GameObject *p_xGameObject, std::vector<GameObject*> *p_vRemoveVector);

	private:
		sf::Vector2f m_v2fMovementVector;

		GameObject *m_xShootObject;
	};

}