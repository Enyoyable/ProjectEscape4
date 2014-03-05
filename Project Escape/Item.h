#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace esc 
{
	class DrawManager;
	class Level;

	class Item : public GameObject
	{
	public:
		Item(sf::Vector2f p_v2Pos, sf::Sprite* p_sSprite, int p_iObjectId, EObjectType p_eType);

		virtual void update(float p_fDeltaTime);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void handleCollision(GameObject *p_oGameObject);

		void setIsRemoved(bool p_bIsGone);
		
		bool getIsRemoved();

		void setSpeed(sf::Vector2f p_v2fInpSpeed);
		sf::Vector2f getSpeed();

		void setIsFlying(bool p_bflying);
		bool getIsflying();

	private:



		bool m_bIsFlying;

		sf::Vector2f m_v2Speed;
	
	};
}